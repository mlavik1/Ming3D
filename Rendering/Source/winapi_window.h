#ifndef MING3D_WINAPI_WINDOW_H
#define MING3D_WINAPI_WINDOW_H

#include "window_base.h"
#include <Windows.h>

namespace Ming3D
{
    class WinAPIWindow : public WindowBase
    {
    private:
        HWND mHWND = nullptr;
        unsigned int mWindowWidth = 800;
        unsigned int mWindowHeight = 600;

    public:
        WinAPIWindow();
        WinAPIWindow(HWND inHWND);
        virtual void Initialise() override;
        virtual void SetSize(unsigned int inWidth, unsigned int inHeight) override;
        virtual unsigned int GetWidth() override { return mWindowWidth; };
        virtual unsigned int GetHeight() override { return mWindowHeight; };
        virtual void BeginRender() override;
        virtual void EndRender() override;
        virtual void* GetOSWindowHandle() override;
    };
}
#endif
