#ifndef MING3D_RENDERTARGET_D3D11_H
#define MING3D_RENDERTARGET_D3D11_H

#include "render_target.h"
#include "window_base.h"

#include <Windows.h>
#include <windowsx.h>
#include <d3d11.h>
#include <d3dx11.h>
#include <d3dx10.h>
#include <DXGI.h>

namespace Ming3D
{
    class RenderTargetD3D11 : public RenderTarget
    {
        friend class RenderDeviceD3D11; // TODO

    private:
        ID3D11RenderTargetView* mBackBuffer;

    public:
        RenderTargetD3D11();
        ~RenderTargetD3D11();
        
        ID3D11RenderTargetView* GetBackBuffer() { return mBackBuffer; }

        virtual void BeginRendering() override;
        virtual void EndRendering() override;

        virtual TextureBuffer* GetColourTextureBuffer(int inSlot) override;
    };
}

#endif
