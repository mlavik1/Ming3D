#ifdef MING3D_OPENGL
#include "render_target_gl.h"

namespace Ming3D::Rendering
{
    RenderTargetGL::RenderTargetGL()
    {

    }

    RenderTargetGL::~RenderTargetGL()
    {
        for (auto colBuffer : mColourBuffers)
        {
            delete colBuffer;
        }
        delete mDepthRenderBuffer;
    }

    TextureBuffer* RenderTargetGL::GetColourTextureBuffer(int inSlot)
    {
        return mColourBuffers[inSlot];
    }

    TextureBuffer* RenderTargetGL::GetDepthTextureBuffer()
    {
        return mDepthRenderBuffer;
    }

    RenderWindow* RenderTargetGL::GetRenderWindow()
    {
        return mRenderWindow;
    }
}
#endif
