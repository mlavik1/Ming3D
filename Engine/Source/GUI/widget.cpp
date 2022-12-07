#include "widget.h"
#include <algorithm>

IMPLEMENT_CLASS(Ming3D::Widget)

namespace Ming3D
{
    Widget::Widget()
    {
    }

    Widget::~Widget()
    {
    }

    void Widget::InitialiseClass()
    {
        
    }

    void Widget::setTransformDirty()
    {
        mTransformIsDirty = true;
        mWidgetInvalidated = true;

        for (std::vector<std::shared_ptr<Widget>>::iterator itrWidget = mChildWidgets.begin(); itrWidget != mChildWidgets.end(); ++itrWidget)
            (*itrWidget)->setTransformDirty();
    }

    void Widget::addVisual(std::shared_ptr<Visual> visual)
    {
        mVisuals.push_back(visual);
        mWidgetInvalidated = true;
    }

    void Widget::addWidget(std::shared_ptr<Widget> widget)
    {
        mChildWidgets.push_back(widget);
        widget->mParentWidget = this;
        mWidgetInvalidated = true;
        widget->mWorld = mWorld; // TODO ?
    }

    void Widget::removeWidget(Widget* widget)
    {
        auto itEnd = std::remove_if(mChildWidgets.begin(), mChildWidgets.end(), [widget](auto candidate){ return candidate.get() == widget; });
        mChildWidgets.erase(itEnd, mChildWidgets.end());
        mWidgetInvalidated = true;
    }

    WidgetRect Widget::getAbsoluteRect()
    {
        if (mTransformIsDirty)
        {
            if (mParentWidget != nullptr)
                mAbsoluteRect = mTransform.createAbsoluteWidgetRect(mParentWidget->getAbsoluteRect());
            else
                mAbsoluteRect = mTransform.createAbsoluteWidgetRect(std::nullopt);

            mTransformIsDirty = false;
        }

        return mAbsoluteRect; // return the re-calculated absolute rect of the widget
    }

    void Widget::SetWidgetTransform(const WidgetTransform& transform)
    {
        mTransform = transform;
        setTransformDirty();
    }

    void Widget::SetEnabled(bool enabled)
    {
        mEnabled = enabled;
        setTransformDirty();
    }

    void Widget::Start()
    {

    }

    void Widget::Tick(float inDeltaTime)
    {

    }
}
