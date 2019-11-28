#ifndef MING3D_PLATFORMLINUX_H
#define MING3D_PLATFORMLINUX_H

#include "platform.h"

namespace Ming3D
{
    class PlatformLinux : public Platform
    {
    public:
        PlatformLinux();
        virtual ~PlatformLinux();
        virtual void Initialise() override;
        virtual void Update() override;
        virtual RenderDevice* CreateRenderDevice() override;
        virtual WindowBase* CreateOSWindow() override;
        virtual RenderWindow* CreateRenderWindow(WindowBase* inWindow, RenderDevice* inDevice) override;
        virtual NetSocket* CreateSocket() override;
        virtual InputHandler* CreateInputHandler() override;

    };
}

#endif
