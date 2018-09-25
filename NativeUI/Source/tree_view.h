#ifndef NATIVEUI_TREEVIEW
#define NATIVEUI_TREEVIEW

#include "control.h"
#include <string>

namespace NativeUI
{
    class TreeViewItem
    {
    public:
        std::string mName;
        long mUserData;

        TreeViewItem(std::string inName, long inUserData = 0)
            : mName(inName), mUserData(inUserData) {}
    };

	class TreeView : public Control
	{
	public:
        TreeView(Control* arg_parent);

        void AddItem(TreeViewItem inItem);
	};
}

#endif // NATIVEUI_TREEVIEW
