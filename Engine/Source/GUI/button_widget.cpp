#include "button_widget.h"
#include "image_widget.h"
#include <iostream>

IMPLEMENT_CLASS(Ming3D::ButtonWidget)

namespace Ming3D
{
    void ButtonWidget::InitialiseClass()
    {
        
    }

    ButtonWidget::ButtonWidget()
    {
        mImageWidget = std::make_shared<ImageWidget>();
        WidgetTransform imageTransform{};
        imageTransform.anchorMin = glm::vec2(0.0f, 0.0f);
        imageTransform.anchorMax = glm::vec2(1.0f, 1.0f);
        mImageWidget->SetWidgetTransform(imageTransform);
        this->addWidget(mImageWidget);
    }

    ButtonWidget::~ButtonWidget()
    {
    }

    void ButtonWidget::SetColour(glm::vec4 colour)
    {
        mImageWidget->SetColour(colour);
    }

    void ButtonWidget::SetTexture(std::shared_ptr<Texture> texture)
    {
        mImageWidget->SetTexture(texture);
    }

    void ButtonWidget::OnInputEvent(InputEvent event)
    {
        Widget::OnInputEvent(event);
        
        if (event.mType == InputEventType::MouseButtonUp)
        {
            mOnClick();
            std::cout << "Mouse button down: " << event.mMouseButton.mButton << std::endl;
        }
    }
}
