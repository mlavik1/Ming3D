#include "platform_win32.h"

#include <winsock2.h>
#include <windows.h>
#include <ws2tcpip.h>

#ifdef MING3D_FORCE_OPENGL
#include <SDL.h>
#include "sdl_window.h"
#include "render_device_gl.h"
#include "Input/input_handler_sdl.h"
#else
#include "render_device_d3d11.h"
#include "winapi_window.h"
#endif
#include "Input/input_handler_win32.h"

#include "Debug/debug.h"

#pragma comment (lib, "Ws2_32.lib") // REMOVE ME

#include "net_socket_winsock.h"
#include "GameEngine/game_engine.h"

namespace Ming3D
{
    PlatformWin32::~PlatformWin32()
    {
        WSACleanup();
    }

    void PlatformWin32::Initialise()
    {
#ifdef MING3D_FORCE_OPENGL
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

        WSADATA wsaData;
        int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
        if (iResult != 0)
        {
            LOG_ERROR() << "WSAStartup failed";
            return;
        }
    }

    void PlatformWin32::Update()
    {
        MSG msg;
        msg.message = ~WM_QUIT;
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);

        }
        if (msg.message == WM_QUIT)
        {
            //GGameEngine->Shutdown();
        }
    }

    RenderDevice* PlatformWin32::CreateRenderDevice()
    {
#ifdef MING3D_FORCE_OPENGL
        LOG_INFO() << "Using OpenGL, version " << glGetString(GL_VERSION);
        return new RenderDeviceGL();
#else
        return new RenderDeviceD3D11();
#endif
    }

    WindowBase* PlatformWin32::CreateOSWindow()
    {
        WindowBase* window;
#ifdef MING3D_FORCE_OPENGL
        window = new SDLWindow();
#else
        auto wndProcCallback = [](HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) -> LRESULT
        {
            InputHandlerWin32* inputHandler = (InputHandlerWin32*)GGameEngine->GetInputHandler();
            return inputHandler->HandleWindowProc(hWnd, message, wParam, lParam);
        };

        window = new WinAPIWindow();
        ((WinAPIWindow*)window)->mWndProcCallback = wndProcCallback;
#endif
        window->Initialise();
        return window;
    }

    RenderWindow* PlatformWin32::CreateRenderWindow(WindowBase* inWindow, RenderDevice* inDevice)
    {
        return inDevice->CreateRenderWindow(inWindow);
    }

    NetSocket* PlatformWin32::CreateSocket()
    {
        return new NetSocketWinsock();
    }

    InputHandler* PlatformWin32::CreateInputHandler()
    {
#if MING3D_FORCE_OPENGL
        return new InputHandlerSDL();
#else
        return new InputHandlerWin32();
#endif
    }
}
