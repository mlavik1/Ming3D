#ifndef NATIVEUI_BUTTON
#define NATIVEUI_BUTTON

#include "control.h"

namespace NativeUI
{
	class Button : public Control
	{
	public:
		Button(Control* arg_parent);

		void SetText(const char* arg_text);
        void SetImage(const char* arg_image);
	};
}

#endif // NATIVEUI_BUTTON
