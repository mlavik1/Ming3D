#include "render_target_d3d11.h"

#include "render_device_d3d11.h"
#include "Debug/st_assert.h"

namespace Ming3D
{
    RenderTargetD3D11::RenderTargetD3D11(WindowBase* inWindow)
        : RenderTarget(inWindow)
    {
        __Assert(inWindow->GetOSWindowHandle());
        __Assert(GRenderDeviceD3D11->GetDXGIFactory());

        DXGI_SWAP_CHAIN_DESC scd;
        ZeroMemory(&scd, sizeof(DXGI_SWAP_CHAIN_DESC));

        scd.BufferCount = 1;
        scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; 
        scd.BufferDesc.Width = inWindow->GetWidth();
        scd.BufferDesc.Height = inWindow->GetHeight();
        scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        scd.OutputWindow = (HWND)inWindow->GetOSWindowHandle();
        scd.SampleDesc.Count = 4;
        scd.Windowed = TRUE;
        scd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

        GRenderDeviceD3D11->GetDXGIFactory()->CreateSwapChain(GRenderDeviceD3D11->GetDevice(), &scd, &mSwapChain);

        ID3D11Texture2D *pBackBuffer;
        mSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);

        // create render target from back buffer address
        GRenderDeviceD3D11->GetDevice()->CreateRenderTargetView(pBackBuffer, NULL, &mBackBuffer);
        pBackBuffer->Release();

        // set the render target as the back buffer
        GRenderDeviceD3D11->GetDeviceContext()->OMSetRenderTargets(1, &mBackBuffer, NULL);




        // ****** TEMP ******
        // TODO: The below code should not be here

        ID3D11RasterizerState* rasterState;
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
        GRenderDeviceD3D11->GetDevice()->CreateRasterizerState(&rastDesc, &rasterState);
        GRenderDeviceD3D11->GetDeviceContext()->RSSetState(rasterState);

        ID3D11DepthStencilState* depthStencilState;
        D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
        // Initialize the description of the stencil state.
        ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));

        // Set up the description of the stencil state.
        depthStencilDesc.DepthEnable = true;
        depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
        depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

        depthStencilDesc.StencilEnable = true;
        depthStencilDesc.StencilReadMask = 0xFF;
        depthStencilDesc.StencilWriteMask = 0xFF;

        // Stencil operations if pixel is front-facing.
        depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
        depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
        depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
        depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

        // Stencil operations if pixel is back-facing.
        depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
        depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
        depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
        depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

        // Create the depth stencil state.
        HRESULT result = GRenderDeviceD3D11->GetDevice()->CreateDepthStencilState(&depthStencilDesc, &depthStencilState);
        if (FAILED(result))
        {
            return;
        }
        // Set the depth stencil state.
        GRenderDeviceD3D11->GetDeviceContext()->OMSetDepthStencilState(depthStencilState, 1);


        // Set the viewport
        D3D11_VIEWPORT viewport;
        ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));

        viewport.TopLeftX = 0;
        viewport.TopLeftY = 0;
        viewport.Width = inWindow->GetWidth();
        viewport.Height = inWindow->GetHeight();

        GRenderDeviceD3D11->GetDeviceContext()->RSSetViewports(1, &viewport);
    }

    RenderTargetD3D11::~RenderTargetD3D11()
    {
        mSwapChain->SetFullscreenState(FALSE, NULL);
        mBackBuffer->Release();
        mSwapChain->Release();
    }

    void RenderTargetD3D11::BeginRendering()
    {
        
    }

    void RenderTargetD3D11::EndRendering()
    {

    }
}
