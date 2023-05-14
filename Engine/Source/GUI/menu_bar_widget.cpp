#include "menu_bar_widget.h"
#include "GUI/button_widget.h"

IMPLEMENT_CLASS(Ming3D::MenuBarWidget)

namespace Ming3D
{
    void MenuBarWidget::SetItems(const std::vector<MenuBarItem>& items)
    {
        mItems = items;
        RebuildWidget();
    }

    void MenuBarWidget::SetItems(std::initializer_list<MenuBarItem> items)
    {
        mItems = items;
        RebuildWidget();
    }

    void MenuBarWidget::RebuildWidget()
    {
        mChildWidgets.clear();
        int currentPosition = 0;
        for (const MenuBarItem& item : mItems)
        {
            const int width = 10 + static_cast<int>(10 * item.mText.size());
            std::shared_ptr<ButtonWidget> itemWidget = std::make_shared<ButtonWidget>();
            itemWidget->SetText(item.mText);
            itemWidget->mOnClick = item.mOnClick;
            WidgetTransform transform{};
            transform.anchorMin = glm::vec2(0.0f, 0.0f);
            transform.anchorMax = glm::vec2(0.0f, 1.0f);
            transform.mPosition = glm::vec2(0.0f, 0.0f);
            transform.mPosition = glm::vec2(static_cast<float>(currentPosition), 0.0f);
            transform.mSize = glm::vec2(static_cast<float>(width), 1.0f);
            itemWidget->SetWidgetTransform(transform);
            addWidget(itemWidget);
            currentPosition += width;
        }
    }
}
