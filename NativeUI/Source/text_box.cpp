#include "text_box.h"
#include "text_utils.h"

namespace NativeUI
{
    std::unordered_map<void*, TextBox*> TextBox::TextBoxInstances;
    WNDCLASSEX* TextBox::ControlClass = NULL;

	TextBox::TextBox(Control* arg_parent)
		: Control(arg_parent)
	{
        if (ControlClass == NULL)
            CreateControlClass();
        mHwnd = CreateWindow(L"NativeUITextBoxHolder", L"", WS_CHILD | WS_VISIBLE, 0, 0, 50, 50, arg_parent->GetHwnd(), NULL, GetModuleHandle(NULL), NULL);
        TextBoxInstances[mHwnd] = this;
        mEditHWND = CreateWindow(L"EDIT", L"Edit box", WS_CHILD | WS_VISIBLE, 0, 0, 50, 50, mHwnd, NULL, GetModuleHandle(NULL), NULL);
		SetSize(Point(80.0f, 40.0f));
		ShowWindow(mHwnd, TRUE);
		UpdateWindow(mHwnd);
	}

    void TextBox::CreateControlClass()
    {
        HINSTANCE hInstance = GetModuleHandle(NULL);
        ControlClass = new WNDCLASSEX();

        ControlClass->cbSize = sizeof(WNDCLASSEX);
        ControlClass->style = 0;
        ControlClass->lpfnWndProc = WndProc;
        ControlClass->cbClsExtra = 0;
        ControlClass->cbWndExtra = 0;
        ControlClass->hInstance = hInstance;
        ControlClass->hIcon = LoadIcon(NULL, IDI_APPLICATION);
        ControlClass->hCursor = LoadCursor(NULL, IDC_ARROW);
        ControlClass->hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
        ControlClass->lpszMenuName = NULL;
        ControlClass->lpszClassName = L"NativeUITextBoxHolder";
        ControlClass->hIconSm = LoadIcon(NULL, IDI_APPLICATION);

        if (!RegisterClassEx(ControlClass))
        {
            MessageBox(NULL, L"UserControl Registration Failed!", L"Error!", MB_ICONEXCLAMATION | MB_OK);
        }
    }

    LRESULT CALLBACK TextBox::WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
    {
        auto twIter = TextBoxInstances.find(hwnd);
        if (twIter == TextBoxInstances.end())
        {
            return DefWindowProc(hwnd, msg, wParam, lParam);
        }
        TextBox* textBox = twIter->second;


        switch (msg)
        {
        case WM_COMMAND:
        {
            if (HIWORD(wParam) == EN_CHANGE)
            {
                if (textBox->mReadOnly)
                {
                    static bool recursionGuard = false; // TODO
                    if (!recursionGuard)
                    {
                        recursionGuard = true;
                        textBox->SetText(textBox->mText);
                    }
                    recursionGuard = false;
                }
                else
                {
                    wchar_t txt[512];
                    GetWindowText(textBox->mEditHWND, txt, 100);
                    std::string str = TextUtils::WCharToUTF8(txt);
                    for (auto callback : textBox->mTextChangedCallbacks)
                    {
                        callback(str);
                    }
                }
            }
            break;
        }
        }
        return DefWindowProc(hwnd, msg, wParam, lParam);
    }

    void TextBox::OnTransformUpdated()
    {
        Control::OnTransformUpdated();

        const int x = 0;// (int)mPositionClientCoords.x;
        const int y = 0;// (int)mPositionClientCoords.y;
        const int w = (int)mSizeClientCoords.x;
        const int h = (int)mSizeClientCoords.y;
        SetWindowPos(mEditHWND, HWND_TOP, x, y, w, h, 0);
    }

    void TextBox::SetReadonly(bool arg_readonly)
    {
        mReadOnly = true;
        //SendMessage(mEditHWND, EM_SETREADONLY, (WPARAM)arg_readonly, 0);
    }

    void TextBox::SetText(std::string arg_text)
    {
        mText = arg_text;
        std::wstring wstr = TextUtils::UTF8ToWChar(arg_text);
        SetWindowText(mEditHWND, wstr.c_str());
    }

    void TextBox::SetFontSize(int arg_size)
    {
        HFONT font = CreateFont(arg_size, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, L"Arial");
        SendMessage(mEditHWND, WM_SETFONT, (WPARAM)font, TRUE);
    }

    void TextBox::RegisterTextChangedCallback(std::function<void(std::string)> inCallback)
    {
        mTextChangedCallbacks.push_back(inCallback);
    }

}
