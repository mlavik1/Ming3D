#include "widget_transform.h"

namespace Ming3D
{
    WidgetTransform::WidgetTransform()
    {
        mPosition = glm::vec2(0.0f, 0.0f);
        mSize = glm::vec2(0.0f, 0.0f);
        mPivot = glm::vec2(0.0f, 0.0f);

        mVerticalPositioning = WidgetSizeMode::Relative;
        mHorizontalPositioning = WidgetSizeMode::Relative;

        mVerticalScaling = WidgetSizeMode::Relative;
        mHorizontalScaling = WidgetSizeMode::Relative;

    }

    WidgetRect WidgetTransform::createAbsoluteWidgetRect(const std::optional<WidgetRect> parentRect)
    {
        WidgetRect absoluteRect;
        absoluteRect.mPosition = mPosition;
        absoluteRect.mSize = mSize;

        // Am I the transform of a root widget
        if (!parentRect.has_value())
        {
            absoluteRect.mPosition = mPosition;
            absoluteRect.mSize = mSize;
            return absoluteRect;
        }

        const glm::vec2 parentPos = parentRect->mPosition;
        const glm::vec2 parentSize = parentRect->mSize;

        // Horizontal positioning
        if (mHorizontalPositioning == WidgetSizeMode::Relative)
            absoluteRect.mPosition.x = (parentPos.x + parentSize.x) * mPosition.x + parentPos.x * (1.0f - mPosition.x);
        else
            absoluteRect.mPosition.x = parentPos.x + mPosition.x;
        // Vertical positioning
        if (mVerticalPositioning == WidgetSizeMode::Relative)
            absoluteRect.mPosition.y = (parentPos.y + parentSize.y) * mPosition.y + parentPos.y * (1.0f - mPosition.y);
        else
            absoluteRect.mPosition.y = parentPos.y + mPosition.y;
        // Horizontal scaling
        if (mHorizontalScaling == WidgetSizeMode::Relative)
            absoluteRect.mSize.x = parentSize.x * mSize.x;
        // Vertical scaling
        if (mVerticalScaling == WidgetSizeMode::Relative)
            absoluteRect.mSize.y = parentSize.y * mSize.y;

        const glm::vec2 absPivotPos = (absoluteRect.mPosition + absoluteRect.mSize) * mPivot + absoluteRect.mPosition * (1.0f - mPivot);
        absoluteRect.mPosition -= (absPivotPos - absoluteRect.mPosition);

        return absoluteRect;
    }

}
