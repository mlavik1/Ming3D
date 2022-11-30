#pragma once
#include "widget_transform.h"
#include <vector>
#include <memory>
#include "visual.h"
#include "Input/input_event.h"
#include "Object/object.h"
#include "Object/objdefs.h"

namespace Ming3D
{
    /**
    * Base class for add Widgets.
    */
    class Widget : public Object
    {
        friend class WidgetTree;
        
        DEFINE_CLASS(Ming3D::Widget, Ming3D::Object)

    private:
        static void InitialiseClass();

    protected:
        /* The local transform of this widget. Is relative to parent Widget. */
        WidgetTransform mTransform;

        /* The absolute (screen space) rect of this Widget's transform. */
        WidgetRect mAbsoluteRect;

        Widget* mParentWidget = nullptr;

        std::vector<std::shared_ptr<Widget>> mChildWidgets;
        std::vector<std::shared_ptr<Visual>> mVisuals;

        bool mTransformIsDirty = true;

        bool mWidgetInvalidated = true;

        /**
        * Marks the absolute transform as dirty.
        * This is called when doing transformations on the widget.
        */
        void setTransformDirty();

        void addVisual(std::shared_ptr<Visual> visual);

    public:
        Widget();
        virtual ~Widget();

        virtual void Tick(float inDeltaTime);

        void addWidget(std::shared_ptr<Widget> widget);

        /*
        * Gets the absolute (screen space) rect of this Widget's transform.
        **/
        WidgetRect getAbsoluteRect();

        void SetWidgetTransform(const WidgetTransform& transform);

        inline WidgetTransform GetWidgetTransform() const { return mTransform; }

        virtual void OnInputEvent(InputEvent event) {}

        const std::vector<std::shared_ptr<Widget>>& GetChildren() { return mChildWidgets; }
    };
}
