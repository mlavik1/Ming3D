#ifndef NATIVEUI_BUTTON
#define NATIVEUI_BUTTON

#include "control.h"
#include <vector>
#include <functional>
#include <unordered_map>

namespace NativeUI
{
	class Button : public Control
	{
    private:
        std::vector<std::function<void()>> mClickCallbacks;
        
        WNDPROC mOrigWndProc;

        static std::unordered_map<HWND, Button*> ButtonInstances;

        static WNDCLASSEX* WindowClass;
        static void CreateWindowClass();

	public:
		Button(Control* arg_parent);
        virtual ~Button();

		void SetText(const char* arg_text);
        void SetImage(const char* arg_image);

        void RegisterClickCallback(std::function<void()> inCallback);

        static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
	};
}

#endif // NATIVEUI_BUTTON
