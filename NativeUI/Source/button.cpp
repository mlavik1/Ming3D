#include "button.h"
#include "text_utils.h"
#include <WinUser.h>

namespace NativeUI
{
    std::unordered_map<HWND, Button*> Button::ButtonInstances;

	Button::Button(Control* arg_parent)
		: Control(arg_parent)
	{
        mHwnd = CreateWindow(L"BUTTON", L"", WS_CHILD | WS_VISIBLE /*| BS_BITMAP*/, 0, 0, 50, 50, arg_parent->GetHwnd(), NULL, GetModuleHandle(NULL), NULL);
		//mHwnd = CreateWindow(L"NativeUIButton", L"", WS_CHILD | WS_VISIBLE, 0, 0, 50, 50, arg_parent->GetHwnd(), NULL, GetModuleHandle(NULL), NULL);
#ifdef _WIN64
        mOrigWndProc = (WNDPROC)SetWindowLongPtr(mHwnd, GWLP_WNDPROC, (LONG)WndProc);
#else
        mOrigWndProc = (WNDPROC)SetWindowLong(mHwnd, GWL_WNDPROC, (LONG)WndProc);
#endif
        //mWinButtonHWND = CreateWindow(L"BUTTON", L"", WS_CHILD | WS_VISIBLE /*| BS_BITMAP*/, 0, 0, 50, 50, mHwnd, (HMENU)mWinButtonUID, GetModuleHandle(NULL), NULL);
        SetSize(Point(80.0f, 40.0f));
		ShowWindow(mHwnd, TRUE);
		UpdateWindow(mHwnd);

        ButtonInstances[mHwnd] = this;
        //mButtonInstances[mWinButtonHWND] = this; // TODO!!
	}

    Button::~Button()
    {
        ButtonInstances.erase(mHwnd);
    }

    LRESULT CALLBACK Button::WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
    {
        auto buttonIter = ButtonInstances.find(hwnd);
        if (buttonIter == ButtonInstances.end())
        {
            return DefWindowProc(hwnd, msg, wParam, lParam);
        }
        Button* button = buttonIter->second;

        static PAINTSTRUCT ps;

        switch (msg)
        {
            case WM_LBUTTONUP:
            {
            for (auto callback : button->mClickCallbacks)
            {
                callback();
            }
            break;
            }
        }
        return CallWindowProc(button->mOrigWndProc, hwnd, msg, wParam, lParam);
    }

	void Button::SetText(const char* arg_text)
	{
        std::wstring wstr = TextUtils::UTF8ToWChar(arg_text);
        SetWindowText(mHwnd, wstr.c_str());
        //SendMessage(mHwnd, WM_SETTEXT, 0, (LPARAM)arg_text);
		//UpdateWindow(mHwnd);
	}

    void Button::SetImage(const char* arg_image)
    {
        HINSTANCE hInstance = GetModuleHandle(NULL);
        //HBITMAP hBitmap = LoadBitmap(hInstance, "wall1");
        HBITMAP hBitmap = (HBITMAP)LoadImage(NULL, (LPCWSTR)arg_image, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
        if (hBitmap == NULL)
            MessageBox(NULL, L"Failed to set button image!", L"Error!", MB_ICONEXCLAMATION | MB_OK);
        else
            SendMessage(mHwnd, BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)hBitmap);
    }

    void Button::RegisterClickCallback(std::function<void()> inCallback)
    {
        mClickCallbacks.push_back(inCallback);
    }
}
