#pragma once
#include "widget_transform.h"
#include <vector>
#include "visual.h"

namespace Ming3D
{
    /**
    * Base class for add Widgets.
    */
    class Widget
    {
        friend class WidgetTree;

    protected:
        /* The local transform of this widget. Is relative to parent Widget. */
        WidgetTransform mTransform;

        /* The absolute (screen space) rect of this Widget's transform. */
        WidgetRect mAbsoluteRect;

        Widget* mParentWidget = nullptr;

        std::vector<Widget*> mChildWidgets;
        std::vector<Visual*> mVisuals;

        bool mTransformIsDirty = true;

        bool mWidgetInvalidated = true;

        /**
        * Marks the absolute transform as dirty.
        * This is called when doing transformations on the widget.
        */
        void setTransformDirty();

        void addVisual(Visual* visual);

    public:
        Widget();
        virtual ~Widget();

        void addWidget(Widget* widget);

        /*
        * Gets the absolute (screen space) rect of this Widget's transform.
        **/
        WidgetRect getAbsoluteRect();

        void setPosition(glm::vec2 arg_pos);
        void setPosition(glm::vec2 arg_pos, const WidgetSizeMode arg_mode);
        void setSize(glm::vec2 arg_size);
        void setSize(glm::vec2 arg_size, const WidgetSizeMode arg_mode);
        void setPivot(glm::vec2 arg_pivot);

        inline void setPosition(const float& x, const float& y) { setPosition(glm::vec2(x, y)); }
        inline void setPosition(const float& x, const float& y, const WidgetSizeMode arg_mode) { setPosition(glm::vec2(x, y), arg_mode); }
        inline void setSize(const float& w, const float& h) { setSize(glm::vec2(w, h)); }
        inline void setSize(const float& w, const float& h, const WidgetSizeMode arg_mode) { setSize(glm::vec2(w, h), arg_mode); }
        inline void setPivot(const float& x, const float& y) { setPivot(glm::vec2(x, y)); }

        void setVerticalPositioning(WidgetSizeMode arg_mode);
        void setHorizontalPositioning(WidgetSizeMode arg_mode);
        void setVerticalScaling(WidgetSizeMode arg_mode);
        void setHorizontalScaling(WidgetSizeMode arg_mode);
    };
}
