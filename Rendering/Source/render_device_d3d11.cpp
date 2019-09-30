#include "render_device_d3d11.h"

#include "Debug/debug.h"
#include "Debug/st_assert.h"
#include "vertex_buffer_d3d11.h"
#include "index_buffer_d3d11.h"
#include "texture_buffer_d3d11.h"
#include "shader_writer_hlsl.h"
#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include "shader_constant_d3d11.h"
#include "Debug/debug_stats.h"

namespace Ming3D
{
    RenderDeviceD3D11* GRenderDeviceD3D11 = nullptr;

    RenderDeviceD3D11::RenderDeviceD3D11()
    {
        __AssertComment(GRenderDeviceD3D11 == nullptr, "Can only have one GRenderDeviceD3D11");
        GRenderDeviceD3D11 = this;

        D3D11CreateDevice(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, 0, NULL, 0, D3D11_SDK_VERSION, &mDevice, NULL, &mDeviceContext);

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

        mDefaultDepthStencilState = (DepthStencilStateD3D11*)CreateDepthStencilState(DepthStencilDepthFunc::LEqual, true);
        SetDepthStencilState(mDefaultDepthStencilState);
    }

    RenderDeviceD3D11::~RenderDeviceD3D11()
    {
        if (mDefaultDepthStencilState != nullptr)
        {
            DepthStencilStateD3D11* d3dDepthStencilState = (DepthStencilStateD3D11*)mDefaultDepthStencilState;
            d3dDepthStencilState->mDepthStencilTexture->Release();
            d3dDepthStencilState->mDepthStencilView->Release();
            d3dDepthStencilState->mDepthStencilState->Release();
            delete mDefaultDepthStencilState;
        }

        mDevice->Release();
        mDeviceContext->Release();
    }

    size_t RenderDeviceD3D11::GetShaderUniformSize(const ShaderUniformInfo& inShaderUniform)
    {
        switch (inShaderUniform.mType.mDatatype)
        {
        case EShaderDatatype::Float:
            return sizeof(float);
            break;
        case EShaderDatatype::Int:
            return sizeof(int);
            break;
        case EShaderDatatype::Vec2:
            return sizeof(DirectX::XMFLOAT2);
            break;
        case EShaderDatatype::Vec3:
            return sizeof(DirectX::XMFLOAT3);
            break;
        case EShaderDatatype::Vec4:
            return sizeof(DirectX::XMFLOAT4);
            break;
        case EShaderDatatype::Mat4x4:
            return sizeof(DirectX::XMFLOAT4X4);
            break;
        default:
            __AssertComment(false, "Unhandled shader uniform type");
            return 0;
        }
    }

