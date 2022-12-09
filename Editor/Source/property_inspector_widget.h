#pragma once
#include "GUI/widget.h"
#include <memory>
#include "widget_layout_builder.h"

namespace Ming3D
{
    class WidgetLayoutBuilder;

    class PropertyInspectorWidget : public Widget
    {
        DEFINE_CLASS(Ming3D::PropertyInspectorWidget, Ming3D::Widget)

    private:
        static void InitialiseClass();
        std::unique_ptr<WidgetLayoutBuilder> mWidgetLayoutBuilder;
        EditorControlStyle mHeaderTextStyle;
        EditorControlStyle mSubHeaderTextStyle;

    public:
        PropertyInspectorWidget();
        virtual ~PropertyInspectorWidget();

        virtual void Start() override;
        virtual void Tick(float deltaTime) override;
    };
}
