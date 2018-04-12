#include "winapi_window.h"

namespace Ming3D
{
	LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
		switch (message)
		{
		case WM_DESTROY:
		{
			PostQuitMessage(0);
			return 0;
		} break;
		}

		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	void WinAPIWindow::Initialise()
	{
		HINSTANCE hInstance = GetModuleHandle(NULL);

		WNDCLASSEX wc;

		ZeroMemory(&wc, sizeof(WNDCLASSEX));

		wc.cbSize = sizeof(WNDCLASSEX);
		wc.style = CS_HREDRAW | CS_VREDRAW;
		wc.lpfnWndProc = WindowProc;
		wc.hInstance = hInstance;
		wc.hCursor = LoadCursor(NULL, IDC_ARROW);
		wc.lpszClassName = L"WindowClass";

		RegisterClassEx(&wc);

		RECT wr = { 0, 0, mWindowWidth, mWindowHeight };
		AdjustWindowRect(&wr, WS_OVERLAPPEDWINDOW, FALSE);

		mHWND = CreateWindowEx(NULL,
			L"WindowClass",
			L"Ming3D",
			WS_OVERLAPPEDWINDOW,
			100,
			100,
			wr.right - wr.left,
			wr.bottom - wr.top,
			NULL,
			NULL,
			hInstance,
			NULL);

		ShowWindow(mHWND, SW_SHOW);

	}

	void WinAPIWindow::SetSize(unsigned int inWidth, unsigned int inHeight)
	{
		mWindowHeight = inWidth;
		mWindowHeight = inHeight;
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
}
