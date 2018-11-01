#ifndef NATIVEUI_TREEVIEW
#define NATIVEUI_TREEVIEW

#include "control.h"
#include <string>
#include <unordered_map>
#include <functional>

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
    private:
        HWND mTreeViewHWND = NULL;
        WNDPROC mOrigWndProc;
        std::unordered_map<long, std::vector<std::function<void()>>> mOnSelectCallbacks;

        static HWND NewlyCreatedTreeViewHandle; // TEMP!
        static std::unordered_map<void*, TreeView*> TreeViewInstances;
        static WNDCLASSEX* ControlClass;

        static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
        static void CreateControlClass();

    protected:
        virtual void OnTransformUpdated() override;

	public:
        TreeView(Control* arg_parent);

        void AddItem(TreeViewItem inItem);
        void RegisterSelectCallback(long itemUserData, std::function<void()> callback);
	};
}

#endif // NATIVEUI_TREEVIEW
