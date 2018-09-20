#ifndef NATIVEUI_USERCONTROL
#define NATIVEUI_USERCONTROL
#include "control.h"

#include <unordered_map>

namespace NativeUI
{
	class UserControl : public Control
	{
	private:
		static std::unordered_map<HWND, UserControl*> RegisteredUserControls;
		static WNDCLASSEX* UserControlClass;
		static void CreateUserControlClass();
		int mScrollX = 0;
		int mScrollY = 0;
		bool mScrollbarVisible = false;

	protected:
		virtual void OnChildTransformUpdated() override;
		void UpdateScrollbar();

	public:
		static LRESULT CALLBACK UserControl::WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
		virtual void OnPaint();
		void ShowScrollbar(bool arg_show);

	public:
		UserControl(Control* arg_parent);
	};
}

#endif