    RenderTarget* RenderDeviceD3D11::CreateRenderTarget(RenderWindow* inWindow)
    {
        //__Assert(inWindow->GetOSWindowHandle());
        //__Assert(GRenderDeviceD3D11->GetDXGIFactory());
        RenderTargetD3D11* renderTarget = new RenderTargetD3D11();
        
        RenderWindowD3D11* renderWindow = (RenderWindowD3D11*)inWindow;

        // create render target from back buffer address
        GRenderDeviceD3D11->GetDevice()->CreateRenderTargetView(renderWindow->GetBackBuffer(), NULL, &renderTarget->mBackBuffer);

        // set the render target as the back buffer
        GRenderDeviceD3D11->GetDeviceContext()->OMSetRenderTargets(1, &renderTarget->mBackBuffer, NULL);


        // Set the viewport
        D3D11_VIEWPORT viewport;
        ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));

        viewport.TopLeftX = 0;
        viewport.TopLeftY = 0;
        viewport.MinDepth = 0;
        viewport.MaxDepth = 1;
        viewport.Width = (float)inWindow->GetWindow()->GetWidth();
        viewport.Height = (float)inWindow->GetWindow()->GetHeight();

        GRenderDeviceD3D11->GetDeviceContext()->RSSetViewports(1, &viewport);

        return renderTarget;
    }

    RenderTarget* RenderDeviceD3D11::CreateRenderTarget(TextureInfo inTextureInfo, int numTextures)
    {
        RenderTargetD3D11* renderTarget = new RenderTargetD3D11();

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

            textureBuffer->mTextureResourceView = shaderResourceViewMap;
            renderTarget->mColourBuffers.push_back(textureBuffer);
            renderTarget->mBackBuffer = renderTargetViewMap; // TEMP TEST
        }

        return renderTarget;
    }

    VertexBuffer* RenderDeviceD3D11::CreateVertexBuffer(VertexData* inVertexData)
    {
        VertexBufferD3D11* vertexBuffer = new VertexBufferD3D11();
        ID3D11Buffer* vBuffer;

        D3D11_BUFFER_DESC vertexBufferDesc;
        ZeroMemory(&vertexBufferDesc, sizeof(vertexBufferDesc));
        vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
        vertexBufferDesc.ByteWidth = inVertexData->GetVertexSize() * inVertexData->GetNumVertices();                                                                        
        vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
        vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
        mDevice->CreateBuffer(&vertexBufferDesc, NULL, &vBuffer);

        D3D11_MAPPED_SUBRESOURCE ms;
        mDeviceContext->Map(vBuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);

        memcpy(ms.pData, inVertexData->GetDataPtr(), inVertexData->GetVertexSize() * inVertexData->GetNumVertices());
        
        mDeviceContext->Unmap(vBuffer, NULL);

        vertexBuffer->SetD3DBuffer(vBuffer);
        vertexBuffer->SetVertexLayout(inVertexData->GetVertexLayout());
        return vertexBuffer;
    }

    IndexBuffer* RenderDeviceD3D11::CreateIndexBuffer(IndexData* inIndexData)
    {
        IndexBufferD3D11* indexBuffer = new IndexBufferD3D11();
        ID3D11Buffer* iBuffer;

        D3D11_BUFFER_DESC indexBufferDesc;
        
        indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
        indexBufferDesc.ByteWidth = sizeof(unsigned long) * inIndexData->GetNumIndices();
        indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
        indexBufferDesc.CPUAccessFlags = 0;
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

    ShaderProgram* RenderDeviceD3D11::CreateShaderProgram(const ParsedShaderProgram* parsedProgram)
    {
        ShaderWriterHLSL shaderWriter;
        ShaderProgramDataHLSL convertedShaderData;
        shaderWriter.WriteShader(parsedProgram, convertedShaderData);

        std::string vertexShaderCode = convertedShaderData.mVertexShader.mSource;
        std::string pixelShaderCode = convertedShaderData.mFragmentShader.mSource;

        std::vector<EVertexComponent> vertexComponents;
        const std::unordered_map<std::string, EVertexComponent> vertexComponentSemanticMap = { {"POSITION", EVertexComponent::Position},{ "NORMAL", EVertexComponent::Normal },{ "TEXCOORD", EVertexComponent::TexCoord },{ "COLOR", EVertexComponent::Colour } };
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

        ID3D10Blob* vsBlob;
        ID3D10Blob* psBlob;
        ID3D11VertexShader* pVS;
        ID3D11PixelShader* pPS;
        ID3D11InputLayout* inputLayout;

        ID3DBlob* errorBlobVS;
        ID3DBlob* errorBlobPS;
        D3DCompile(vertexShaderCode.data(), vertexShaderCode.size(), "", NULL, NULL, "main", "vs_4_0", 0, NULL, &vsBlob, &errorBlobVS);
        D3DCompile(pixelShaderCode.data(), pixelShaderCode.size(), "", NULL, NULL, "main", "ps_4_0", 0, NULL, &psBlob, &errorBlobPS);

        if (errorBlobVS)
        {
            OutputDebugStringA((char*)errorBlobVS->GetBufferPointer());
            errorBlobVS->Release();
        }
        if (errorBlobPS)
        {
            OutputDebugStringA((char*)errorBlobPS->GetBufferPointer());
            errorBlobPS->Release();
        }

        HRESULT vsRes = mDevice->CreateVertexShader(vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), NULL, &pVS);
        HRESULT psRes = mDevice->CreatePixelShader(psBlob->GetBufferPointer(), psBlob->GetBufferSize(), NULL, &pPS);

        mDeviceContext->VSSetShader(pVS, 0, 0);
        mDeviceContext->PSSetShader(pPS, 0, 0);

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

        mDevice->CreateInputLayout(inputElements.data(), inputElements.size(), vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), &inputLayout);
        
        ShaderProgramD3D11* shaderProgram = new ShaderProgramD3D11();
        shaderProgram->mInputLayout = inputLayout;
        shaderProgram->mVS = pVS;
        shaderProgram->mPS = pPS;

        std::vector<ShaderDataHLSL> shaderDataList;
        shaderDataList.push_back(convertedShaderData.mVertexShader);
        shaderDataList.push_back(convertedShaderData.mFragmentShader);

        const size_t numCBuffers = parsedProgram->mShaderUniformBlocks.size();
        for (size_t iCBuffer = 0; iCBuffer < numCBuffers; iCBuffer++)
        {
            const ShaderUniformBlock& uniformBlock = parsedProgram->mShaderUniformBlocks[iCBuffer];
            ConstantBufferD3D11* cBuffer = new ConstantBufferD3D11();

            size_t cBufferSize = 0;
            for(size_t iVar = 0; iVar < uniformBlock.mShaderUniforms.size(); iVar++)
            {
                const ShaderVariableInfo& parserUniformInfo = uniformBlock.mShaderUniforms[iVar];
                shaderProgram->mConstantNameMap.emplace(parserUniformInfo.mName, ShaderConstantRef{iCBuffer, iVar});

                ShaderDatatypeInfo varType = parserUniformInfo.mDatatypeInfo;
                ShaderUniformInfo uniformInfo(varType, parserUniformInfo.mName);
                const size_t currentUniformSize = GetShaderUniformSize(uniformInfo);
                ShaderConstantD3D11 scInfo(uniformInfo, cBufferSize, currentUniformSize);
                cBuffer->mShaderConstants.push_back(scInfo);
                cBufferSize += currentUniformSize;
            }
            ID3D11Buffer*   constantBuffer = NULL;

            char* cBufferData = new char[cBufferSize];

            D3D11_BUFFER_DESC cbDesc;
            cbDesc.ByteWidth = cBufferSize;
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
            cBuffer->mShaderConstantsSize = cBufferSize;
            shaderProgram->mConstantBuffers.push_back(cBuffer);
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

        ID3D11Texture2D *boxTex = 0;

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

        GRenderDeviceD3D11->GetDevice()->CreateTexture2D(&desc, &initData, &boxTex);
        GRenderDeviceD3D11->GetDevice()->CreateShaderResourceView(boxTex, NULL, &textureBuffer->mTextureResourceView);

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
        scd.SampleDesc.Count = 4;
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
        rastDesc.ScissorEnable = false;
        rastDesc.SlopeScaledDepthBias = 0.0f;

        rastDesc.CullMode = cullModeMap[inCullMode];
        rastDesc.DepthClipEnable = inDepthClipEnabled;

        rasteriserState->mRasteriserStateDesc = rastDesc;
        GetDevice()->CreateRasterizerState(&rastDesc, &rasteriserState->mRasteriserState);

        return rasteriserState;
    }

    DepthStencilState* RenderDeviceD3D11::CreateDepthStencilState(DepthStencilDepthFunc inDepthFunc, bool inDepthEnabled)
    {
        DepthStencilStateD3D11* d3dDepthStencilState = new DepthStencilStateD3D11();

        std::unordered_map<DepthStencilDepthFunc, D3D11_COMPARISON_FUNC> depthFuncMap = { {DepthStencilDepthFunc::Less, D3D11_COMPARISON_LESS },{ DepthStencilDepthFunc::LEqual, D3D11_COMPARISON_LESS_EQUAL },
            { DepthStencilDepthFunc::Equal, D3D11_COMPARISON_EQUAL },{ DepthStencilDepthFunc::GEqual, D3D11_COMPARISON_GREATER_EQUAL },{ DepthStencilDepthFunc::Greater, D3D11_COMPARISON_GREATER } };
        
        D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
        ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));
        depthStencilDesc.DepthEnable = inDepthEnabled;
        depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
        depthStencilDesc.DepthFunc = depthFuncMap[inDepthFunc];
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

        // Create depth stencil texture
        D3D11_TEXTURE2D_DESC descDepth;
        ZeroMemory(&descDepth, sizeof(descDepth));
        descDepth.Width = 1280; // TODO: I guess this should match the render target?
        descDepth.Height = 960; // TODO: I guess this should match the render target?   
        descDepth.MipLevels = 1;
        descDepth.ArraySize = 1;
        descDepth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
        descDepth.SampleDesc.Count = 4;
        descDepth.SampleDesc.Quality = 0;
        descDepth.Usage = D3D11_USAGE_DEFAULT;
        descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
        descDepth.CPUAccessFlags = 0;
        descDepth.MiscFlags = 0;

        mDevice->CreateTexture2D(&descDepth, nullptr, &d3dDepthStencilState->mDepthStencilTexture);

        // Create depth stencil view
        D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
        ZeroMemory(&descDSV, sizeof(descDSV));

        descDSV.Format = descDepth.Format;
        descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS;
        descDSV.Texture2D.MipSlice = 0;;

        result = GetDevice()->CreateDepthStencilView(d3dDepthStencilState->mDepthStencilTexture, &descDSV, &d3dDepthStencilState->mDepthStencilView);
        if (FAILED(result))
        {
            LOG_ERROR() << "Faield to create depth stencil view";
            delete d3dDepthStencilState;
            return nullptr;
        }

        return d3dDepthStencilState;
    }

    void RenderDeviceD3D11::SetTexture(const TextureBuffer* inTexture, int inSlot)
    {
        ADD_FRAME_STAT_INT("SetTexture", 1);

        TextureBufferD3D11* d3dTexture = (TextureBufferD3D11*)inTexture;
        GetDeviceContext()->PSSetSamplers(inSlot, 1, &mDefaultSamplerState);
        GetDeviceContext()->PSSetShaderResources(0, 1, &d3dTexture->mTextureResourceView);
    }

    void RenderDeviceD3D11::SetActiveShaderProgram(ShaderProgram* inProgram)
    {
        ADD_FRAME_STAT_INT("SetActiveShaderProgram", 1);

        __AssertComment(inProgram, "SetActiveShaderProgram called with null program");

        ShaderProgramD3D11* dxShaderProgram = (ShaderProgramD3D11*)inProgram;
        mActiveShaderProgram = dxShaderProgram;
        mDeviceContext->VSSetShader(dxShaderProgram->mVS, 0, 0);
        mDeviceContext->PSSetShader(dxShaderProgram->mPS, 0, 0);

        for (const ConstantBufferD3D11* cBuffer : dxShaderProgram->mConstantBuffers)
        {
            // TODO: Check if cbuffer is used by each shader
            mDeviceContext->VSSetConstantBuffers(0, 1, &cBuffer->mConstantBuffer);
            mDeviceContext->PSSetConstantBuffers(0, 1, &cBuffer->mConstantBuffer);
        }
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

    void RenderDeviceD3D11::BeginRenderTarget(RenderTarget* inTarget)
    {
        mRenderTarget = (RenderTargetD3D11*)inTarget;

        mRenderTarget->BeginRendering();

        GRenderDeviceD3D11->GetDeviceContext()->OMSetRenderTargets(1, &mRenderTarget->mBackBuffer, mDefaultDepthStencilState->mDepthStencilView);

        const float clearCol[4] = { 0.0f, 0.2f, 0.4f, 1.0f };
        mDeviceContext->ClearRenderTargetView(mRenderTarget->GetBackBuffer(), clearCol);
        mDeviceContext->ClearDepthStencilView(mDefaultDepthStencilState->mDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

    }

    void RenderDeviceD3D11::EndRenderTarget(RenderTarget* inTarget)
    {
        __Assert(mRenderTarget == inTarget);

        mRenderTarget->EndRendering();

        mRenderTarget = nullptr;
    }

    void RenderDeviceD3D11::RenderPrimitive(VertexBuffer* inVertexBuffer, IndexBuffer* inIndexBuffer)
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
        
        mDeviceContext->DrawIndexed(inIndexBuffer->GetNumIndices(), 0, 0);
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

    void RenderDeviceD3D11::SetConstantBufferData(const std::string& inName, const void* inData, size_t inSize)
    {
        __Assert(mActiveShaderProgram != nullptr);

        ADD_FRAME_STAT_INT("SetConstantBufferData", 1);

        bool foundUniform = false;

        auto scIter = mActiveShaderProgram->mConstantNameMap.find(inName);
        if (scIter != mActiveShaderProgram->mConstantNameMap.end())
        {
            const ShaderConstantRef& scRef = scIter->second;
            const ConstantBufferD3D11* cBuffer = mActiveShaderProgram->mConstantBuffers[scRef.mCBufferIndex];
            const ShaderConstantD3D11& scInfo = cBuffer->mShaderConstants[scRef.mConstantIndex];

            void* shaderConstData = cBuffer->mConstantData;

            memcpy((char*)shaderConstData + scInfo.mOffset, inData, inSize);

            D3D11_MAPPED_SUBRESOURCE mappedResource;
            mDeviceContext->Map(cBuffer->mConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);

            memcpy(mappedResource.pData, shaderConstData, cBuffer->mShaderConstantsSize);

            mDeviceContext->Unmap(cBuffer->mConstantBuffer, 0);
        }
        else
        {
            LOG_ERROR() << "Could not find shader constant: " << inName;
        }
    }

    void RenderDeviceD3D11::SetShaderUniformFloat(const std::string& inName, float inVal)
    {
        SetConstantBufferData(inName, &inVal, sizeof(inVal));
    }

    void RenderDeviceD3D11::SetShaderUniformInt(const std::string& inName, int inVal)
    {
        SetConstantBufferData(inName, &inVal, sizeof(inVal));
    }

    void RenderDeviceD3D11::SetShaderUniformVec2(const std::string& inName, const glm::vec2 inVec)
    {
        DirectX::XMFLOAT2 value(&inVec[0]);
        SetConstantBufferData(inName, &value, sizeof(value));
    }

    void RenderDeviceD3D11::SetShaderUniformVec3(const std::string& inName, const glm::vec3 inVec)
    {
        DirectX::XMFLOAT3 value(&inVec[0]);
        SetConstantBufferData(inName, &value, sizeof(value));
    }

    void RenderDeviceD3D11::SetShaderUniformVec4(const std::string& inName, const glm::vec4 inVec)
    {
        DirectX::XMFLOAT4 value(&inVec[0]);
        SetConstantBufferData(inName, &value, sizeof(value));
    }

    void RenderDeviceD3D11::SetShaderUniformMat4x4(const std::string& inName, const glm::mat4 inMat)
    {
        glm::mat4 mat = glm::transpose(inMat);
        DirectX::XMFLOAT4X4 value(&mat[0][0]);
        SetConstantBufferData(inName, &value, sizeof(value));
    }

}
