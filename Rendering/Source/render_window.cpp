#include "render_window.h"

namespace Ming3D::Rendering
{
    RenderWindow::RenderWindow(WindowBase* inWindow)
    {
        mWindow = inWindow;
    }

    RenderWindow::~RenderWindow()
    {
    }
}
