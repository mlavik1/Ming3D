#include "tree_view_item_widget.h"
#include "GUI/text_widget.h"

namespace Ming3D
{
    TreeViewItemWidget::TreeViewItemWidget(int id, const std::string& displayName, int depth)
    : mId(id), mDisplayName(displayName), mDepth(depth)
    {
        mTextWidget = std::make_shared<TextWidget>();
        mTextWidget->SetText(displayName);
        addWidget(mTextWidget);
    }

    TreeViewItemWidget::~TreeViewItemWidget()
    {
        
    }
}
