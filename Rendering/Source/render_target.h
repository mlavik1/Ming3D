#ifndef MING3D_RENDER_TARGET_H
#define MING3D_RENDER_TARGET_H

#include "window_base.h"

namespace Ming3D
{
    class RenderTarget
    {
    protected:
        WindowBase* mWindow;

    public:
        RenderTarget(WindowBase* inWindow);
        WindowBase* GetWindow() { return mWindow; }

        virtual void BeginRendering() = 0;
        virtual void EndRendering() = 0;
    };
}
#endif
