#pragma once
#include "glm/vec2.hpp"
#include <optional>

namespace Ming3D
{
    enum class WidgetSizeMode
    {
        Absolute,
        Relative
    };

    class WidgetRect
    {
    public:
        glm::vec2 mPosition;
        glm::vec2 mSize;

        bool Contains(glm::vec2 point) const
        {
            return point.x >= mPosition.x && point.x <= mPosition.x + mSize.x &&
             point.y >= mPosition.y && point.y <= mPosition.y + mSize.y;
        }
    };

    class WidgetTransform
    {
    public:
        glm::vec2 anchorMin = glm::vec2(0.0f, 0.0f);
        glm::vec2 anchorMax = glm::vec2(1.0f, 1.0f);
        glm::vec2 mPosition = glm::vec2(0.0f, 0.0f);
        glm::vec2 mSize = glm::vec2(0.0f, 0.0f);
        glm::vec2 mPivot = glm::vec2(0.0f, 0.0f);

    public:
        WidgetTransform();

        /**
        * Creates an absolute WidgetTransform from this (local) WidgetTransform.
        */
        WidgetRect createAbsoluteWidgetRect(const std::optional<WidgetRect> parentRect);

    };
}
