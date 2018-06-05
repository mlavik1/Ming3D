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
