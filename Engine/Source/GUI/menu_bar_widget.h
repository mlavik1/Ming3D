#pragma once
#include <string>
#include <vector>
#include <initializer_list>
#include <functional>
#include "GUI/widget.h"

namespace Ming3D
{
    class MenuBarWidget: public Widget
    {
        DEFINE_CLASS(Ming3D::MenuBarWidget, Ming3D::Widget)
    public:
        class MenuBarItem
        {
        public:
            std::string mText;
            std::function<void()> mOnClick;
            std::vector<MenuBarItem> mChildren;
        };

        void SetItems(const std::vector<MenuBarItem>& items);
        void SetItems(std::initializer_list<MenuBarItem> items);
        void RebuildWidget();

    private:
        std::vector<MenuBarItem> mItems;
        static void InitialiseClass() {}
    };
}
