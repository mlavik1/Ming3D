#include "widget.h"

namespace Ming3D
{
    Widget::Widget()
    {
    }

    Widget::~Widget()
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
}
