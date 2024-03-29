#ifndef MING3D_INPUTEVENT_H
#define MING3D_INPUTEVENT_H

#include "keycodes.h"
#include "glm/vec2.hpp"

namespace Ming3D
{
    enum class InputEventType
    {
        KeyDown,
        KeyUp,
        MouseButtonDown,
        MouseButtonUp,
        MouseEnter,
        MouseLeave,
        MouseMove,
        Axis2D
    };

    struct KeyboardEvent
    {
        KeyCode mKeyCode;
    };

    struct MouseButtonEvent
    {
        int mButton = 0;
    };

    struct Axis2DEvent
    {
        EAxis2D mAxis;
        glm::vec2 mValue;
    };

    struct InputEvent
    {
        InputEventType mType; // common for all
        KeyboardEvent mKey;
        MouseButtonEvent mMouseButton;
        Axis2DEvent mAxis;
        glm::ivec2 mMousePosition;
        // TODO: Use std::variant or union?
    };
}

#endif
