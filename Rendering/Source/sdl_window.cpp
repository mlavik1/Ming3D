#include "sdl_window.h"

// TODO: change include path
#ifdef _WIN32
#include <windows.h>
#include <SDL.h>
#include <SDL_video.h>
#include <SDL_syswm.h>
#else
#include <SDL2/SDL.h>
#include <SDL2/SDL_video.h>
#include <SDL2/SDL_syswm.h>
#endif

#include "Debug/st_assert.h"

namespace Ming3D::Rendering
{
    SDL_GLContext SDLWindow::GSharedGLContext = nullptr;

    SDLWindow::SDLWindow()
    {

    }

    SDLWindow::SDLWindow(const void* inNativewindow)
    {
        mSDLWindow = SDL_CreateWindowFrom(inNativewindow);
        __Assert(mSDLWindow != nullptr);
    }

    SDLWindow::~SDLWindow()
    {
        if (mSDLWindow != nullptr)
        {
            SDL_DestroyWindow(mSDLWindow);
            mSDLWindow = nullptr;
        }
    }

    void SDLWindow::Initialise()
    {
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

        if(mSDLWindow == nullptr)
            mSDLWindow = SDL_CreateWindow("Ming3D", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, mWindowWidth, mWindowHeight, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);

        // Re-use shared GL context (from previously created windows)
        // We only want to use one context for all windows, since we can't share data between contexts.
        if (GSharedGLContext == nullptr)
        {
            mGLContext = SDL_GL_CreateContext(mSDLWindow);
            GSharedGLContext = mGLContext;
        }
        else
        {
            mGLContext = GSharedGLContext;
            SDL_GL_MakeCurrent(mSDLWindow, mGLContext);
        }

        SDL_GL_SetSwapInterval(1); // vsync
    }

    void SDLWindow::SetSize(unsigned int inWidth, unsigned int inHeight)
    {
        mWindowWidth = inWidth;
        mWindowHeight = inHeight;

        if(mSDLWindow != nullptr)
            SDL_SetWindowSize(mSDLWindow, mWindowWidth, mWindowHeight);
    }

    void SDLWindow::BeginRender()
    {
        SDL_UpdateWindowSurface(mSDLWindow);
        SDL_GL_MakeCurrent(mSDLWindow, mGLContext);
    }

    void SDLWindow::EndRender()
    {
        SDL_GL_SwapWindow(mSDLWindow);
    }

    void* SDLWindow::GetOSWindowHandle()
    {
#ifdef _WIN32
        SDL_SysWMinfo wmInfo;
        SDL_VERSION(&wmInfo.version);
        SDL_GetWindowWMInfo(mSDLWindow, &wmInfo);
        return wmInfo.info.win.window;
#else
        return nullptr; // TODO
#endif
    }

    void SDLWindow::Close()
    {
        SDL_DestroyWindow(mSDLWindow);
        mSDLWindow = nullptr;
    }

    bool SDLWindow::IsOpen()
    {
        return mSDLWindow != nullptr;
    }
}
