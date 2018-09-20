#ifndef NATIVEUI_PANEL
#define NATIVEUI_PANEL
#include "user_control.h"

#include <unordered_map>

namespace NativeUI
{
	class Panel : public UserControl
	{
	public:
		Panel(Control* arg_parent);
	};
}

#endif
