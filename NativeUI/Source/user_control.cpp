#include "user_control.h"

#include <iostream>

namespace NativeUI
{
	WNDCLASSEX* UserControl::UserControlClass = NULL;
	std::unordered_map<HWND, UserControl*> UserControl::RegisteredUserControls = std::unordered_map<HWND, UserControl*>();

	LRESULT CALLBACK UserControl::WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		UserControl* userControl = RegisteredUserControls[hwnd];
		if (userControl == nullptr)
		{
			return DefWindowProc(hwnd, msg, wParam, lParam);
		}
		static PAINTSTRUCT ps;

		switch (msg)
		{
		case WM_SIZE:
			userControl->OnResize();
			break;
		case WM_CLOSE:
			DestroyWindow(hwnd);
			break;
		case WM_PAINT:
			BeginPaint(hwnd, &ps);
			userControl->OnPaint();
			EndPaint(hwnd, &ps);
			break;
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		case WM_HSCROLL:
		{
			SCROLLINFO si = { 0 };
			si.cbSize = sizeof(SCROLLINFO);
			si.fMask = SIF_POS;
			int newX = -1;
			switch (LOWORD(wParam))
			{
				case SB_THUMBPOSITION:
				{
					newX = HIWORD(wParam);
					break;
				}
			}
			if (newX != -1)
			{
				int deltaX = newX - userControl->mScrollX;
				si.nPos = newX;
				SetScrollInfo(hwnd, SB_HORZ, &si, TRUE);
				ScrollWindow(hwnd, -deltaX, 0, NULL, NULL);
				userControl->mScrollX = newX;
			}
			break;
		}
		case WM_VSCROLL:
		{
			SCROLLINFO si = { 0 };
			si.cbSize = sizeof(SCROLLINFO);
			si.fMask = SIF_POS;
			int newY = -1;
			switch (LOWORD(wParam))
			{
			case SB_THUMBPOSITION:
			{
				newY = HIWORD(wParam);
				break;
			}
			}
			if (newY != -1)
			{
				int deltaY = newY - userControl->mScrollY;
				si.nPos = newY;
				SetScrollInfo(hwnd, SB_VERT, &si, TRUE);
				ScrollWindow(hwnd, 0, -deltaY, NULL, NULL);
				userControl->mScrollY = newY;
			}
			break;
		}
		default:
			return DefWindowProc(hwnd, msg, wParam, lParam);
		}
		return 0;
	}

	void UserControl::CreateUserControlClass()
	{
		HINSTANCE hInstance = GetModuleHandle(NULL);
		UserControlClass = new WNDCLASSEX();

		UserControlClass->cbSize = sizeof(WNDCLASSEX);
		UserControlClass->style = 0;
		UserControlClass->lpfnWndProc = WndProc;
		UserControlClass->cbClsExtra = 0;
		UserControlClass->cbWndExtra = 0;
		UserControlClass->hInstance = hInstance;
		UserControlClass->hIcon = LoadIcon(NULL, IDI_APPLICATION);
		UserControlClass->hCursor = LoadCursor(NULL, IDC_ARROW);
		UserControlClass->hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
		UserControlClass->lpszMenuName = NULL;
		UserControlClass->lpszClassName = L"NativeUIUserControl";
		UserControlClass->hIconSm = LoadIcon(NULL, IDI_APPLICATION);

		if (!RegisterClassEx(UserControlClass))
		{
			MessageBox(NULL, L"UserControl Registration Failed!", L"Error!", MB_ICONEXCLAMATION | MB_OK);
		}
	}

	UserControl::UserControl(Control* arg_parent)
		: Control(arg_parent)
	{
		if (UserControlClass == NULL)
		{
			CreateUserControlClass();
		}

		mHwnd = CreateWindow(L"NativeUIUserControl", L"", WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON | WS_HSCROLL | WS_VSCROLL, 0, 0, 50, 50, arg_parent->GetHwnd(), NULL, GetModuleHandle(NULL), NULL);
		ShowWindow(mHwnd, TRUE);
		UpdateWindow(mHwnd);

		if (mHwnd == NULL)
		{
			MessageBox(NULL, L"UserControl Creation Failed!", L"Error!", MB_ICONEXCLAMATION | MB_OK);
		}
		else
		{
			RegisteredUserControls[mHwnd] = this;
		}

		ShowScrollbar(false);
	}

	void UserControl::OnChildTransformUpdated()
	{
		Control::OnChildTransformUpdated();

		if (mScrollbarVisible)
		{
			UpdateScrollbar();
		}
	}

	void UserControl::UpdateScrollbar()
	{
		NativeUI::Point maxRBCorner = GetSizeClientCoords();
		for (Control* child : mChildren)
		{
			const NativeUI::Point childPos = child->GetPositionClientCoords();
			const NativeUI::Point childSize = child->GetSizeClientCoords();
			const NativeUI::Point childRBCorner = childPos + childSize;
			if (childRBCorner.x > maxRBCorner.x)
				maxRBCorner.x = childRBCorner.x;
			if (childRBCorner.y > maxRBCorner.y)
				maxRBCorner.y = childRBCorner.y;
		}

		SCROLLINFO siHor = { 0 };
		siHor.cbSize = sizeof(SCROLLINFO);
		siHor.fMask = SIF_RANGE;
		siHor.nMin = 0;
		siHor.nMax = (int)maxRBCorner.x;

		SCROLLINFO siVer = siHor;
		siVer.nMin = 0;
		siVer.nMax = (int)maxRBCorner.y;

		SetScrollInfo(mHwnd, SB_HORZ, &siHor, TRUE);
		SetScrollInfo(mHwnd, SB_VERT, &siVer, TRUE);
	}

	void UserControl::OnPaint()
	{

	}

	void UserControl::ShowScrollbar(bool arg_show)
	{
		ShowScrollBar(mHwnd, SB_BOTH, arg_show);
		mScrollbarVisible = arg_show;
		if (arg_show)
		{
			UpdateScrollbar();
		}
	}
}
