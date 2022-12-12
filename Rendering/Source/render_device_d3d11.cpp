#ifdef MING3D_D3D11
#include "render_device_d3d11.h"

#include "Debug/debug.h"
#include "Debug/st_assert.h"
#include "vertex_buffer_d3d11.h"
#include "index_buffer_d3d11.h"
#include "texture_buffer_d3d11.h"
#include "shader_writer_hlsl.h"
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include "shader_constant_d3d11.h"
#include "Debug/debug_stats.h"
#include "shader_info_hlsl.h"
#include "depth_stencil_view_d3d11.h"
#include "blend_state_d3d11.h"

namespace Ming3D::Rendering
{
    RenderDeviceD3D11* GRenderDeviceD3D11 = nullptr;

    RenderDeviceD3D11::RenderDeviceD3D11()
    {
        __AssertComment(GRenderDeviceD3D11 == nullptr, "Can only have one GRenderDeviceD3D11");
        GRenderDeviceD3D11 = this;

#ifdef _DEBUG
        UINT flags = D3D11_CREATE_DEVICE_DEBUG;
#else
        UINT flags = 0;
#endif

        ID3D11Device* baseDevice = nullptr;
        ID3D11DeviceContext* baseDeviceContext = nullptr;
        D3D11CreateDevice(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, flags, NULL, 0, D3D11_SDK_VERSION, &baseDevice, NULL, &baseDeviceContext);
        baseDevice->QueryInterface(__uuidof (ID3D11Device1), reinterpret_cast<void**>(&mDevice));
        baseDeviceContext->QueryInterface(__uuidof (ID3D11DeviceContext1), reinterpret_cast<void**>(&mDeviceContext));

        IDXGIDevice * dxgiDevice = 0;
        HRESULT hr = mDevice->QueryInterface(__uuidof(IDXGIDevice), (void **)& dxgiDevice);
        if (!SUCCEEDED(hr))
        {
            LOG_ERROR() << "Failed to create IDXGIDevice";
            return;
        }

        IDXGIAdapter * dxgiAdapter = 0;
        hr = dxgiDevice->GetParent(__uuidof(IDXGIAdapter), (void **)& dxgiAdapter);
        if (!SUCCEEDED(hr))
        {
            LOG_ERROR() << "Failed to create IDXGIAdapter";
            return;
        }

        mDXGIFactory = 0;
        hr = dxgiAdapter->GetParent(__uuidof(IDXGIFactory), (void **)& mDXGIFactory);
        if (!SUCCEEDED(hr))
        {
            LOG_ERROR() << "Failed to create IDXGIFactory";
            return;
        }

        D3D11_SAMPLER_DESC sampDesc;
        ZeroMemory(&sampDesc, sizeof(sampDesc));
        sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
        sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
        sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
        sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
        sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
        sampDesc.MinLOD = 0;
        sampDesc.MaxLOD = D3D11_FLOAT32_MAX;
        HRESULT hrCreateSampler = GetDevice()->CreateSamplerState(&sampDesc, &mDefaultSamplerState);
        if (!SUCCEEDED(hrCreateSampler))
        {
            LOG_ERROR() << "Failed to create default sampler";
            return;
        }

        mDefaultRasteriserState = (RasteriserStateD3D11*)CreateRasteriserState(RasteriserStateCullMode::Front, true);
        SetRasteriserState(mDefaultRasteriserState);

        DepthStencilStateDesc dssDesc;
        dssDesc.mDepthEnabled = true;
        dssDesc.mDepthWrite = true;
        dssDesc.mDepthFunc = DepthStencilDepthFunc::LEqual;
        mDefaultDepthStencilState = (DepthStencilStateD3D11*)CreateDepthStencilState(dssDesc);
        SetDepthStencilState(mDefaultDepthStencilState);
    }

    RenderDeviceD3D11::~RenderDeviceD3D11()
    {
        if (mDefaultDepthStencilState != nullptr)
        {
            DepthStencilStateD3D11* d3dDepthStencilState = (DepthStencilStateD3D11*)mDefaultDepthStencilState;
            d3dDepthStencilState->mDepthStencilState->Release();
            delete mDefaultDepthStencilState;
        }

        mDevice->Release();
        mDeviceContext->Release();
    }

    void RenderDeviceD3D11::ApplyConstantPacking(const ShaderUniformInfo& inShaderUniform, size_t& outSize)
    {
        switch (inShaderUniform.mType.mDatatype)
        {
        case EShaderDatatype::Float:
            outSize += sizeof(float);
            break;
        case EShaderDatatype::Int:
            outSize +=  sizeof(int); // TODO
            break;
        case EShaderDatatype::Vec2:
            outSize += (16 - (outSize % 16)) % 16;
            outSize += sizeof(DirectX::XMFLOAT2);
            break;
        case EShaderDatatype::Vec3:
            outSize += (16 - (outSize % 16)) % 16;
            outSize += sizeof(DirectX::XMFLOAT3);
            break;
        case EShaderDatatype::Vec4:
            outSize += (16 - (outSize % 16)) % 16;
            outSize += sizeof(DirectX::XMFLOAT4);
            break;
        case EShaderDatatype::Mat4x4:
            outSize += (16 - (outSize % 16)) % 16;
            outSize += sizeof(DirectX::XMFLOAT4X4);
            break;
        default:
            __AssertComment(false, "Unhandled shader uniform type");
        }
    }

