#ifndef MING3D_PLATFORMWIN32_H
#define MING3D_PLATFORMWIN32_H

#include "platform.h"

namespace Ming3D
{
    class PlatformWin32 : public Platform
    {
    public:
        PlatformWin32();
        ~PlatformWin32();
        virtual void Initialise() override;
        virtual void Update() override;
        virtual RenderDevice* CreateRenderDevice() override;
        virtual WindowBase* CreateOSWindow() override;
        virtual RenderWindow* CreateRenderWindow(WindowBase* inWindow, RenderDevice* inDevice) override;
        virtual NetSocket* CreateSocket() override;
        virtual InputHandler* CreateInputHandler() override;
        virtual std::string ReadConsoleLine() override;

    };
}

#endif
