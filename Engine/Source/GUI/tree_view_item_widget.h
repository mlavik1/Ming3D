#pragma once
#include "widget.h"
#include <string>
#include <memory>

namespace Ming3D
{
    class TextWidget;

    class TreeViewItemWidget : public Widget
    {
    public:
        int mId;
        std::string mDisplayName;
        std::shared_ptr<TextWidget> mTextWidget;
        int mDepth;
        
    public:
        TreeViewItemWidget(int id, const std::string& displayName, int depth = 0);
        virtual ~TreeViewItemWidget();
    };
}
