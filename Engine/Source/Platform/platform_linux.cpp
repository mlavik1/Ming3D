#include "platform_linux.h"

#include <SDL.h>
#include "sdl_window.h"
#include "render_device_gl.h"
#include "Input/input_handler_sdl.h"
#include "Debug/debug.h"
#include "GameEngine/game_engine.h"

#include "platform_file_linux.h"

namespace Ming3D
{
    PlatformLinux::PlatformLinux()
    {
        mPlatformFile = new PlatformFileLinux();
    }

    PlatformLinux::~PlatformLinux()
    {

    }

    void PlatformLinux::Initialise()
    {
        if (SDL_Init(SDL_INIT_VIDEO) != 0)
            LOG_ERROR() << "Failed to initialise SDL";
        else
        {
            SDL_version linkedver; SDL_version compiledver;
            SDL_GetVersion(&linkedver);
            SDL_VERSION(&compiledver);
            LOG_INFO() << "SDL compiled version: " << (int)compiledver.major << "." << (int)compiledver.minor << ", pathch: " << (int)compiledver.patch;
            LOG_INFO() << "SDL linked version: " << (int)linkedver.major << "." << (int)linkedver.minor << ", pathch: " << (int)linkedver.patch;
        }
    }

    void PlatformLinux::Update()
    {

    }

    RenderDevice* PlatformLinux::CreateRenderDevice()
    {
        LOG_INFO() << "Using OpenGL, version " << glGetString(GL_VERSION);
        return new RenderDeviceGL();
    }

    WindowBase* PlatformLinux::CreateOSWindow()
    {
        WindowBase* window;
        window = new SDLWindow();
        window->Initialise();
        return window;
    }

    RenderWindow* PlatformLinux::CreateRenderWindow(WindowBase* inWindow, RenderDevice* inDevice)
    {
        return inDevice->CreateRenderWindow(inWindow);
    }

    NetSocket* PlatformLinux::CreateSocket()
    {
        return nullptr;
    }

    InputHandler* PlatformLinux::CreateInputHandler()
    {
        return new InputHandlerSDL();
    }
}
