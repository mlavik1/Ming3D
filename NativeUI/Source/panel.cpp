#include "panel.h"

namespace NativeUI
{
	Panel::Panel(Control* arg_parent)
		: UserControl(arg_parent)
	{
		ShowScrollBar(mHwnd, SB_BOTH, FALSE);
	}



}
