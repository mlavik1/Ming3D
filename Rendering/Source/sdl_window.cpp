#include "sdl_window.h"

#include <SDL.h>
#include <SDL_video.h>

#include <SDL_syswm.h>

#include "Debug/st_assert.h"

namespace Ming3D
{
    SDLWindow::SDLWindow()
    {
        
    }

    SDLWindow::SDLWindow(const void* inNativewindow)
    {
        mSDLWindow = SDL_CreateWindowFrom(inNativewindow);
        __Assert(mSDLWindow != nullptr);
    }

    void SDLWindow::Initialise()
    {
        if(mSDLWindow == nullptr)
            mSDLWindow = SDL_CreateWindow("Ming3D", 0, 0, mWindowWidth, mWindowHeight, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);
        mGLContext = SDL_GL_CreateContext(mSDLWindow);
    }

    void SDLWindow::SetSize(unsigned int inWidth, unsigned int inHeight)
    {
        mWindowHeight = inWidth;
        mWindowHeight = inHeight;
    }

    void SDLWindow::BeginRender()
    {

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
}
