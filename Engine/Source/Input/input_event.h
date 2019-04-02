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
        MouseButtonUp
    };

    struct KeyboardEvent
    {
        InputEventType mType;
        KeyCode mKeyCode;
    };

    struct MouseButtonEvent
    {
        InputEventType mType;
        MouseButton mButton;
    };

    typedef union InputEvent
    {
        InputEventType mType; // common for all
        KeyboardEvent mKey;
        MouseButtonEvent mMouseButton;
    };
}

#endif
