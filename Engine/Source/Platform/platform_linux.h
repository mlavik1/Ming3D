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
        virtual Rendering::RenderDevice* CreateRenderDevice() override;
        virtual Rendering::WindowBase* CreateOSWindow() override;
        virtual Rendering::RenderWindow* CreateRenderWindow(Rendering::WindowBase* inWindow, Rendering::RenderDevice* inDevice) override;
        virtual NetSocket* CreateSocket() override;
        virtual InputHandler* CreateInputHandler() override;
        virtual std::string ReadConsoleLine() override;

    };
}

#endif
