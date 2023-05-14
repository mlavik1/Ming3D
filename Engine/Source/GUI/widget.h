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
    class World;
    
    /**
    * Base class for add Widgets.
    */
    class Widget : public Object
    {
        friend class WidgetTree;
        friend class WidgetComponent;
        
        DEFINE_CLASS(Ming3D::Widget, Ming3D::Object)

    private:
        static void InitialiseClass();

        bool mHasTicked = false;
        World* mWorld;
        bool mHasMouseInside = false;

    protected:
        /* The local transform of this widget. Is relative to parent Widget. */
        WidgetTransform mTransform;

        /* The absolute (screen space) rect of this Widget's transform. */
        WidgetRect mAbsoluteRect;

        Widget* mParentWidget = nullptr;
        WidgetTree* mWidgetTree = nullptr; // TODO

        std::vector<std::shared_ptr<Widget>> mChildWidgets;
        std::vector<std::shared_ptr<Visual>> mVisuals;

        bool mTransformIsDirty = true;

        bool mWidgetInvalidated = true;

        bool mEnabled = true;

        /**
        * Marks the absolute transform as dirty.
        * This is called when doing transformations on the widget.
        */
        void setTransformDirty();

        void addVisual(std::shared_ptr<Visual> visual);

        WidgetTree* GetWidgetTree();

        inline World* GetWorld() { return mWorld; }

    public:
        Widget();
        virtual ~Widget();

        virtual void Start();

        virtual void Tick(float inDeltaTime);

        void addWidget(std::shared_ptr<Widget> widget);
        void removeWidget(Widget* widget);
        void AddPopupWidget(std::shared_ptr<Widget> widget);
        void RemovePopupWidget(Widget* widget);

        /*
        * Gets the absolute (screen space) rect of this Widget's transform.
        **/
        WidgetRect getAbsoluteRect();

        void SetWidgetTransform(const WidgetTransform& transform);

        inline WidgetTransform GetWidgetTransform() const { return mTransform; }

        inline bool IsEnabled() const { return mEnabled; }

        virtual void OnInputEvent(InputEvent event) {}

        const std::vector<std::shared_ptr<Widget>>& GetChildren() { return mChildWidgets; }

        void SetEnabled(bool enabled);
    };
}
