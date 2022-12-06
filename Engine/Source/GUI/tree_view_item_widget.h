#pragma once
#include "widget.h"
#include <string>
#include <memory>
#include <functional>

namespace Ming3D
{
    class TextWidget;
    class ImageWidget;

    class TreeViewItemWidget : public Widget
    {
    public:
        int mId;
        std::string mDisplayName;
        std::shared_ptr<ImageWidget> mImageWidget;
        std::shared_ptr<TextWidget> mTextWidget;
        int mDepth;
        std::function<void(int)> mOnClick;
        
    public:
        TreeViewItemWidget(int id, const std::string& displayName, int depth = 0);
        virtual ~TreeViewItemWidget();

        virtual void OnInputEvent(InputEvent event) override;
    
        void SetSelected(bool selected);
    };
}