    RenderTarget* RenderDeviceD3D11::CreateRenderTarget(RenderWindow* inWindow)
    {
        //__Assert(inWindow->GetOSWindowHandle());
        //__Assert(GRenderDeviceD3D11->GetDXGIFactory());
        RenderTargetD3D11* renderTarget = new RenderTargetD3D11();
        renderTarget->mRenderWindow = inWindow;

        RenderWindowD3D11* renderWindow = (RenderWindowD3D11*)inWindow;

        ID3D11Texture2D* backBufferTex = renderWindow->GetBackBuffer();

        // create render target from back buffer address
        GRenderDeviceD3D11->GetDevice()->CreateRenderTargetView(backBufferTex, NULL, &renderTarget->mBackBuffer);

        // set the render target as the back buffer
        GRenderDeviceD3D11->GetDeviceContext()->OMSetRenderTargets(1, &renderTarget->mBackBuffer, NULL);

        renderTarget->mWidth = inWindow->GetWindow()->GetWidth();
        renderTarget->mHeight = inWindow->GetWindow()->GetHeight();
        renderTarget->mTextureInfo.mWidth = renderTarget->mWidth;
        renderTarget->mTextureInfo.mHeight = renderTarget->mHeight;
        // TODO: set the rest of mTextureInfo???
        
        // create backbuffer resource view
        ID3D11ShaderResourceView* backBufferSRV;
        D3D11_SHADER_RESOURCE_VIEW_DESC backBufferSRVDesc;
        ZeroMemory(&backBufferSRVDesc, sizeof(backBufferSRVDesc));
        backBufferSRVDesc.Format = DXGI_FORMAT_R32_FLOAT;
        backBufferSRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
        backBufferSRVDesc.Texture2D.MipLevels = 1;
        backBufferSRVDesc.Texture2D.MostDetailedMip = -1;
        mDevice->CreateShaderResourceView(backBufferTex, &backBufferSRVDesc, &backBufferSRV);

        TextureBufferD3D11* backBufferTexBuf = new TextureBufferD3D11();
        backBufferTexBuf->mTexture = backBufferTex;
        backBufferTexBuf->mTextureResourceView = backBufferSRV;

        renderTarget->mColourBuffers.push_back(backBufferTexBuf);

        DepthStencilViewD3D11* depthStencilView = CreateDepthStencilView(inWindow->GetWindow()->GetWidth(), inWindow->GetWindow()->GetHeight());
        renderTarget->mDepthStencilView = depthStencilView;

        return renderTarget;
    }

    RenderTarget* RenderDeviceD3D11::CreateRenderTarget(TextureInfo inTextureInfo, int numTextures)
    {
        RenderTargetD3D11* renderTarget = new RenderTargetD3D11();

        renderTarget->mWidth = inTextureInfo.mWidth;
        renderTarget->mHeight = inTextureInfo.mHeight;
        renderTarget->mTextureInfo = inTextureInfo;

        for (int i = 0; i < numTextures; i++)
        {
            TextureBufferD3D11* textureBuffer = new TextureBufferD3D11();

            ID3D11Texture2D* renderTargetTextureMap;
            ID3D11ShaderResourceView* shaderResourceViewMap;

            D3D11_TEXTURE2D_DESC textureDesc;
            D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;

            D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc;
            ID3D11RenderTargetView* renderTargetViewMap;

            ZeroMemory(&textureDesc, sizeof(textureDesc));

            textureDesc.Width = inTextureInfo.mWidth;
            textureDesc.Height = inTextureInfo.mHeight;
            textureDesc.MipLevels = 1;
            textureDesc.ArraySize = 1;
            textureDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
            textureDesc.SampleDesc.Count = 1;
            textureDesc.SampleDesc.Quality = 0;
            textureDesc.Usage = D3D11_USAGE_DEFAULT;
            textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
            textureDesc.CPUAccessFlags = 0;
            textureDesc.MiscFlags = 0;

            mDevice->CreateTexture2D(&textureDesc, NULL, &renderTargetTextureMap);

            // Render target
            renderTargetViewDesc.Format = textureDesc.Format;
            renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
            renderTargetViewDesc.Texture2D.MipSlice = 0;
            mDevice->CreateRenderTargetView(renderTargetTextureMap, &renderTargetViewDesc, &renderTargetViewMap);

            // Shader resource view
            shaderResourceViewDesc.Format = textureDesc.Format;
            shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
            shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
            shaderResourceViewDesc.Texture2D.MipLevels = 1;

            mDevice->CreateShaderResourceView(renderTargetTextureMap, &shaderResourceViewDesc, &shaderResourceViewMap);

            textureBuffer->mTexture = renderTargetTextureMap;
            textureBuffer->mTextureResourceView = shaderResourceViewMap;
            renderTarget->mColourBuffers.push_back(textureBuffer);
            renderTarget->mBackBuffer = renderTargetViewMap; // TEMP TEST
        }

        DepthStencilViewD3D11* depthStencilView = CreateDepthStencilView(inTextureInfo.mWidth, inTextureInfo.mHeight);
        renderTarget->mDepthStencilView = depthStencilView;

        return renderTarget;
    }

    VertexBuffer* RenderDeviceD3D11::CreateVertexBuffer(VertexData* inVertexData, EBufferUsage usage)
    {
        VertexBufferD3D11* vertexBuffer = new VertexBufferD3D11(inVertexData->GetVertexLayout(), usage);
        vertexBuffer->mDataSize = inVertexData->GetVertexSize() * inVertexData->GetNumVertices();
        ID3D11Buffer* vBuffer;

        D3D11_USAGE vbUsage = usage == EBufferUsage::StaticDraw ? D3D11_USAGE_DEFAULT : D3D11_USAGE_DYNAMIC;
        UINT vbAccess = usage == EBufferUsage::StaticDraw ? 0 : D3D11_CPU_ACCESS_WRITE;

        D3D11_BUFFER_DESC vertexBufferDesc;
        ZeroMemory(&vertexBufferDesc, sizeof(vertexBufferDesc));
        vertexBufferDesc.Usage = vbUsage;
        vertexBufferDesc.ByteWidth = vertexBuffer->mDataSize;
        vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
        vertexBufferDesc.CPUAccessFlags = vbAccess;
        mDevice->CreateBuffer(&vertexBufferDesc, NULL, &vBuffer);

        // Fill in the subresource data.
        D3D11_SUBRESOURCE_DATA initData;
        initData.pSysMem = inVertexData->GetDataPtr();
        initData.SysMemPitch = 0;
        initData.SysMemSlicePitch = 0;

        // Create the vertex buffer.
        HRESULT hr = mDevice->CreateBuffer(&vertexBufferDesc, &initData, &vBuffer);
        if (hr != S_OK)
        {
            LOG_ERROR() << "Failed to create vertex buffer";
            delete vertexBuffer;
            return nullptr;
        }

        vertexBuffer->SetD3DBuffer(vBuffer);
        return vertexBuffer;
    }

