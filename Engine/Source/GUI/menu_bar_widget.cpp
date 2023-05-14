#include "menu_bar_widget.h"
#include "GUI/button_widget.h"
#include "GUI/image_widget.h"
#include <algorithm>

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
            itemWidget->mOnClick = [this, item, currentPosition](){ this->OnMenuItemClick(item, currentPosition, 0); };
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

    void MenuBarWidget::OnMenuItemClick(const MenuBarItem& menuItem, int x, int y)
    {
        if (mPopupWidget != nullptr)
        {
            RemovePopupWidget(mPopupWidget.get());
            mPopupWidget = nullptr;
        }
        if (menuItem.mChildren.size() > 0)
        {
            std::shared_ptr<ImageWidget> popupWidget = std::make_shared<ImageWidget>();
            {
                int height = static_cast<int>(menuItem.mChildren.size()) * 40;
                size_t maxStringLength = 0;
                for (const MenuBarItem& item : menuItem.mChildren)
                    maxStringLength = std::max(maxStringLength, item.mText.size());
                int width = 10 + 10 * static_cast<int>(maxStringLength);
                WidgetTransform transform{};
                transform.anchorMin = glm::vec2(0.0f, 0.0f);
                transform.anchorMax = glm::vec2(0.0f, 0.0f);
                transform.mPosition = glm::vec2(static_cast<float>(x), static_cast<float>(y));
                transform.mSize = glm::vec2(static_cast<float>(height), static_cast<float>(width));
                popupWidget->SetWidgetTransform(transform);
                popupWidget->SetColour(glm::vec4(0.195f, 0.195f, 0.195f, 1.0f));
            }

            int currentPosition = 0;
            for (const MenuBarItem& item : menuItem.mChildren)
            {
                const int itemHeight = 40;
                const int itemWidth = 10 + static_cast<int>(10 * item.mText.size());
                std::shared_ptr<ButtonWidget> itemWidget = std::make_shared<ButtonWidget>();
                itemWidget->SetText(item.mText);
                itemWidget->mOnClick = [&](){
                    if (item.mOnClick != nullptr)
                        item.mOnClick();
                    RemovePopupWidget(popupWidget.get());
                };
                WidgetTransform transform{};
                transform.anchorMin = glm::vec2(0.0f, 0.0f);
                transform.anchorMax = glm::vec2(0.0f, 0.0f);
                transform.mPosition = glm::vec2(0.0f, 0.0f);
                transform.mPosition = glm::vec2(0.0f, static_cast<float>(currentPosition));
                transform.mSize = glm::vec2(static_cast<float>(itemWidth), static_cast<float>(itemHeight));
                itemWidget->SetWidgetTransform(transform);
                popupWidget->addWidget(itemWidget);
                currentPosition += itemHeight;
            }

            AddPopupWidget(popupWidget);
        }
    }
}
