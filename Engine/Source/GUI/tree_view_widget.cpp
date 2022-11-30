#include "tree_view_widget.h"
#include "tree_view_item_widget.h"

IMPLEMENT_CLASS(Ming3D::TreeViewWidget)

namespace Ming3D
{
    TreeViewWidget::TreeViewWidget()
    {

    }

    TreeViewWidget::~TreeViewWidget()
    {

    }

    void TreeViewWidget::InitialiseClass()
    {

    }

    void TreeViewWidget::AddItem(int id, const std::string& displayName, int depth)
    {
        std::shared_ptr<TreeViewItemWidget> itemWidget = std::make_shared<TreeViewItemWidget>(id, displayName, depth);
        mItems.push_back(itemWidget);
        this->addWidget(itemWidget);
        UpdateTransforms();
    }

    void TreeViewWidget::UpdateTransforms()
    {
        for (size_t i = 0; i < mItems.size(); ++i)
        {
            WidgetTransform transform{};
            transform.anchorMin = glm::vec2(0.0f, 0.0f);
            transform.anchorMax = glm::vec2(1.0f, 0.0f);
            transform.mSize.y = mItemHeight;
            transform.mPosition.x = mIndentWidth * i;
            transform.mPosition.y = mItemHeight * i;
            mItems[i]->SetWidgetTransform(transform);
        }
    }
}
