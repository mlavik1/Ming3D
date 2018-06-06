#ifndef MING3D_RENDERWINDOW_H
#define MING3D_RENDERWINDOW_H

#include "window_base.h"

namespace Ming3D
{
    class RenderWindow
    {
    private:
        WindowBase* mWindow;

    public:
        RenderWindow(WindowBase* inWindow);
        virtual ~RenderWindow() {}

        WindowBase* GetWindow() { return mWindow; }
    };
}

#endif