    void RenderDeviceD3D11::UpdateVertexBuffer(VertexBuffer* inVertexBuffer, VertexData* inVertexData)
    {
        VertexBufferD3D11* vertexBuffer = static_cast<VertexBufferD3D11*>(inVertexBuffer);
        size_t newSize = inVertexData->GetVertexSize() * inVertexData->GetNumVertices();

        // Dynamic vertex buffer with same size => update buffer data
        if (inVertexBuffer->GetUsage() == EBufferUsage::DynamicDraw && newSize == vertexBuffer->mDataSize)
        {
            ID3D11Buffer* vBuffer = vertexBuffer->GetD3DBuffer();

            D3D11_MAPPED_SUBRESOURCE ms;
            mDeviceContext->Map(vBuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);
            memcpy(ms.pData, inVertexData->GetDataPtr(), vertexBuffer->mDataSize);
            mDeviceContext->Unmap(vBuffer, NULL);
        }
        else
        {
            // Delete old buffer
            ID3D11Buffer* oldBuffer = vertexBuffer->GetD3DBuffer();
            oldBuffer->Release();
            vertexBuffer->SetD3DBuffer(nullptr);

            // Create new vertex buffer
            vertexBuffer->mDataSize = newSize;
            D3D11_USAGE vbUsage = vertexBuffer->GetUsage() == EBufferUsage::StaticDraw ? D3D11_USAGE_DEFAULT : D3D11_USAGE_DYNAMIC;
            UINT vbAccess = vertexBuffer->GetUsage() == EBufferUsage::StaticDraw ? 0 : D3D11_CPU_ACCESS_WRITE;
            ID3D11Buffer* vBuffer;
            D3D11_BUFFER_DESC vertexBufferDesc;
            ZeroMemory(&vertexBufferDesc, sizeof(vertexBufferDesc));
            vertexBufferDesc.Usage = vbUsage;
            vertexBufferDesc.ByteWidth = vertexBuffer->mDataSize;
            vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
            vertexBufferDesc.CPUAccessFlags = vbAccess;
            mDevice->CreateBuffer(&vertexBufferDesc, NULL, &vBuffer);

            D3D11_SUBRESOURCE_DATA initData;
            initData.pSysMem = inVertexData->GetDataPtr();
            initData.SysMemPitch = 0;
            initData.SysMemSlicePitch = 0;

            HRESULT hr = mDevice->CreateBuffer(&vertexBufferDesc, &initData, &vBuffer);
            if (hr != S_OK)
            {
                LOG_ERROR() << "Failed to create vertex buffer";
            }

            vertexBuffer->SetD3DBuffer(vBuffer);
        }
    }

    IndexBuffer* RenderDeviceD3D11::CreateIndexBuffer(IndexData* inIndexData, EBufferUsage usage)
    {
        IndexBufferD3D11* indexBuffer = new IndexBufferD3D11(usage);
        ID3D11Buffer* iBuffer;

        D3D11_USAGE ibUsage = usage == EBufferUsage::StaticDraw ? D3D11_USAGE_DEFAULT : D3D11_USAGE_DYNAMIC;
        UINT ibAccess = usage == EBufferUsage::StaticDraw ? 0 : D3D11_CPU_ACCESS_WRITE;

        D3D11_BUFFER_DESC indexBufferDesc;
        
        indexBufferDesc.Usage = ibUsage;
        indexBufferDesc.ByteWidth = sizeof(unsigned long) * inIndexData->GetNumIndices();
        indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
        indexBufferDesc.CPUAccessFlags = ibAccess;
        indexBufferDesc.MiscFlags = 0;
        indexBufferDesc.StructureByteStride = 0;

        D3D11_SUBRESOURCE_DATA indexData;
        
        indexData.pSysMem = inIndexData->GetData();
        indexData.SysMemPitch = 0;
        indexData.SysMemSlicePitch = 0;

        HRESULT result = mDevice->CreateBuffer(&indexBufferDesc, &indexData, &iBuffer);
        if (FAILED(result))
        {
            LOG_ERROR() << "Failed to create index buffer";
            return nullptr;
        }
        indexBuffer->SetD3DBuffer(iBuffer);
        indexBuffer->SetNumIndices(inIndexData->GetNumIndices());
        return indexBuffer;
    }

    void RenderDeviceD3D11::UpdateIndexBuffer(IndexBuffer* inIndexBuffer, IndexData* inIndexData)
    {
        IndexBufferD3D11* indexBuffer = static_cast<IndexBufferD3D11*>(inIndexBuffer);
        size_t oldSize = sizeof(unsigned long) * inIndexBuffer->GetNumIndices();
        size_t newSize = sizeof(unsigned long) * inIndexData->GetNumIndices();

        // Dynamic vertex buffer with same size => update buffer data
        if (inIndexBuffer->GetUsage() == EBufferUsage::DynamicDraw && newSize == oldSize)
        {
            ID3D11Buffer* buffer = indexBuffer->GetD3DBuffer();

            D3D11_MAPPED_SUBRESOURCE ms;
            mDeviceContext->Map(buffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);
            memcpy(ms.pData, inIndexData->GetData(), newSize);
            mDeviceContext->Unmap(buffer, NULL);
        }
        else
        {
            // Delete old buffer
            ID3D11Buffer* oldBuffer = indexBuffer->GetD3DBuffer();
            oldBuffer->Release();

            // Create new index buffer
            D3D11_USAGE ibUsage = inIndexBuffer->GetUsage() == EBufferUsage::StaticDraw ? D3D11_USAGE_DEFAULT : D3D11_USAGE_DYNAMIC;
            UINT ibAccess = inIndexBuffer->GetUsage() == EBufferUsage::StaticDraw ? 0 : D3D11_CPU_ACCESS_WRITE;
            ID3D11Buffer* buffer;
            D3D11_BUFFER_DESC indexBufferDesc;
            ZeroMemory(&indexBufferDesc, sizeof(indexBufferDesc));
            indexBufferDesc.Usage = ibUsage;
            indexBufferDesc.ByteWidth = newSize;
            indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
            indexBufferDesc.CPUAccessFlags = ibAccess;
            mDevice->CreateBuffer(&indexBufferDesc, NULL, &buffer);

            D3D11_SUBRESOURCE_DATA initData;
            initData.pSysMem = inIndexData->GetData();
            initData.SysMemPitch = 0;
            initData.SysMemSlicePitch = 0;

            HRESULT hr = mDevice->CreateBuffer(&indexBufferDesc, &initData, &buffer);
            if (hr != S_OK)
            {
                LOG_ERROR() << "Failed to create index buffer";
            }

            indexBuffer->SetNumIndices(inIndexData->GetNumIndices());
            indexBuffer->SetD3DBuffer(buffer);
        }
    }

