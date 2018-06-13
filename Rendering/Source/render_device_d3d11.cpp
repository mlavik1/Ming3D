#include "render_device_d3d11.h"

#include "Debug/debug.h"
#include "Debug/st_assert.h"
#include "vertex_buffer_d3d11.h"
#include "index_buffer_d3d11.h"
#include <D3Dcompiler.h>
#include <DirectXMath.h>
#include "texture_buffer_d3d11.h"

#include "shader_writer_hlsl.h"

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

        D3D11_RASTERIZER_DESC rastDesc;
        rastDesc.AntialiasedLineEnable = false;
        rastDesc.CullMode = D3D11_CULL_FRONT;
        rastDesc.DepthBias = 0;
        rastDesc.DepthBiasClamp = 0.0f;
        rastDesc.DepthClipEnable = true;
        rastDesc.FillMode = D3D11_FILL_SOLID;
        rastDesc.FrontCounterClockwise = false;
        rastDesc.MultisampleEnable = false;
        rastDesc.ScissorEnable = false;
        rastDesc.SlopeScaledDepthBias = 0.0f;
        GRenderDeviceD3D11->GetDevice()->CreateRasterizerState(&rastDesc, &mDefaultRasterState);
        GRenderDeviceD3D11->GetDeviceContext()->RSSetState(mDefaultRasterState);

        // Initialize the description of the default stencil state.
        D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
        ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));
        depthStencilDesc.DepthEnable = true;
        depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
        depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;
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

        HRESULT result = GRenderDeviceD3D11->GetDevice()->CreateDepthStencilState(&depthStencilDesc, &mDefaultDepthStencilState);
        if (FAILED(result))
        {
            return;
        }
        // Set the depth stencil state.
        GRenderDeviceD3D11->GetDeviceContext()->OMSetDepthStencilState(mDefaultDepthStencilState, 1);
    }

    RenderDeviceD3D11::~RenderDeviceD3D11()
    {
        mDevice->Release();
        mDeviceContext->Release();
    }

    size_t RenderDeviceD3D11::GetShaderUniformSize(const ShaderUniformInfo& inShaderUniform)
    {
        switch (inShaderUniform.mType)
        {
        case ShaderVariableType::Float:
            return sizeof(float);
            break;
        case ShaderVariableType::Int:
            return sizeof(int);
            break;
        case ShaderVariableType::Mat4x4:
            return sizeof(DirectX::XMFLOAT4X4);
            break;
        case ShaderVariableType::Vec4:
            return sizeof(DirectX::XMFLOAT4);
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
        viewport.Width = inWindow->GetWindow()->GetWidth();
        viewport.Height = inWindow->GetWindow()->GetHeight();

        GRenderDeviceD3D11->GetDeviceContext()->RSSetViewports(1, &viewport);

        return renderTarget;
    }

    RenderTarget* RenderDeviceD3D11::CreateRenderTarget(TextureInfo inTextureInfo, int numTextures)
    {
        return nullptr; // TODO
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

    ShaderProgram* RenderDeviceD3D11::CreateShaderProgram(const std::string& inShaderProgramPath)
    {
        ShaderConverter::ShaderParser shaderParser;
        ShaderConverter::ParsedShaderProgram* parsedProgram = shaderParser.ParseShaderProgram(inShaderProgramPath.c_str());
        ShaderConverter::ShaderWriterHLSL shaderWriter;
        ShaderConverter::ShaderProgramDataHLSL convertedShaderData;
        shaderWriter.WriteShader(parsedProgram, convertedShaderData);

        std::string vertexShaderCode = convertedShaderData.mVertexShader.mSource;
        std::string pixelShaderCode = convertedShaderData.mFragmentShader.mSource;

        std::vector<EVertexComponent> vertexComponents;
        const std::unordered_map<std::string, EVertexComponent> vertexComponentSemanticMap = { {"POSITION", EVertexComponent::Position},{ "NORMAL", EVertexComponent::Normal },{ "TEXCOORD", EVertexComponent::TexCoord },{ "COLOR", EVertexComponent::Colour } };
        for (const ShaderConverter::ShaderStructMember inputVar : parsedProgram->mVertexShader->mInput.mMemberVariables)
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
        delete parsedProgram;

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

        std::vector<ShaderConverter::ShaderDataHLSL> shaderDataList;
        shaderDataList.push_back(convertedShaderData.mVertexShader);
        shaderDataList.push_back(convertedShaderData.mFragmentShader);

        for (size_t iShader = 0; iShader < shaderDataList.size(); iShader++)
        {
            std::unordered_map<std::string, ShaderConstantD3D11> shaderConstantMap;

            ShaderConverter::ShaderDataHLSL& currShaderData = iShader == 0 ? convertedShaderData.mVertexShader : convertedShaderData.mFragmentShader; // TODO

            size_t currentUniformOffset = 0;
            for (const ShaderConverter::ShaderUniformInfo& parserUniformInfo : shaderDataList[iShader].mUniforms)
            {
                // TEMP - FIX THIS
                ShaderVariableType varType = parserUniformInfo.mUniformType == "float" ? ShaderVariableType::Float : (parserUniformInfo.mUniformType == "int" ? ShaderVariableType::Int : parserUniformInfo.mUniformType == "vec4" ? ShaderVariableType::Vec4 : ShaderVariableType::Mat4x4);

                ShaderUniformInfo uniformInfo(varType, parserUniformInfo.mUniformName);

                const size_t currentUniformSize = GetShaderUniformSize(uniformInfo);
                shaderConstantMap.emplace(uniformInfo.mName, ShaderConstantD3D11(currentUniformOffset, currentUniformSize));
                currentUniformOffset += currentUniformSize;
            }
            const size_t constantBufferSize = currentUniformOffset;

            ShaderD3D11* shaderStage = new ShaderD3D11();
            shaderProgram->mShaders.push_back(shaderStage);

            if (iShader == 0) // TODO
                shaderStage->mType = EShaderTypeD3D11::VertexShader;
            else
                shaderStage->mType = EShaderTypeD3D11::PixelShader;

            if (!shaderConstantMap.empty())
            {
                ID3D11Buffer*   constantBuffer = NULL;

                char* constData = new char[constantBufferSize];

                D3D11_BUFFER_DESC cbDesc;
                cbDesc.ByteWidth = constantBufferSize;
                cbDesc.Usage = D3D11_USAGE_DYNAMIC;
                cbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
                cbDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
                cbDesc.MiscFlags = 0;
                cbDesc.StructureByteStride = 0;

                D3D11_SUBRESOURCE_DATA subresData;
                subresData.pSysMem = constData;
                subresData.SysMemPitch = 0;
                subresData.SysMemSlicePitch = 0;

                HRESULT hr = mDevice->CreateBuffer(&cbDesc, &subresData, &constantBuffer);

                if (FAILED(hr))
                {
                    LOG_ERROR() << "Failed to create constant buffer";
                    return nullptr;
                }

                shaderStage->mConstantBuffer = constantBuffer;
                shaderStage->mConstantData = constData;
                shaderStage->mShaderConstantMap = shaderConstantMap;
                shaderStage->mShaderConstantsSize = constantBufferSize;
            }
        }

       
        return shaderProgram;
    }

    TextureBuffer* RenderDeviceD3D11::CreateTextureBuffer(TextureInfo inTextureInfo, void* inTextureData)
    {
        TextureBufferD3D11* textureBuffer = new TextureBufferD3D11();

        __AssertComment(inTextureInfo.mBytesPerPixel % 2 == 0, "Bytes per pixel must be a power of 2");

        const unsigned int w = inTextureInfo.mWidth;
        const unsigned int h = inTextureInfo.mHeight;

        ID3D11Texture2D *boxTex = 0;

        void* dataPtr = inTextureData;

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
        initData.pSysMem = dataPtr;
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

    void RenderDeviceD3D11::SetTexture(TextureBuffer* inTexture, int inSlot)
    {
        TextureBufferD3D11* d3dTexture = (TextureBufferD3D11*)inTexture;
        GetDeviceContext()->PSSetSamplers(inSlot, 1, &mDefaultSamplerState);
        GetDeviceContext()->PSSetShaderResources(0, 1, &d3dTexture->mTextureResourceView);
    }

    void RenderDeviceD3D11::SetActiveShaderProgram(ShaderProgram* inProgram)
    {
        ShaderProgramD3D11* dxShaderProgram = (ShaderProgramD3D11*)inProgram;
        mActiveShaderProgram = dxShaderProgram;
        mDeviceContext->VSSetShader(dxShaderProgram->mVS, 0, 0);
        mDeviceContext->PSSetShader(dxShaderProgram->mPS, 0, 0);

        for (ShaderD3D11* shader : dxShaderProgram->mShaders)
        {
            if (shader->mConstantBuffer != nullptr)
            {
                if (shader->mType == EShaderTypeD3D11::VertexShader) // TODO
                    mDeviceContext->VSSetConstantBuffers(0, 1, &shader->mConstantBuffer);
                else
                    mDeviceContext->PSSetConstantBuffers(0, 1, &shader->mConstantBuffer);
            }
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

        mDeviceContext->ClearRenderTargetView(mRenderTarget->GetBackBuffer(), D3DXCOLOR(0.0f, 0.2f, 0.4f, 1.0f));
    }

    void RenderDeviceD3D11::EndRenderTarget(RenderTarget* inTarget)
    {
        __Assert(mRenderTarget == inTarget);

        mRenderTarget->EndRendering();

        mRenderTarget = nullptr;
    }

    void RenderDeviceD3D11::RenderPrimitive(VertexBuffer* inVertexBuffer, IndexBuffer* inIndexBuffer)
    {
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

    void RenderDeviceD3D11::SetShaderUniformMat4x4(const char* inName, const glm::mat4 inMat)
    {
        __Assert(mActiveShaderProgram != nullptr);

        bool foundUniform = false;

        for (ShaderD3D11* currShader : mActiveShaderProgram->mShaders)
        {
            auto shaderConstIter = currShader->mShaderConstantMap.find(inName);
            if (shaderConstIter == currShader->mShaderConstantMap.end())
            {
                break;
            }
            foundUniform = true;

            const ShaderConstantD3D11 shaderConst = shaderConstIter->second;

            void* shaderConstData = currShader->mConstantData;

            glm::mat4 mat = glm::transpose(inMat);
            *((DirectX::XMFLOAT4X4*)((char*)shaderConstData + shaderConst.mOffset)) = DirectX::XMFLOAT4X4(&mat[0][0]);

            D3D11_MAPPED_SUBRESOURCE mappedResource;
            mDeviceContext->Map(currShader->mConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);

            memcpy(mappedResource.pData, shaderConstData, currShader->mShaderConstantsSize);

            mDeviceContext->Unmap(currShader->mConstantBuffer, 0);
        }

        if (!foundUniform)
        {
            LOG_ERROR() << "Could not find shader constant: " << inName;
        }
    }

    void RenderDeviceD3D11::SetShaderUniformVec4(const char* inName, const glm::vec4 inVec)
    {
        __Assert(mActiveShaderProgram != nullptr);

        bool foundUniform = false;

        for (ShaderD3D11* currShader : mActiveShaderProgram->mShaders)
        {
            auto shaderConstIter = currShader->mShaderConstantMap.find(inName);
            if (shaderConstIter == currShader->mShaderConstantMap.end())
            {
                break;
            }
            foundUniform = true;

            const ShaderConstantD3D11 shaderConst = shaderConstIter->second;

            void* shaderConstData = currShader->mConstantData;

            *((DirectX::XMFLOAT4*)((char*)shaderConstData + shaderConst.mOffset)) = DirectX::XMFLOAT4(&inVec[0]);

            D3D11_MAPPED_SUBRESOURCE mappedResource;
            mDeviceContext->Map(currShader->mConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);

            memcpy(mappedResource.pData, shaderConstData, currShader->mShaderConstantsSize);

            mDeviceContext->Unmap(currShader->mConstantBuffer, 0);
        }
        if (!foundUniform)
        {
            LOG_ERROR() << "Could not find shader constant: " << inName;
        }
    }

}
