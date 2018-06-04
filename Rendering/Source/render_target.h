#ifndef MING3D_RENDER_TARGET_H
#define MING3D_RENDER_TARGET_H

#include "window_base.h"

namespace Ming3D
{
    /**
    * RenderTarget base class.
    * The RenderDevice will render to this.
    */
    class RenderTarget
    {
    protected:
        WindowBase* mWindow; // TODO: Need to support other types of render targets

    public:
        RenderTarget(WindowBase* inWindow);
        WindowBase* GetWindow() { return mWindow; }

        virtual void BeginRendering() = 0;
        virtual void EndRendering() = 0;
    };
}
#endif
