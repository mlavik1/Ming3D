#include "tree_view.h"
#include "text_utils.h"

#include <Windows.h>
#include <CommCtrl.h>
#pragma comment(lib,"comctl32.lib")  // TODO

namespace NativeUI
{
	TreeView::TreeView(Control* arg_parent)
		: Control(arg_parent)
	{

#define ID_TREEVIEW 42
        HINSTANCE hInstance = GetModuleHandle(NULL);

        RECT rcClient;
        GetClientRect(arg_parent->GetHwnd(), &rcClient);

        mHwnd = CreateWindowEx(0,
            WC_TREEVIEW,
            TEXT("Tree View"),
            WS_VISIBLE | WS_CHILD | WS_BORDER | TVS_HASLINES,
            0,
            0,
            rcClient.right,
            rcClient.bottom,
            arg_parent->GetHwnd(),
            (HMENU)ID_TREEVIEW,
            hInstance,
            NULL);
        
        //SetSize(Point(80.0f, 40.0f));
		ShowWindow(mHwnd, TRUE);
		UpdateWindow(mHwnd);
	}

    void TreeView::AddItem(TreeViewItem inItem)
    {
        TVITEM tvi;
        TVINSERTSTRUCT tvins;
        static HTREEITEM hPrev = (HTREEITEM)TVI_FIRST;
        static HTREEITEM hPrevRootItem = NULL;
        static HTREEITEM hPrevLev2Item = NULL;
        HTREEITEM hti;

        tvi.mask = TVIF_TEXT | TVIF_IMAGE
            | TVIF_SELECTEDIMAGE | TVIF_PARAM;

        // Set the text of the item. 
        std::wstring itemName = TextUtils::UTF8ToWChar(inItem.mName);
        const size_t nameLen = itemName.length();
        LPWSTR lpwName = new WCHAR[nameLen + 1]; // TODO: I hope Windows deletes this..
        memcpy(lpwName, itemName.c_str(), sizeof(WCHAR) * nameLen);
        lpwName[nameLen] = 0;
        tvi.pszText = lpwName;
        tvi.cchTextMax = sizeof(tvi.pszText) / sizeof(tvi.pszText[0]);

        // Save the heading level in the item's application-defined 
        // data area. 
        tvi.lParam = inItem.mUserData;
        tvins.item = tvi;
        tvins.hInsertAfter = hPrev;

        // Set the parent item based on the specified level. 
        tvins.hParent = TVI_ROOT;

        // Add the item to the tree-view control. 
        hPrev = (HTREEITEM)SendMessage(mHwnd, TVM_INSERTITEM,
            0, (LPARAM)(LPTVINSERTSTRUCT)&tvins);
    }
}
