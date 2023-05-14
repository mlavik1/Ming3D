#pragma once
#include "GUI/menu_bar_widget.h"

namespace Ming3D
{
    class EditorMenuBarWidget: public MenuBarWidget
    {
        DEFINE_CLASS(Ming3D::EditorMenuBarWidget, Ming3D::MenuBarWidget)
    private:
        static void InitialiseClass() {}
        void CallbackImportModel();
    public:
        EditorMenuBarWidget();
    };
}
