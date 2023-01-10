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

    void TreeViewWidget::OnItemClicked(int id)
    {
        auto itWidget = std::find_if(mItems.begin(), mItems.end(), [id](auto candidate){ return candidate->mId == id; });
        if (itWidget != mItems.end())
        {
            if (!mSelectedItem.expired())
                mSelectedItem.lock()->SetSelected(false);
            mSelectedItem = *itWidget;
            (*itWidget)->SetSelected(true);
        }
        mOnItemSelected(id);
    }

    void TreeViewWidget::AddItem(int id, const std::string& displayName, int depth)
    {
        std::shared_ptr<TreeViewItemWidget> itemWidget = std::make_shared<TreeViewItemWidget>(id, displayName, depth);
        itemWidget->mOnClick = [this](int id){
            this->OnItemClicked(id);
        };
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
            transform.mSize.y = static_cast<float>(mItemHeight);
            transform.mPosition.x = static_cast<float>(mIndentWidth * mItems[i]->mDepth);
            transform.mPosition.y = static_cast<float>(mItemHeight) * i;
            mItems[i]->SetWidgetTransform(transform);
        }
    }
}
