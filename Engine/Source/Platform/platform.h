#ifndef MING3D_PLATFORM_H
#define MING3D_PLATFORM_H

#include "render_device.h"
#include "window_base.h"
#include "render_window.h"

#include "net_socket.h"

namespace Ming3D
{
    class Platform
    {
    public:
        virtual void Initialise() = 0;

        virtual RenderDevice* CreateRenderDevice() = 0;
        virtual WindowBase* CreateOSWindow() = 0;
        virtual RenderWindow* CreateRenderWindow(WindowBase* inWindow, RenderDevice* inDevice) = 0;
        virtual NetSocket* CreateSocket() = 0;
    };
}

#endif
