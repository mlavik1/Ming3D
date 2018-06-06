#ifndef MING3D_RENDERWINDOW_GL_H
#define MING3D_RENDERWINDOW_GL_H

#include "render_window.h"

namespace Ming3D
{
    class RenderWindowGL : public RenderWindow
    {
    public:
        RenderWindowGL(WindowBase* inWindow);
        virtual ~RenderWindowGL();
    };
}

#endif
