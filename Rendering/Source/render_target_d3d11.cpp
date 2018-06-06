#include "render_target_d3d11.h"

#include "render_device_d3d11.h"
#include "Debug/st_assert.h"

namespace Ming3D
{
    RenderTargetD3D11::RenderTargetD3D11()
    {

    }

    RenderTargetD3D11::~RenderTargetD3D11()
    {
        if (mBackBuffer != nullptr)
        {
            mBackBuffer->Release();
        }
    }

    void RenderTargetD3D11::BeginRendering()
    {
        
    }

    void RenderTargetD3D11::EndRendering()
    {

    }

    TextureBuffer* RenderTargetD3D11::GetColourTextureBuffer(int inSlot)
    {
        // TODO
        return nullptr;
    }

}
