#ifdef _WIN32
#include "platform_win32.h"

#include <winsock2.h>
#include <windows.h>
#include <ws2tcpip.h>

#include "render_device_factory.h"

#ifdef MING3D_OPENGL
#include <SDL.h>
#include "sdl_window.h"
#include "Input/input_handler_sdl.h"
#else
#include "winapi_window.h"
#endif
#include "Input/input_handler_win32.h"

#include "Debug/debug.h"

#pragma comment (lib, "Ws2_32.lib") // REMOVE ME

#include "net_socket_winsock.h"
#include "GameEngine/game_engine.h"

#include "platform_file_win32.h"
#include <string>

namespace Ming3D
{
    PlatformWin32::PlatformWin32()
    {
        mPlatformFile = new PlatformFileWin32();
    }

    PlatformWin32::~PlatformWin32()
    {
        WSACleanup();
    }

    void PlatformWin32::Initialise()
    {
#ifdef MING3D_OPENGL
        if (SDL_Init(SDL_INIT_VIDEO) != 0)
            LOG_ERROR() << "Failed to initialise SDL";
        else
        {
            SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
            //SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
            //SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);

            SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
            SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
            SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
            SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
            SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
            SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, 32);
            SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
            SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 0);

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

    Rendering::RenderDevice* PlatformWin32::CreateRenderDevice()
    {
        return Rendering::RenderDeviceFactory::CreateRenderDevice();
    }

    Rendering::WindowBase* PlatformWin32::CreateOSWindow()
    {
        Rendering::WindowBase* window;
#ifdef MING3D_OPENGL
        window = new Rendering::SDLWindow();
#else
        auto wndProcCallback = [](HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) -> LRESULT
        {
            InputHandlerWin32* inputHandler = (InputHandlerWin32*)GGameEngine->GetInputHandler();
            return inputHandler->HandleWindowProc(hWnd, message, wParam, lParam);
        };

        window = new Rendering::WinAPIWindow();
        ((Rendering::WinAPIWindow*)window)->mWndProcCallback = wndProcCallback;
#endif
        window->Initialise();
        return window;
    }

    Rendering::RenderWindow* PlatformWin32::CreateRenderWindow(Rendering::WindowBase* inWindow, Rendering::RenderDevice* inDevice)
    {
        return inDevice->CreateRenderWindow(inWindow);
    }

    NetSocket* PlatformWin32::CreateSocket()
    {
        return new NetSocketWinsock();
    }

    InputHandler* PlatformWin32::CreateInputHandler(Rendering::WindowBase* window)
    {
#if MING3D_OPENGL
        return new InputHandlerSDL();
#else
        return new InputHandlerWin32();
#endif
    }
    
    std::string PlatformWin32::ReadConsoleLine()
    {
        std::string terminalString;
        std::getline(std::cin, terminalString);
        return std::string(terminalString);
    }
}
#endif
