#include "render_window_d3d11.h"

namespace Ming3D
{
    RenderWindowD3D11::RenderWindowD3D11(WindowBase* inWindow)
        : RenderWindow(inWindow)
    {

    }

    RenderWindowD3D11::~RenderWindowD3D11()
    {
        mBackBuffer->Release();

        if (mSwapChain != nullptr)
        {
            mSwapChain->SetFullscreenState(FALSE, NULL);
            mSwapChain->Release();

        }
    }

    void RenderWindowD3D11::SetSwapChain(IDXGISwapChain* inSwapChain)
    {
        mSwapChain = inSwapChain;
    }

    void RenderWindowD3D11::SetBackBuffer(ID3D11Texture2D* inBackBuffer)
    {
        mBackBuffer = inBackBuffer;
    }
}
