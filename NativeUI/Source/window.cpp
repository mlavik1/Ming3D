#include "window.h"
#include <iostream>
#include "text_utils.h"

namespace NativeUI
{
	WNDCLASSEX* Window::WindowClass = NULL;
	std::unordered_map<HWND, Window*> Window::mRegisteredWindows = std::unordered_map<HWND, Window*>();

	LRESULT CALLBACK Window::WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		Window* window = mRegisteredWindows[hwnd];
		if (window == nullptr)
		{
			return DefWindowProc(hwnd, msg, wParam, lParam);
		}
		static PAINTSTRUCT ps;

		switch (msg)
		{
		case WM_SIZE:
			window->OnUserResize();
			break;
		case WM_CLOSE:
			DestroyWindow(hwnd);
			break;
		case WM_PAINT:
			//BeginPaint(hwnd, &ps);
			// RENDERING HERE
			//EndPaint(hwnd, &ps);
			break;
        case WM_COMMAND:
        {
            auto menuItemID = window->mMenuItemIDs.find(LOWORD(wParam));
            if (menuItemID != window->mMenuItemIDs.end())
            {
                auto clickEvent = window->mMenuItemClickEvents.find(menuItemID->second);
                if (clickEvent != window->mMenuItemClickEvents.end())
                {
                    clickEvent->second();
                }
            }
            break;
        }
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		default:
			return DefWindowProc(hwnd, msg, wParam, lParam);
		}
		return DefWindowProc(hwnd, msg, wParam, lParam);;
	}

	void Window::CreateWindowClass()
	{
		HINSTANCE hInstance = GetModuleHandle(NULL);
		WindowClass = new WNDCLASSEX();

		WindowClass->cbSize = sizeof(WNDCLASSEX);
		WindowClass->style = 0;
		WindowClass->lpfnWndProc = WndProc;
		WindowClass->cbClsExtra = 0;
		WindowClass->cbWndExtra = 0;
		WindowClass->hInstance = hInstance;
		WindowClass->hIcon = LoadIcon(NULL, IDI_APPLICATION);
		WindowClass->hCursor = LoadCursor(NULL, IDC_ARROW);
		WindowClass->hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
		WindowClass->lpszMenuName = NULL;
		WindowClass->lpszClassName = L"NativeUWindowBase";
		WindowClass->hIconSm = LoadIcon(NULL, IDI_APPLICATION);

		if (!RegisterClassEx(WindowClass))
		{
			MessageBox(NULL, L"Window Registration Failed!", L"Error!", MB_ICONEXCLAMATION | MB_OK);
		}
	}

	Window::Window()
		: Control(NULL)
	{
		if (WindowClass == NULL)
		{
			CreateWindowClass();
		}
		HINSTANCE hInstance = GetModuleHandle(NULL);

		mHwnd = CreateWindowEx(
			WS_EX_CLIENTEDGE,
			L"NativeUWindowBase",
			L"Window title",
			WS_OVERLAPPEDWINDOW,
			CW_USEDEFAULT, CW_USEDEFAULT, 800, 600,
			NULL, NULL, hInstance, NULL);

		if (mHwnd == NULL)
		{
			MessageBox(NULL, L"Window Creation Failed!", L"Error!", MB_ICONEXCLAMATION | MB_OK);
		}
		else
		{
			mRegisteredWindows[mHwnd] = this;
			SetSize(Point(800.0f, 600.0f));
		}

	}

	void Window::Show()
	{
		ShowWindow(mHwnd, TRUE);
	}

	void Window::SetTitle(const char* arg_title)
	{
        std::wstring wstr = TextUtils::UTF8ToWChar(arg_title);
		SetWindowText(mHwnd, wstr.c_str());
	}

	void Window::Maximize()
	{
		SendMessage(mHwnd, WM_SYSCOMMAND, SC_MAXIMIZE, 0);
	}

    void Window::AddMenuPopup(std::string arg_id, std::string arg_name, std::string arg_parent)
    {
        if (mHMenuBar == nullptr)
            mHMenuBar = CreateMenu();

        if (mMenuPopups.find(arg_id) != mMenuPopups.end())
        {
            std::cout << "Menu popup already added: " << arg_id << std::endl;
            return;
        }

        HMENU hParentMenu = GetHMenu(arg_parent);
        if (hParentMenu == nullptr)
        {
            std::cout << "Failed to find parent menu: " << arg_parent << std::endl;
            return;
        }

        std::wstring wstr = TextUtils::UTF8ToWChar(arg_name);

        HMENU hMenu = CreateMenu();
        AppendMenuW(hParentMenu, MF_POPUP, (UINT_PTR)hMenu, wstr.c_str());

        mMenuPopups[arg_id] = hMenu;
    
        SetMenu(GetHwnd(), mHMenuBar);
    }

    void Window::AddMenuItem(std::string arg_id, std::string arg_name, std::string arg_parent)
    {
        HMENU hParentMenu = GetHMenu(arg_parent);

        if (hParentMenu == nullptr)
        {
            std::cout << "Found no menu popup with ID: " << arg_parent << std::endl;
            return;
        }

        UINT uid = mMenuItemUIDSequence++;
        mMenuItemIDs[uid] = arg_id;

        std::wstring wstr = TextUtils::UTF8ToWChar(arg_name);

        AppendMenuW(hParentMenu, MF_STRING, uid, wstr.c_str());

        SetMenu(GetHwnd(), mHMenuBar);
    }

    void Window::AddMenuSeparator(std::string arg_parent)
    {
        HMENU hParentMenu = GetHMenu(arg_parent);

        if (hParentMenu == nullptr)
        {
            std::cout << "Found no menu popup with ID: " << arg_parent << std::endl;
            return;
        }

        AppendMenuW(hParentMenu, MF_SEPARATOR, 0, NULL);
    }

    void Window::SetMenuItemClickEvent(std::string arg_id, std::function<void()> arg_func)
    {
        mMenuItemClickEvents[arg_id] = arg_func;
    }

    HMENU Window::GetHMenu(std::string arg_id)
    {
        if (arg_id == "")
            return mHMenuBar;

        auto parentMenuIter = mMenuPopups.find(arg_id);
        if (parentMenuIter == mMenuPopups.end())
        {
            return nullptr;
        }
        else
        {
            return parentMenuIter->second;
        }
    }
}
