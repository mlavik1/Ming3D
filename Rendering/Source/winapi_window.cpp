#ifdef _WIN32
#include "winapi_window.h"
#include <cassert>

#define MING3D_WINDOW_STYLE WS_OVERLAPPEDWINDOW

namespace Ming3D::Rendering
{
    bool WinAPIWindow::ClassInitialised = false;
    std::unordered_map<HWND, WinAPIWindow*> WinAPIWindow::WindowInstances;

    WinAPIWindow::WinAPIWindow()
    {

    }

    WinAPIWindow::WinAPIWindow(HWND inHWND)
    {
        mHWND = inHWND;
    }

    LRESULT CALLBACK WinAPIWindow::WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
    {
        switch (message)
        {
        case WM_DESTROY:
        {
            PostQuitMessage(0);
            return 0;
        } break;
        }

        auto wndIter = WinAPIWindow::WindowInstances.find(hWnd);
        if (wndIter != WinAPIWindow::WindowInstances.end() && wndIter->second->mWndProcCallback)
            return wndIter->second->mWndProcCallback(hWnd, message, wParam, lParam);

        return DefWindowProc(hWnd, message, wParam, lParam);
    }

    void WinAPIWindow::Initialise()
    {
        if (mHWND != nullptr)
            return;

        HINSTANCE hInstance = GetModuleHandle(NULL);

        if (!ClassInitialised)
        {
            WNDCLASSEX wc;
            ZeroMemory(&wc, sizeof(WNDCLASSEX));
            wc.cbSize = sizeof(WNDCLASSEX);
            wc.style = CS_HREDRAW | CS_VREDRAW;
            wc.lpfnWndProc = WindowProc;
            wc.hInstance = hInstance;
            wc.hCursor = LoadCursor(NULL, IDC_ARROW);
            wc.lpszClassName = L"WinAPIWindowClass";

            RegisterClassEx(&wc);
            ClassInitialised = true;
        }

        RECT wr = { 0, 0, mWindowWidth, mWindowHeight };
        AdjustWindowRect(&wr, MING3D_WINDOW_STYLE, FALSE);

        mHWND = CreateWindowEx(NULL,
            L"WinAPIWindowClass",
            L"Ming3D",
            MING3D_WINDOW_STYLE,
            100,
            100,
            wr.right - wr.left,
            wr.bottom - wr.top,
            NULL,
            NULL,
            hInstance,
            NULL);

        ShowWindow(mHWND, SW_SHOW);

        WindowInstances.emplace(mHWND, this);
    }

    void WinAPIWindow::SetSize(unsigned int inWidth, unsigned int inHeight)
    {
        mWindowWidth = inWidth;
        mWindowHeight = inHeight;

        RECT oldRect;
        assert(GetWindowRect(mHWND, &oldRect));

        RECT wndRect = { 0, 0, mWindowWidth, mWindowHeight };
        assert(AdjustWindowRect(&wndRect, MING3D_WINDOW_STYLE, FALSE));

        wndRect.right += (oldRect.left - wndRect.left);
        wndRect.left = oldRect.left;
        wndRect.bottom += (oldRect.top - wndRect.top);
        wndRect.top = oldRect.top;

		SetWindowPos(mHWND, NULL, wndRect.left, wndRect.top, wndRect.right - wndRect.left, wndRect.bottom - wndRect.top, 0);
    }

    void WinAPIWindow::BeginRender()
    {

    }

    void WinAPIWindow::EndRender()
    {

    }

    void* WinAPIWindow::GetOSWindowHandle()
    {
        return mHWND;
    }

    void WinAPIWindow::Close()
    {
        // TODO
    }

    bool WinAPIWindow::IsOpen()
    {
        return true; // TODO
    }
}
#endif

