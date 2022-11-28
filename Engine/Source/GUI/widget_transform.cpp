#include "widget_transform.h"
#include <cmath>

namespace Ming3D
{
    WidgetTransform::WidgetTransform()
    {

    }

    glm::vec2 lerp(glm:: vec2 a, glm::vec2 b, glm::vec2 t) // TODO: Move to math class
    {
        return glm::vec2(
            b.x * t.x + a.x * (1.0f - t.x),
            b.y * t.y + a.y * (1.0f - t.y)
        );
    }

    WidgetRect WidgetTransform::createAbsoluteWidgetRect(const std::optional<WidgetRect> parentRect)
    {
        WidgetRect absoluteRect;

        // Am I the transform of a root widget
        if (!parentRect.has_value())
        {
            absoluteRect.mPosition = mPosition;
            absoluteRect.mSize = mSize;
            return absoluteRect;
        }

        const glm::vec2 parentPos = parentRect->mPosition;
        const glm::vec2 parentSize = parentRect->mSize;
        const glm::vec2 parentMin = parentPos;
        const glm::vec2 parentMax = parentPos + parentSize;

        const glm::vec2 anchorMinPoint = lerp(parentMin, parentMax, anchorMin);
        const glm::vec2 anchorMaxPoint = lerp(parentMin, parentMax, anchorMax);
        const glm::vec2 anchorsDistance = glm::vec2(std::abs(anchorMaxPoint.x - anchorMinPoint.x), std::abs(anchorMaxPoint.y - anchorMinPoint.y));

        const glm::vec2 pivotAbsPos = anchorMinPoint + this->mPosition;
        absoluteRect.mSize = anchorsDistance + this->mSize;
        absoluteRect.mPosition = pivotAbsPos - (this->mPivot * absoluteRect.mSize);

        return absoluteRect;
    }

}
