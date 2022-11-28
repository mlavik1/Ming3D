#ifndef MING3D_SDL_WINDOW_H
#define MING3D_SDL_WINDOW_H

#include "window_base.h"

struct SDL_Window;
typedef void *SDL_GLContext;

namespace Ming3D::Rendering
{
    class SDLWindow : public WindowBase
    {
    private:
        SDL_Window* mSDLWindow = nullptr;
        SDL_GLContext mGLContext = nullptr;
        unsigned int mWindowWidth = 800;
        unsigned int mWindowHeight = 600;

        static SDL_GLContext GSharedGLContext; // Shared GL context between all windows (TODO: remove this hack!)

    public:
        SDLWindow();
        SDLWindow(const void* inNativewindow);
        virtual ~SDLWindow();

        virtual void Initialise() override;
        virtual void SetSize(unsigned int inWidth, unsigned int inHeight) override;
        virtual unsigned int GetWidth() const override { return mWindowWidth; };
        virtual unsigned int GetHeight() const override { return mWindowHeight; };
        virtual void BeginRender() override;
        virtual void EndRender() override;
        virtual void* GetOSWindowHandle() override;
        virtual void Close() override;
        virtual bool IsOpen() override;
        SDL_Window* GetSDLWindow() { return mSDLWindow; }
        SDL_GLContext GetGLContext() { return mGLContext; }
    };
}
#endif
