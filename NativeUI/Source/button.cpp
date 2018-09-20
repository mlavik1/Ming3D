#include "button.h"
#include "text_utils.h"

namespace NativeUI
{
	Button::Button(Control* arg_parent)
		: Control(arg_parent)
	{
		mHwnd = CreateWindow(L"BUTTON", L"", WS_CHILD | WS_VISIBLE /*| BS_BITMAP*/, 0, 0, 50, 50, arg_parent->GetHwnd(), NULL, GetModuleHandle(NULL), NULL);
		SetSize(Point(80.0f, 40.0f));
		ShowWindow(mHwnd, TRUE);
		UpdateWindow(mHwnd);
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
}
