#include "tree_view.h"
#include "text_utils.h"

#include <assert.h>
#include <Windows.h>
#include <CommCtrl.h>
#pragma comment(lib,"comctl32.lib")  // TODO

namespace NativeUI
{
    WNDCLASSEX* TreeView::ControlClass = NULL;
    HWND TreeView::NewlyCreatedTreeViewHandle = NULL;
    std::unordered_map<void*, TreeView*> TreeView::TreeViewInstances;

	TreeView::TreeView(Control* arg_parent)
		: Control(arg_parent)
	{
        if (ControlClass == NULL)
        {
            CreateControlClass();
        }

        HINSTANCE hInstance = GetModuleHandle(NULL);

        RECT rcClient;
        GetClientRect(arg_parent->GetHwnd(), &rcClient);

        mHwnd = CreateWindow(
            TEXT("NativeUITreeView"), L"",
            WS_VISIBLE | WS_CHILD | WS_BORDER | TVS_HASLINES,
            0,
            0,
            rcClient.right,
            rcClient.bottom,
            arg_parent->GetHwnd(),
            NULL,
            hInstance,
            NULL);

        // TEMP! TODO
        assert(NewlyCreatedTreeViewHandle);
        mTreeViewHWND = NewlyCreatedTreeViewHandle; // assigned in WM_CREATE
        TreeViewInstances[mTreeViewHWND] = this;
        NewlyCreatedTreeViewHandle = NULL;

        TreeViewInstances[mHwnd] = this;
        //mOrigWndProc = (WNDPROC)SetWindowLong(mHwnd, GWL_WNDPROC, (LONG)WndProc);

        //SetSize(Point(80.0f, 40.0f));
		ShowWindow(mHwnd, TRUE);
		UpdateWindow(mHwnd);
	}

    void TreeView::CreateControlClass()
    {
        HINSTANCE hInstance = GetModuleHandle(NULL);
        ControlClass = new WNDCLASSEX();

        ControlClass->cbSize = sizeof(WNDCLASSEX);
        ControlClass->style = 0;
        ControlClass->lpfnWndProc = WndProc;
        ControlClass->cbClsExtra = 0;
        ControlClass->cbWndExtra = 0;
        ControlClass->hInstance = hInstance;
        ControlClass->hIcon = LoadIcon(NULL, IDI_APPLICATION);
        ControlClass->hCursor = LoadCursor(NULL, IDC_ARROW);
        ControlClass->hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
        ControlClass->lpszMenuName = NULL;
        ControlClass->lpszClassName = L"NativeUITreeView";
        ControlClass->hIconSm = LoadIcon(NULL, IDI_APPLICATION);

        if (!RegisterClassEx(ControlClass))
        {
            MessageBox(NULL, L"UserControl Registration Failed!", L"Error!", MB_ICONEXCLAMATION | MB_OK);
        }
    }

    LRESULT CALLBACK TreeView::WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
    {
        if (msg == WM_CREATE)
        {
            HINSTANCE hInstance = GetModuleHandle(NULL);

            RECT rc;
            GetClientRect(hwnd, &rc);

            NewlyCreatedTreeViewHandle = CreateWindowEx(0,
                WC_TREEVIEW,
                TEXT("Tree View"),
                WS_VISIBLE | WS_CHILD | WS_BORDER | TVS_HASLINES,
                0,
                0,
                rc.right,
                rc.bottom,
                hwnd,
                NULL,
                hInstance,
                NULL);
            return DefWindowProc(hwnd, msg, wParam, lParam);
        }

        auto twIter = TreeViewInstances.find(hwnd);
        if (twIter == TreeViewInstances.end())
        {
            return DefWindowProc(hwnd, msg, wParam, lParam);
        }
        TreeView* treeView = twIter->second;

        static PAINTSTRUCT ps;

        switch (msg)
        {
        case WM_NOTIFY:
        {
            LPNM_TREEVIEW pntv = (LPNM_TREEVIEW)lParam;
            if (pntv->hdr.code == TVN_SELCHANGED)
            {
                auto callbackIter = treeView->mOnSelectCallbacks.find(pntv->itemNew.lParam);
                if (callbackIter != treeView->mOnSelectCallbacks.end())
                {
                    for (auto callback : callbackIter->second)
                    {
                        callback();
                    }
                }
            }
            break;
        }
        }
        return DefWindowProc(hwnd, msg, wParam, lParam);
    }

    void TreeView::OnTransformUpdated()
    {
        Control::OnTransformUpdated();

        const int x = (int)mPositionClientCoords.x;
        const int y = (int)mPositionClientCoords.y;
        const int w = (int)mSizeClientCoords.x;
        const int h = (int)mSizeClientCoords.y;
        SetWindowPos(mTreeViewHWND, HWND_TOP, x, y, w, h, 0);
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
        hPrev = (HTREEITEM)SendMessage(mTreeViewHWND, TVM_INSERTITEM,
            0, (LPARAM)(LPTVINSERTSTRUCT)&tvins);
    }

    void TreeView::RegisterSelectCallback(long itemUserData, std::function<void()> callback)
    {
        if (mOnSelectCallbacks.find(itemUserData) == mOnSelectCallbacks.end())
            mOnSelectCallbacks.emplace(itemUserData, std::vector<std::function<void()>>());
        mOnSelectCallbacks[itemUserData].push_back(callback);
    }
}
