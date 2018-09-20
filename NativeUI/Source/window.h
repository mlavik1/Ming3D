#ifndef NATIVEUI_WINDOW
#define NATIVEUI_WINDOW

#include "control.h"
#include <unordered_map>
#include <string>
#include <functional>

namespace NativeUI
{
	class Window : public Control
	{
	private:
        std::unordered_map<std::string, HMENU> mMenuPopups;
        std::unordered_map<UINT, std::string> mMenuItemIDs;
        std::unordered_map<std::string, std::function<void()>> mMenuItemClickEvents;
        HMENU mHMenuBar = nullptr;

		static std::unordered_map<HWND, Window*> mRegisteredWindows;
		static WNDCLASSEX* WindowClass;
		static void CreateWindowClass();

        HMENU GetHMenu(std::string arg_id);

        UINT mMenuItemUIDSequence = 1000;

	public:
		static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

	public:
		Window::Window();

		void Show();
		void SetTitle(const char* arg_title);
		void Maximize();

        void AddMenuPopup(std::string arg_id, std::string arg_name, std::string arg_parent = "");
        void AddMenuItem(std::string arg_id, std::string arg_name, std::string arg_parent);
        void AddMenuSeparator(std::string arg_parent);
        void SetMenuItemClickEvent(std::string arg_id, std::function<void()> arg_func);
    };
}

#endif // NATIVEUI_WINDOW
