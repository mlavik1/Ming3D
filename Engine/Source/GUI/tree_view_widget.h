#pragma once
#include "widget.h"
#include <vector>
#include <memory>

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

        void UpdateTransforms();

    public:
        TreeViewWidget();
        virtual ~TreeViewWidget();
        void AddItem(int id, const std::string& displayName, int depth = 0);
    };
}
