#include "platform_win32.h"

#define MING3D_USE_OPENGL

#ifdef MING3D_USE_OPENGL
#include <SDL.h>
#include "sdl_window.h"
#include "render_device_gl.h"
#else
#include <Windows.h>
#include "render_device_d3d11.h"
#include "winapi_window.h"
#endif

#include "Debug/debug.h"

namespace Ming3D
{
    void PlatformWin32::Initialise()
    {
#ifdef MING3D_USE_OPENGL
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
#endif
    }

    RenderDevice* PlatformWin32::CreateRenderDevice()
    {
#ifdef MING3D_USE_OPENGL
        LOG_INFO() << "Using OpenGL, version " << glGetString(GL_VERSION);
        return new RenderDeviceGL();
#else
        return new RenderDeviceD3D11();
#endif
    }

    WindowBase* PlatformWin32::CreateOSWindow()
    {
        WindowBase* window;
#ifdef MING3D_USE_OPENGL
        window = new SDLWindow();
#else
        window = new WinAPIWindow();
#endif
        window->Initialise();
        return window;
    }

    RenderWindow* PlatformWin32::CreateRenderWindow(WindowBase* inWindow, RenderDevice* inDevice)
    {
        return inDevice->CreateRenderWindow(inWindow);
    }
}
