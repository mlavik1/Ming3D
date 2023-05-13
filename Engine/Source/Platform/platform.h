#pragma once

#include "Input/input_handler.h"
#include "platform_interface.h"
#include <string>
#include <memory>

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

        virtual void Initialise() = 0;
        virtual void Update() = 0;

        virtual Rendering::RenderDevice* CreateRenderDevice() = 0;
        virtual Rendering::WindowBase* CreateOSWindow() = 0;
        virtual Rendering::RenderWindow* CreateRenderWindow(Rendering::WindowBase* inWindow, Rendering::RenderDevice* inDevice) = 0;
        virtual NetSocket* CreateSocket() = 0;
        virtual InputHandler* CreateInputHandler(Rendering::WindowBase* window) = 0;
        virtual std::string ReadConsoleLine() = 0;

        inline PlatformInterface* GetPlatformInterface() { return mPlatformInterface.get(); }
        
    protected:
        std::unique_ptr<PlatformInterface> mPlatformInterface = nullptr;
    };
}
