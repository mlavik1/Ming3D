#ifdef MING3D_OPENGL
#include "render_target_gl.h"

namespace Ming3D
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
    }

    void RenderTargetGL::BeginRendering()
    {
    }

    void RenderTargetGL::EndRendering()
    {
    }

    TextureBuffer* RenderTargetGL::GetColourTextureBuffer(int inSlot)
    {
        return mColourBuffers[inSlot];
    }
}
#endif
