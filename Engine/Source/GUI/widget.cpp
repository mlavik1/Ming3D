#include "widget.h"

namespace Ming3D
{
    Widget::Widget()
    {
    }

    Widget::~Widget()
    {
        for (Widget* child : mChildWidgets)
            delete child;
    }

    void Widget::setTransformDirty()
    {
        mTransformIsDirty = true;
        mWidgetInvalidated = true;

        for (Widget* child : mChildWidgets)
            child->setTransformDirty();
    }

    void Widget::addVisual(Visual* visual)
    {
        mVisuals.push_back(visual);
        mWidgetInvalidated = true;
    }

    void Widget::addWidget(Widget* widget)
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

    void Widget::setPosition(glm::vec2 arg_pos)
    {
        mTransform.mPosition = arg_pos;
        setTransformDirty();
    }

    void Widget::setPosition(glm::vec2 arg_pos, const WidgetSizeMode arg_mode)
    {
        mTransform.mPosition = arg_pos;
        mTransform.mVerticalPositioning = arg_mode;
        mTransform.mHorizontalPositioning = arg_mode;
        setTransformDirty();
    }

    void Widget::setSize(glm::vec2 arg_size)
    {
        mTransform.mSize = arg_size;
        setTransformDirty();
    }

    void Widget::setSize(glm::vec2 arg_size, const WidgetSizeMode arg_mode)
    {
        mTransform.mSize = arg_size;
        mTransform.mVerticalScaling = arg_mode;
        mTransform.mHorizontalScaling = arg_mode;
        setTransformDirty();
    }

    void Widget::setPivot(glm::vec2 arg_pivot)
    {
        mTransform.mPivot = arg_pivot;
        setTransformDirty();
    }

    void Widget::setVerticalPositioning(WidgetSizeMode arg_mode)
    {
        mTransform.mVerticalPositioning = arg_mode;
        setTransformDirty();
    }

    void Widget::setHorizontalPositioning(WidgetSizeMode arg_mode)
    {
        mTransform.mHorizontalPositioning = arg_mode;
        setTransformDirty();
    }

    void Widget::setVerticalScaling(WidgetSizeMode arg_mode)
    {
        mTransform.mVerticalScaling = arg_mode;
        setTransformDirty();
    }

    void Widget::setHorizontalScaling(WidgetSizeMode arg_mode)
    {
        mTransform.mHorizontalScaling = arg_mode;
        setTransformDirty();
    }
}
