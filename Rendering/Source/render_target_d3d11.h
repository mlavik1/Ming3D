#ifndef MING3D_RENDERTARGET_D3D11_H
#define MING3D_RENDERTARGET_D3D11_H

#include "render_target.h"
#include "window_base.h"
#include "depth_stencil_view_d3d11.h"

#include <Windows.h>
#include <windowsx.h>
#include <d3d11.h>
#include <DXGI.h>
#include <vector>

namespace Ming3D::Rendering
{
    class RenderTargetD3D11 : public RenderTarget
    {
        friend class RenderDeviceD3D11; // TODO

    private:
        ID3D11RenderTargetView* mBackBuffer;
        std::vector<TextureBuffer*> mColourBuffers;
        DepthStencilViewD3D11* mDepthStencilView;

    public:
        RenderTargetD3D11();
        ~RenderTargetD3D11();
        
        ID3D11RenderTargetView* GetBackBuffer() { return mBackBuffer; }

        virtual void BeginRendering() override;
        virtual void EndRendering() override;

        virtual TextureBuffer* GetColourTextureBuffer(int inSlot) override;
        virtual TextureBuffer* GetDepthTextureBuffer() override;
    };
}

#endif
