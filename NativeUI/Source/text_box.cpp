#include "text_box.h"
#include "text_utils.h"

namespace NativeUI
{
	TextBox::TextBox(Control* arg_parent)
		: Control(arg_parent)
	{
		mHwnd = CreateWindow(L"EDIT", L"Edit box", WS_CHILD | WS_VISIBLE, 0, 0, 50, 50, arg_parent->GetHwnd(), NULL, GetModuleHandle(NULL), NULL);
		SetSize(Point(80.0f, 40.0f));
		ShowWindow(mHwnd, TRUE);
		UpdateWindow(mHwnd);
	}

    void TextBox::SetText(const char* arg_text)
    {
        std::wstring wstr = TextUtils::UTF8ToWChar(arg_text);
        SetWindowText(mHwnd, wstr.c_str());
    }

    void TextBox::SetFontSize(int arg_size)
    {
        HFONT font = CreateFont(arg_size, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, L"Arial");
        SendMessage(mHwnd, WM_SETFONT, (WPARAM)font, TRUE);
    }

}
