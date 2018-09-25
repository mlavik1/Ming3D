#ifndef NATIVEUI_TEXTBOX_H
#define NATIVEUI_TEXTBOX_H

#include "control.h"

namespace NativeUI
{
	class TextBox : public Control
	{
	public:
        TextBox(Control* arg_parent);

		void SetText(const char* arg_text);
        void SetFontSize(int arg_size);
	};
}

#endif
