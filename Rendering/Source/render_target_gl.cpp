#include "render_target_gl.h"

namespace Ming3D
{
    RenderTargetGL::RenderTargetGL(WindowBase* inWindow)
        : RenderTarget(inWindow)
    {

    }

    void RenderTargetGL::BeginRendering()
    {
        mWindow->BeginRender();
    }

    void RenderTargetGL::EndRendering()
    {
        mWindow->EndRender();
    }
}
