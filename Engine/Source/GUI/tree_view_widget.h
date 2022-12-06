#pragma once
#include "widget.h"
#include <vector>
#include <memory>
#include <functional>

namespace Ming3D
{
    class TreeViewItemWidget;

    class TreeViewWidget : public Widget
    {
        DEFINE_CLASS(Ming3D::TreeViewWidget, Ming3D::Widget)

    private:
        static void InitialiseClass();

    private:
        std::vector<std::shared_ptr<TreeViewItemWidget>> mItems;
        int mItemHeight = 16;
        int mIndentWidth = 16;
        std::weak_ptr<TreeViewItemWidget> mSelectedItem;

        void UpdateTransforms();
        void OnItemClicked(int id);

    public:
        TreeViewWidget();
        virtual ~TreeViewWidget();
        
        std::function<void(int)> mOnItemSelected;
        
        void AddItem(int id, const std::string& displayName, int depth = 0);
    };
}