    ShaderProgram* RenderDeviceD3D11::CreateShaderProgram(ParsedShaderProgram* parsedProgram)
    {
        ShaderProgramDataHLSL convertedShaderData;
        // Get the converted shaders (if already converted)
        ConvertedShaderProgramHLSL* convertedProgram = static_cast<ConvertedShaderProgramHLSL*>(parsedProgram->mConvertedProgram);
        // Convert shaders to HLSL (if not already converted)
        if (parsedProgram->mConvertedProgram == nullptr)
        {
            ShaderWriterHLSL shaderWriter;
            shaderWriter.WriteShader(parsedProgram, convertedShaderData);

            std::string vertexShaderCode = convertedShaderData.mVertexShader;
            std::string pixelShaderCode = convertedShaderData.mFragmentShader;

            ID3D10Blob* vsBlob;
            ID3D10Blob* psBlob;

            ID3DBlob* errorBlobVS;
            ID3DBlob* errorBlobPS;
            D3DCompile(vertexShaderCode.data(), vertexShaderCode.size(), "", NULL, NULL, "main", "vs_4_0", 0, NULL, &vsBlob, &errorBlobVS);
            D3DCompile(pixelShaderCode.data(), pixelShaderCode.size(), "", NULL, NULL, "main", "ps_4_0", 0, NULL, &psBlob, &errorBlobPS);

            if (errorBlobVS)
            {
                LPVOID err = errorBlobVS->GetBufferPointer();
                LOG_ERROR() << "Error when compiling Vertex Shader: " << parsedProgram->mProgramPath << "\n   " << (char*)err;
                errorBlobVS->Release();
            }
            if (errorBlobPS)
            {
                LPVOID err = errorBlobPS->GetBufferPointer();
                LOG_ERROR() << "Error when compiling Pixel Shader:" << parsedProgram->mProgramPath << "\n   " << (char*)err;
                errorBlobPS->Release();
            }

            // TODO: use placeholder material if null-program is returned
            __AssertComment(vsBlob != nullptr && psBlob != nullptr, "Failed compiling shader - see log for error.");

            if (vsBlob == nullptr || psBlob == nullptr)
                return nullptr;

            convertedProgram = new ConvertedShaderProgramHLSL();
            convertedProgram->vsBlob = vsBlob;
            convertedProgram->psBlob = psBlob;
            parsedProgram->mConvertedProgram = convertedProgram;
        }

        ID3D10Blob* vsBlob = convertedProgram->vsBlob;
        ID3D10Blob* psBlob = convertedProgram->psBlob;
        ID3D11VertexShader* pVS;
        ID3D11PixelShader* pPS;
        // Create shaders
        HRESULT vsRes = mDevice->CreateVertexShader(vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), NULL, &pVS);
        HRESULT psRes = mDevice->CreatePixelShader(psBlob->GetBufferPointer(), psBlob->GetBufferSize(), NULL, &pPS);
        // Set shaders
        mDeviceContext->VSSetShader(pVS, 0, 0);
        mDeviceContext->PSSetShader(pPS, 0, 0);

        // Gather Vertex Components
        std::vector<EVertexComponent> vertexComponents;
        const std::unordered_map<std::string, EVertexComponent> vertexComponentSemanticMap = { { "POSITION", EVertexComponent::Position },{ "NORMAL", EVertexComponent::Normal },{ "TEXCOORD", EVertexComponent::TexCoord },{ "COLOR", EVertexComponent::Colour } };
        for (const ShaderStructMember inputVar : parsedProgram->mVertexShader->mInput.mMemberVariables)
        {
            auto vertCompMatch = vertexComponentSemanticMap.find(inputVar.mSemantic);
            if (vertCompMatch != vertexComponentSemanticMap.end())
            {
                vertexComponents.push_back(vertCompMatch->second);
            }
            else
            {
                LOG_ERROR() << "Invalid semantic: " << inputVar.mSemantic;
            }
        }

        // Create input element descriptions (for input layout)
        const char* vertCompNames[] = { "POSITION", "NORMAL", "TEXCOORD", "COLOR" }; // TEMP - TODO
        const DXGI_FORMAT vertFormats[] = { DXGI_FORMAT_R32G32B32_FLOAT , DXGI_FORMAT_R32G32B32_FLOAT , DXGI_FORMAT_R32G32_FLOAT , DXGI_FORMAT_R32G32B32A32_FLOAT }; // TODO
        UINT byteOffset = 0;
        std::vector<D3D11_INPUT_ELEMENT_DESC> inputElements;
        for (const EVertexComponent& vertexComp : vertexComponents)
        {
            D3D11_INPUT_ELEMENT_DESC desc = { vertCompNames[vertexComp], 0, vertFormats[vertexComp], 0, byteOffset, D3D11_INPUT_PER_VERTEX_DATA, 0 };
            inputElements.push_back(desc);
            byteOffset += VertexData::GetVertexComponentSize(vertexComp);
        }

