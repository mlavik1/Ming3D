#ifndef MING3D_PLATFORM_H
#define MING3D_PLATFORM_H

#include "Input/input_handler.h"
#include "platform_file.h"
#include <string>

namespace Ming3D
{
    class NetSocket;

    namespace Rendering
    {
        class RenderDevice;
        class WindowBase;
        class RenderWindow;
    }

    class Platform
    {
    public:
        virtual ~Platform() = default;

        PlatformFile * mPlatformFile = nullptr;

        virtual void Initialise() = 0;
        virtual void Update() = 0;

        virtual Rendering::RenderDevice* CreateRenderDevice() = 0;
        virtual Rendering::WindowBase* CreateOSWindow() = 0;
        virtual Rendering::RenderWindow* CreateRenderWindow(Rendering::WindowBase* inWindow, Rendering::RenderDevice* inDevice) = 0;
        virtual NetSocket* CreateSocket() = 0;
        virtual InputHandler* CreateInputHandler(Rendering::WindowBase* window) = 0;
        virtual std::string ReadConsoleLine() = 0;
        
    };
}

#endif
