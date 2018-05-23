#ifndef MING3D_SDL_WINDOW_H
#define MING3D_SDL_WINDOW_H

#include "window_base.h"
#include <SDL.h>
#include <SDL_video.h>

namespace Ming3D
{
    class SDLWindow : public WindowBase
    {
    private:
        SDL_Window* mSDLWindow;
        SDL_GLContext mGLContext;
        unsigned int mWindowWidth = 800;
        unsigned int mWindowHeight = 600;

    public:
        virtual void Initialise() override;
        virtual void SetSize(unsigned int inWidth, unsigned int inHeight) override;
        virtual unsigned int GetWidth() override { return mWindowWidth; };
        virtual unsigned int GetHeight() override { return mWindowHeight; };
        virtual void BeginRender() override;
        virtual void EndRender() override;
        virtual void* GetOSWindowHandle() override;
        SDL_Window* GetSDLWindow() { return mSDLWindow; }
        SDL_GLContext GetGLContext() { return mGLContext; }
    };
}
#endif
