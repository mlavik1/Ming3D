#ifndef MING3D_RENDERWINDOW_D3D11_H
#define MING3D_RENDERWINDOW_D3D11_H

#include "render_window.h"

#include <DXGI.h>
#include <d3d11.h>

namespace Ming3D
{
    class RenderWindowD3D11 : public RenderWindow
    {
    private:
        IDXGISwapChain* mSwapChain = nullptr;
        ID3D11Texture2D* mBackBuffer;

    public:
        RenderWindowD3D11(WindowBase* inWindow);
        virtual ~RenderWindowD3D11();

        void SetSwapChain(IDXGISwapChain* inSwapChain);
        void SetBackBuffer(ID3D11Texture2D* inBackBuffer);

        IDXGISwapChain* GetSwapChain() { return mSwapChain; }
        ID3D11Texture2D* GetBackBuffer() { return mBackBuffer; }
    };
}

#endif
