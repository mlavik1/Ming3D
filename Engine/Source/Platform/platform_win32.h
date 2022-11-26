#ifndef MING3D_PLATFORMWIN32_H
#define MING3D_PLATFORMWIN32_H

#include "platform.h"

namespace Ming3D
{
    class PlatformWin32 : public Platform
    {
    public:
        PlatformWin32();
        virtual ~PlatformWin32();
        virtual void Initialise() override;
        virtual void Update() override;
        virtual Rendering::RenderDevice* CreateRenderDevice() override;
        virtual Rendering::WindowBase* CreateOSWindow() override;
        virtual Rendering::RenderWindow* CreateRenderWindow(Rendering::WindowBase* inWindow, Rendering::RenderDevice* inDevice) override;
        virtual NetSocket* CreateSocket() override;
        virtual InputHandler* CreateInputHandler(Rendering::WindowBase* window) override;
        virtual std::string ReadConsoleLine() override;

    };
}

#endif
