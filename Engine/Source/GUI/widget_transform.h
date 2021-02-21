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

    struct WidgetRect
    {
        glm::vec2 mPosition;
        glm::vec2 mSize;
    };

    class WidgetTransform
    {
    public:
        glm::vec2 mPosition = glm::vec2(0.0f, 0.0f);
        glm::vec2 mSize = glm::vec2(1.0f, 1.0f);
        glm::vec2 mPivot = glm::vec2(0.0f, 0.0f);

        WidgetSizeMode mVerticalPositioning = WidgetSizeMode::Relative;
        WidgetSizeMode mHorizontalPositioning = WidgetSizeMode::Relative;

        WidgetSizeMode mVerticalScaling = WidgetSizeMode::Relative;
        WidgetSizeMode mHorizontalScaling = WidgetSizeMode::Relative;

    public:
        WidgetTransform();

        /**
        * Creates an absolute WidgetTransform from this (local) WidgetTransform.
        */
        WidgetRect createAbsoluteWidgetRect(const std::optional<WidgetRect> parentRect);

    };
}
