#include "tree_view_item_widget.h"
#include "GUI/text_widget.h"
#include "GUI/image_widget.h"

namespace Ming3D
{
    TreeViewItemWidget::TreeViewItemWidget(int id, const std::string& displayName, int depth)
    : mId(id), mDisplayName(displayName), mDepth(depth)
    {
        mImageWidget = std::make_shared<ImageWidget>();
        mImageWidget->SetColour(glm::vec4(0.7f, 0.7f, 0.7f, 0.3f));
        mImageWidget->SetEnabled(false);
        addWidget(mImageWidget);
        mTextWidget = std::make_shared<TextWidget>();
        mTextWidget->SetText(displayName);
        addWidget(mTextWidget);
    }

    TreeViewItemWidget::~TreeViewItemWidget()
    {
        
    }

    void TreeViewItemWidget::OnInputEvent(InputEvent event)
    {
        Widget::OnInputEvent(event);

        if (event.mType == InputEventType::MouseButtonUp)
        {
            mOnClick(mId);
        }
    }

    void TreeViewItemWidget::SetSelected(bool selected)
    {
        mImageWidget->SetEnabled(selected);
    }
}