        // Create input layout
        ID3D11InputLayout* inputLayout;
        mDevice->CreateInputLayout(inputElements.data(), inputElements.size(), vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), &inputLayout);
        
        ShaderProgramD3D11* shaderProgram = new ShaderProgramD3D11();
        shaderProgram->mInputLayout = inputLayout;
        shaderProgram->mVS = pVS;
        shaderProgram->mPS = pPS;

        // Set up array of bound constant buffers
        const size_t numcbuffers = parsedProgram->mConstantBufferInfos.size() + (parsedProgram->mUniforms.size() > 0 ? 1 : 0);
        shaderProgram->mBoundConstantBuffers.resize(numcbuffers);

        for (size_t iCB = 0; iCB < parsedProgram->mConstantBufferInfos.size(); iCB++)
        {
            shaderProgram->mConstantBufferLocations.emplace(parsedProgram->mConstantBufferInfos[iCB].mName, iCB);
        }

        // Crate a constant buffer for all uniforms
        if (parsedProgram->mUniforms.size() > 0)
        {
            size_t cBufferSize = 0;
            for (const ShaderVariableInfo& uniformInfo : parsedProgram->mUniforms)
            {
                size_t offset = cBufferSize;
                offset += (16 - (offset % 16)) % 16; // ensure 16 byte boundary
                ShaderDatatypeInfo varType = uniformInfo.mDatatypeInfo;
                ShaderUniformInfo uniformInfo(varType, uniformInfo.mName);
                ApplyConstantPacking(uniformInfo, cBufferSize);
                ShaderConstantD3D11 scInfo(uniformInfo, offset, cBufferSize - offset);
                shaderProgram->mUniforms.emplace(uniformInfo.mName, scInfo);
            }
            // Add final padding (size must be a multiple of 16)
            cBufferSize += (16 - (cBufferSize % 16) % 16);

            shaderProgram->mUniformsSize = cBufferSize;

            ConstantBufferD3D11* cBuffer = static_cast<ConstantBufferD3D11*>(CreateConstantBuffer(cBufferSize));
            shaderProgram->mUniformCBuffer = cBuffer;
            
            shaderProgram->mBoundConstantBuffers[numcbuffers - 1] = cBuffer->mConstantBuffer;
        }

        return shaderProgram;
    }

    TextureBuffer* RenderDeviceD3D11::CreateTextureBuffer(TextureInfo inTextureInfo, void* inTextureData)
    {
        ADD_DEBUG_STAT_INT("CreateTextureBuffer", 1);
        
        TextureBufferD3D11* textureBuffer = new TextureBufferD3D11();

        __AssertComment(inTextureInfo.mBytesPerPixel % 2 == 0, "Bytes per pixel must be a power of 2");

        const unsigned int w = inTextureInfo.mWidth;
        const unsigned int h = inTextureInfo.mHeight;

        char* buffer = new char[inTextureInfo.mWidth * inTextureInfo.mHeight * inTextureInfo.mBytesPerPixel];
        memcpy(buffer, inTextureData, inTextureInfo.mWidth * inTextureInfo.mHeight * inTextureInfo.mBytesPerPixel);
        textureBuffer->mData = buffer;

        DXGI_FORMAT dxgiFormat = DXGI_FORMAT_R8G8B8A8_UNORM;
        switch (inTextureInfo.mPixelFormat)
        {
        case PixelFormat::RGB:
        {
            if (inTextureInfo.mBytesPerPixel == 12)
            {
                dxgiFormat = DXGI_FORMAT_R32G32B32_FLOAT;
            }
            else
            {
                __AssertComment(0, "Unhandled pixel format");
            }
            break;
        }
        case PixelFormat::RGBA:
        {
            if (inTextureInfo.mBytesPerPixel == 4)
            {
                dxgiFormat = DXGI_FORMAT_R8G8B8A8_UNORM;
            }
            else if (inTextureInfo.mBytesPerPixel == 8)
            {
                dxgiFormat = DXGI_FORMAT_R16G16B16A16_UNORM;

            }
            else if (inTextureInfo.mBytesPerPixel == 16)
            {
                dxgiFormat = DXGI_FORMAT_R32G32B32A32_FLOAT;

            }
            else
            {
                __AssertComment(0, "Unhandled pixel format");
            }
            break;
        }
        case PixelFormat::BGRA:
        {
            if (inTextureInfo.mBytesPerPixel == 4)
            {
                dxgiFormat = DXGI_FORMAT_B8G8R8A8_UNORM;
            }
            else
            {
                __AssertComment(0, "Unhandled pixel format");
            }
            break;
        }
        }

        D3D11_SUBRESOURCE_DATA initData;
        initData.pSysMem = buffer;
        initData.SysMemPitch = w * 4 * sizeof(uint8_t);
        initData.SysMemSlicePitch = w * h * 4 * sizeof(uint8_t);

        D3D11_TEXTURE2D_DESC desc = {};
        desc.Width = w;
        desc.Height = h;
        desc.MipLevels = 1;
        desc.ArraySize = 1;
        desc.Format = dxgiFormat;
        desc.SampleDesc.Count = 1;
        desc.Usage = D3D11_USAGE_IMMUTABLE;
        desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;

        GRenderDeviceD3D11->GetDevice()->CreateTexture2D(&desc, &initData, &textureBuffer->mTexture);
        GRenderDeviceD3D11->GetDevice()->CreateShaderResourceView(textureBuffer->mTexture, NULL, &textureBuffer->mTextureResourceView);

        return textureBuffer;
    }

    RenderWindow* RenderDeviceD3D11::CreateRenderWindow(WindowBase* inWindow)
    {
        RenderWindowD3D11* renderWindow = new RenderWindowD3D11(inWindow);

        DXGI_SWAP_CHAIN_DESC scd;
        ZeroMemory(&scd, sizeof(DXGI_SWAP_CHAIN_DESC));
        
        IDXGISwapChain* swapChain = nullptr;
        scd.BufferCount = 1;
        scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        scd.BufferDesc.Width = inWindow->GetWidth();
        scd.BufferDesc.Height = inWindow->GetHeight();
        scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        scd.OutputWindow = (HWND)inWindow->GetOSWindowHandle();
        scd.SampleDesc.Count = 1;
        scd.Windowed = TRUE;
        scd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

        GRenderDeviceD3D11->GetDXGIFactory()->CreateSwapChain(GRenderDeviceD3D11->GetDevice(), &scd, &swapChain);

        ID3D11Texture2D* backBuffer;
        swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBuffer);

        renderWindow->SetSwapChain(swapChain);
        renderWindow->SetBackBuffer(backBuffer);
        
        return renderWindow;
    }

    RasteriserState* RenderDeviceD3D11::CreateRasteriserState(RasteriserStateCullMode inCullMode, bool inDepthClipEnabled)
    {
        RasteriserStateD3D11* rasteriserState = new RasteriserStateD3D11();
        std::unordered_map<RasteriserStateCullMode, D3D11_CULL_MODE> cullModeMap = { {RasteriserStateCullMode::Back, D3D11_CULL_BACK},{ RasteriserStateCullMode::Front, D3D11_CULL_FRONT },{ RasteriserStateCullMode::None, D3D11_CULL_NONE } };
        
        D3D11_RASTERIZER_DESC rastDesc;
        rastDesc.AntialiasedLineEnable = false;
        rastDesc.DepthBias = 0;
        rastDesc.DepthBiasClamp = 0.0f;
        rastDesc.FillMode = D3D11_FILL_SOLID;
        rastDesc.FrontCounterClockwise = false;
        rastDesc.MultisampleEnable = false;
        rastDesc.ScissorEnable = true;
        rastDesc.SlopeScaledDepthBias = 0.0f;

        rastDesc.CullMode = cullModeMap[inCullMode];
        rastDesc.DepthClipEnable = inDepthClipEnabled;

        rasteriserState->mRasteriserStateDesc = rastDesc;
        GetDevice()->CreateRasterizerState(&rastDesc, &rasteriserState->mRasteriserState);

        return rasteriserState;
    }

    BlendState* RenderDeviceD3D11::CreateBlendState(bool enableBlend, EBlendMode blendMode)
    {
        BlendStateD3D11* blendState = new BlendStateD3D11(enableBlend, blendMode);
        GetDevice()->CreateBlendState(&blendState->mBlendDesc, &blendState->mBlendState);
        return blendState;
    }

    ConstantBuffer* RenderDeviceD3D11::CreateConstantBuffer(size_t inSize)
    {
        ConstantBufferD3D11* cBuffer = new ConstantBufferD3D11();

        ID3D11Buffer*   constantBuffer = NULL;

        char* cBufferData = new char[inSize];

        D3D11_BUFFER_DESC cbDesc;
        cbDesc.ByteWidth = inSize;
        cbDesc.Usage = D3D11_USAGE_DYNAMIC;
        cbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
        cbDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
        cbDesc.MiscFlags = 0;
        cbDesc.StructureByteStride = 0;

        D3D11_SUBRESOURCE_DATA subresData;
        subresData.pSysMem = cBufferData;
        subresData.SysMemPitch = 0;
        subresData.SysMemSlicePitch = 0;

        HRESULT hr = mDevice->CreateBuffer(&cbDesc, &subresData, &constantBuffer);

        if (FAILED(hr))
        {
            LOG_ERROR() << "Failed to create constant buffer";
            return nullptr;
        }

        cBuffer->mConstantBuffer = constantBuffer;
        cBuffer->mConstantData = cBufferData;
        cBuffer->mSize = inSize;

        return cBuffer;
    }

    DepthStencilState* RenderDeviceD3D11::CreateDepthStencilState(DepthStencilStateDesc inDesc)
    {
        DepthStencilStateD3D11* d3dDepthStencilState = new DepthStencilStateD3D11();

        static std::unordered_map<DepthStencilDepthFunc, D3D11_COMPARISON_FUNC> depthFuncMap = { {DepthStencilDepthFunc::Less, D3D11_COMPARISON_LESS },{ DepthStencilDepthFunc::LEqual, D3D11_COMPARISON_LESS_EQUAL },
            { DepthStencilDepthFunc::Equal, D3D11_COMPARISON_EQUAL },{ DepthStencilDepthFunc::GEqual, D3D11_COMPARISON_GREATER_EQUAL },{ DepthStencilDepthFunc::Greater, D3D11_COMPARISON_GREATER } };

        D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
        ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));
        depthStencilDesc.DepthEnable = inDesc.mDepthEnabled;
        depthStencilDesc.DepthWriteMask = inDesc.mDepthWrite ? D3D11_DEPTH_WRITE_MASK_ALL : D3D11_DEPTH_WRITE_MASK_ZERO;
        depthStencilDesc.DepthFunc = depthFuncMap[inDesc.mDepthFunc];
        depthStencilDesc.StencilEnable = true;
        depthStencilDesc.StencilReadMask = 0xFF;
        depthStencilDesc.StencilWriteMask = 0xFF;
        depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
        depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
        depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
        depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
        depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
        depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
        depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
        depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
        d3dDepthStencilState->mDepthStencilStateDesc = depthStencilDesc;
        HRESULT result = GetDevice()->CreateDepthStencilState(&depthStencilDesc, &d3dDepthStencilState->mDepthStencilState);
        if (FAILED(result))
        {
            LOG_ERROR() << "Faield to create depth stencil state";
            delete d3dDepthStencilState;
            return nullptr;
        }

        return d3dDepthStencilState;
    }

    DepthStencilViewD3D11* RenderDeviceD3D11::CreateDepthStencilView(int inWidth, int inHeight)
    {
        DepthStencilViewD3D11* d3dDepthStencilView = new DepthStencilViewD3D11();

        ID3D11Texture2D* texture = nullptr;
        ID3D11ShaderResourceView* shaderResourceView = nullptr;
        ID3D11DepthStencilView* depthStencilView;

        // Create depth stencil texture
        D3D11_TEXTURE2D_DESC descDepth;
        ZeroMemory(&descDepth, sizeof(descDepth));
        descDepth.Width = inWidth;
        descDepth.Height = inHeight;
        descDepth.MipLevels = 1;
        descDepth.ArraySize = 1;
        descDepth.Format = DXGI_FORMAT_R32_TYPELESS;
        descDepth.SampleDesc.Count = 1;
        descDepth.SampleDesc.Quality = 0;
        descDepth.Usage = D3D11_USAGE_DEFAULT;
        descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
        descDepth.CPUAccessFlags = 0;
        descDepth.MiscFlags = 0;
        descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;

        mDevice->CreateTexture2D(&descDepth, nullptr, &texture);

        D3D11_SHADER_RESOURCE_VIEW_DESC depthSRVDesc;
        ZeroMemory(&depthSRVDesc, sizeof(depthSRVDesc));
        depthSRVDesc.Format = DXGI_FORMAT_R32_FLOAT;
        depthSRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
        depthSRVDesc.Texture2D.MipLevels = 1;
        depthSRVDesc.Texture2D.MostDetailedMip = 0;
        mDevice->CreateShaderResourceView(texture, &depthSRVDesc, &shaderResourceView);

        // Create depth stencil view
        D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
        ZeroMemory(&descDSV, sizeof(descDSV));

        descDSV.Format = DXGI_FORMAT_D32_FLOAT;
        descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS;
        descDSV.Texture2D.MipSlice = 0;;

        HRESULT result = GetDevice()->CreateDepthStencilView(texture, &descDSV, &depthStencilView);
        if (FAILED(result))
        {
            LOG_ERROR() << "Faield to create depth stencil view";
            delete d3dDepthStencilView;
            return nullptr;
        }

        d3dDepthStencilView->mDepthStencilTexture = new TextureBufferD3D11();
        d3dDepthStencilView->mDepthStencilTexture->mTexture = texture;
        d3dDepthStencilView->mDepthStencilTexture->mTextureResourceView = shaderResourceView;
        d3dDepthStencilView->mDepthStencilView = depthStencilView;

        return d3dDepthStencilView;
    }

    void RenderDeviceD3D11::SetTexture(const TextureBuffer* inTexture, int inSlot)
    {
        ADD_FRAME_STAT_INT("SetTexture", 1);

        TextureBufferD3D11* d3dTexture = (TextureBufferD3D11*)inTexture;
        GetDeviceContext()->PSSetSamplers(inSlot, 1, &mDefaultSamplerState);
        GetDeviceContext()->PSSetShaderResources(inSlot, 1, &d3dTexture->mTextureResourceView);
    }

    void RenderDeviceD3D11::SetActiveShaderProgram(ShaderProgram* inProgram)
    {
        ADD_FRAME_STAT_INT("SetActiveShaderProgram", 1);

        __AssertComment(inProgram, "SetActiveShaderProgram called with null program");

        ShaderProgramD3D11* dxShaderProgram = (ShaderProgramD3D11*)inProgram;
        mActiveShaderProgram = dxShaderProgram;
        mDeviceContext->VSSetShader(dxShaderProgram->mVS, 0, 0);
        mDeviceContext->PSSetShader(dxShaderProgram->mPS, 0, 0);

        // Bind constant buffers
        mDeviceContext->VSSetConstantBuffers(0, dxShaderProgram->mBoundConstantBuffers.size(), dxShaderProgram->mBoundConstantBuffers.data());
        mDeviceContext->PSSetConstantBuffers(0, dxShaderProgram->mBoundConstantBuffers.size(), dxShaderProgram->mBoundConstantBuffers.data());
    }

    void RenderDeviceD3D11::BeginRenderWindow(RenderWindow* inWindow)
    {
        mRenderWindow = (RenderWindowD3D11*)inWindow;
        mRenderWindow->GetWindow()->BeginRender();
    }

    void RenderDeviceD3D11::EndRenderWindow(RenderWindow* inWindow)
    {
        __Assert(mRenderWindow == inWindow);

        mRenderWindow->GetWindow()->EndRender();

        mRenderWindow->GetSwapChain()->Present(0, 0);
        mRenderWindow = nullptr;
    }

    void RenderDeviceD3D11::SetRenderTarget(RenderTarget* inTarget)
    {
        if (inTarget == nullptr)
        {
            ID3D11RenderTargetView* nullRTV = nullptr;
            GRenderDeviceD3D11->GetDeviceContext()->OMSetRenderTargets(1, &nullRTV, nullptr);
            mRenderTarget = nullptr;
        }
        else
        {
            mRenderTarget = static_cast<RenderTargetD3D11*>(inTarget);
            GRenderDeviceD3D11->GetDeviceContext()->OMSetRenderTargets(1, &mRenderTarget->mBackBuffer, mRenderTarget->mDepthStencilView->mDepthStencilView);
        }
    }

    void RenderDeviceD3D11::BlitRenderTargetToWindow(RenderTarget* target, RenderWindow* window)
    {

    }

    void RenderDeviceD3D11::BeginViewport(unsigned int x, unsigned int y, unsigned int width, unsigned int height)
    {
        __Assert(mRenderTarget != nullptr);

        const unsigned int rtHeight = mRenderTarget->mHeight;

        // Convert coordinate space
        y = rtHeight - (y + height);

        D3D11_VIEWPORT viewport;
        ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));
        viewport.TopLeftX = static_cast<float>(x);
        viewport.TopLeftY = static_cast<float>(y);
        viewport.MinDepth = 0;
        viewport.MaxDepth = 1;
        viewport.Width = static_cast<float>(width);
        viewport.Height = static_cast<float>(height);
        GRenderDeviceD3D11->GetDeviceContext()->RSSetViewports(1, &viewport);

        D3D11_RECT rect{};
        rect.left = x;
        rect.top = y;
        rect.right = x + width;
        rect.bottom = y + height;
        mDeviceContext->RSSetScissorRects(1, &rect);

        const float clearCol[4] = { 0.0f, 0.2f, 0.4f, 1.0f }; // TODO: Allow adjusting the clear colour
        mDeviceContext->ClearView(mRenderTarget->GetBackBuffer(), clearCol, &rect, 1); // TODO: Don't use scissor/viewport? (render to texture instead)
        //mDeviceContext->ClearRenderTargetView(mRenderTarget->GetBackBuffer(), clearCol);
        mDeviceContext->ClearDepthStencilView(mRenderTarget->mDepthStencilView->mDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
    }

    void RenderDeviceD3D11::RenderPrimitive(VertexBuffer* inVertexBuffer, IndexBuffer* inIndexBuffer, unsigned int startIndex, unsigned int indexCount)
    {
        ADD_FRAME_STAT_INT("RenderPrimitive", 1);
        
        VertexBufferD3D11* vertexBufferDX = (VertexBufferD3D11*)inVertexBuffer;
        IndexBufferD3D11* indexBufferDX = (IndexBufferD3D11*)inIndexBuffer;

        ID3D11Buffer* vb = vertexBufferDX->GetD3DBuffer();

        mDeviceContext->IASetInputLayout(mActiveShaderProgram->mInputLayout);

        UINT stride = inVertexBuffer->GetVertexSize();
        UINT offset = 0;
        mDeviceContext->IASetVertexBuffers(0, 1, &vb, &stride, &offset);
        mDeviceContext->IASetIndexBuffer(indexBufferDX->GetD3DBuffer(), DXGI_FORMAT_R32_UINT, 0);
        
        mDeviceContext->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
        
        mDeviceContext->DrawIndexed(indexCount, startIndex, 0);
    }

    void RenderDeviceD3D11::SetRasteriserState(RasteriserState* inState)
    {
        if (inState == nullptr)
            inState = mDefaultRasteriserState; // if null: set to default

        RasteriserStateD3D11* d3dState = (RasteriserStateD3D11*)inState;
        GetDeviceContext()->RSSetState(d3dState->mRasteriserState);
    }

    void RenderDeviceD3D11::SetDepthStencilState(DepthStencilState* inState)
    {
        DepthStencilStateD3D11* depthStencilState = (DepthStencilStateD3D11*)inState;
        GetDeviceContext()->OMSetDepthStencilState(depthStencilState->mDepthStencilState, 1);
    }

    void RenderDeviceD3D11::SetBlendState(BlendState* inState)
    {
        BlendStateD3D11* blendState = (BlendStateD3D11*)inState;
        GetDeviceContext()->OMSetBlendState(blendState->mBlendState, blendState->mBlendFactor, 0xffffffff);
    }

    void RenderDeviceD3D11::SetConstantBufferData(ConstantBuffer* inConstantBuffer, void* inData, size_t inSize)
    {
        ConstantBufferD3D11* cbuffer = static_cast<ConstantBufferD3D11*>(inConstantBuffer);

        size_t size = inSize;
        assert(size == size);

        memcpy((char*)cbuffer->mConstantData, inData, inSize);

        D3D11_MAPPED_SUBRESOURCE mappedResource;
        mDeviceContext->Map(cbuffer->mConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);

        memcpy(mappedResource.pData, cbuffer->mConstantData, size);

        mDeviceContext->Unmap(cbuffer->mConstantBuffer, 0);
    }

    void RenderDeviceD3D11::BindConstantBuffer(ConstantBuffer* inConstantBuffer, const char* inName, ShaderProgram* inProgram)
    {
        ShaderProgramD3D11* shaderProgram = static_cast<ShaderProgramD3D11*>(inProgram);
        ConstantBufferD3D11* cbuffer = static_cast<ConstantBufferD3D11*>(inConstantBuffer);

        auto iter = shaderProgram->mConstantBufferLocations.find(inName);
        if (iter != shaderProgram->mConstantBufferLocations.end())
        {
            shaderProgram->mBoundConstantBuffers[iter->second] = cbuffer->mConstantBuffer;
        }
        else
        {
            LOG_ERROR() << "Constant buffer does not exist: " << inName;
        }
    }

    void RenderDeviceD3D11::SetUniformCBufferData(const std::string& inName, const void* inData, size_t inSize)
    {
        __Assert(mActiveShaderProgram != nullptr);

        ADD_FRAME_STAT_INT("SetConstantBufferData", 1);

        bool foundUniform = false;

        auto scIter = mActiveShaderProgram->mUniforms.find(inName);
        if (scIter != mActiveShaderProgram->mUniforms.end())
        {
            const ConstantBufferD3D11* cBuffer = mActiveShaderProgram->mUniformCBuffer;
            const ShaderConstantD3D11& scInfo = scIter->second;

            void* shaderConstData = cBuffer->mConstantData;

            memcpy((char*)shaderConstData + scInfo.mOffset, inData, inSize);

            D3D11_MAPPED_SUBRESOURCE mappedResource;
            mDeviceContext->Map(cBuffer->mConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);

            memcpy(mappedResource.pData, shaderConstData, cBuffer->mSize);

            mDeviceContext->Unmap(cBuffer->mConstantBuffer, 0);
        }
        else
        {
            LOG_ERROR() << "Could not find shader constant: " << inName;
        }
    }

    void RenderDeviceD3D11::SetShaderUniformFloat(const std::string& inName, float inVal)
    {
        SetUniformCBufferData(inName, &inVal, sizeof(inVal));
    }

    void RenderDeviceD3D11::SetShaderUniformInt(const std::string& inName, int inVal)
    {
        SetUniformCBufferData(inName, &inVal, sizeof(inVal));
    }

    void RenderDeviceD3D11::SetShaderUniformVec2(const std::string& inName, const glm::vec2 inVec)
    {
        DirectX::XMFLOAT2 value(&inVec[0]);
        SetUniformCBufferData(inName, &value, sizeof(value));
    }

    void RenderDeviceD3D11::SetShaderUniformVec3(const std::string& inName, const glm::vec3 inVec)
    {
        DirectX::XMFLOAT3 value(&inVec[0]);
        SetUniformCBufferData(inName, &value, sizeof(value));
    }

    void RenderDeviceD3D11::SetShaderUniformVec4(const std::string& inName, const glm::vec4 inVec)
    {
        DirectX::XMFLOAT4 value(&inVec[0]);
        SetUniformCBufferData(inName, &value, sizeof(value));
    }

    void RenderDeviceD3D11::SetShaderUniformMat4x4(const std::string& inName, const glm::mat4 inMat)
    {
        glm::mat4 mat = glm::transpose(inMat);
        DirectX::XMFLOAT4X4 value(&mat[0][0]);
        SetUniformCBufferData(inName, &value, sizeof(value));
    }

}
#endif

