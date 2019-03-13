#include "combo_box.h"
#include "text_utils.h"

namespace NativeUI
{
    std::unordered_map<void*, ComboBox*> ComboBox::ComboBoxInstances;
    WNDCLASSEX* ComboBox::ControlClass = NULL;

    ComboBox::ComboBox(Control* arg_parent)
		: Control(arg_parent)
	{
        if (ControlClass == NULL)
            CreateControlClass();
        mHwnd = CreateWindow(L"NativeUITextBoxHolder", L"", WS_CHILD | WS_VISIBLE, 0, 0, 50, 50, arg_parent->GetHwnd(), NULL, GetModuleHandle(NULL), NULL);
        ComboBoxInstances[mHwnd] = this;
        SetSize(Point(80.0f, 40.0f));
		ShowWindow(mHwnd, TRUE);
		UpdateWindow(mHwnd);
	}

    void ComboBox::CreateControlClass()
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

    LRESULT CALLBACK ComboBox::WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
    {
        auto twIter = ComboBoxInstances.find(hwnd);
        if (twIter == ComboBoxInstances.end())
        {
            return DefWindowProc(hwnd, msg, wParam, lParam);
        }
        ComboBox* cmbBox = twIter->second;


        switch (msg)
        {
        case WM_COMMAND:
        {
            if (HIWORD(wParam) == EN_CHANGE)
            {
            }
            break;
        }
        }
        return DefWindowProc(hwnd, msg, wParam, lParam);
    }

}
