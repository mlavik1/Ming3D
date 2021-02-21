#pragma once

#include "widget_transform.h"

namespace Ming3D
{
    class WidgetUpdateParams
    {
    public:
        /**
        * Defines the position and extent of the content to be rendered.
        * This is decided by the transform of the widget.
        */
        WidgetRect mContentRect;

        /**
        * Defines visible region of the content to be rendered.
        * This is decided by the transform of the parent widget.
        */
        WidgetRect mVisibleRect;

        /**
        * True, if the Visuals needs to recreate its render data.
        */
        bool mVisualsInvalidated;
    };
}
