#include "button_widget.h"
#include "image_widget.h"
#include "text_widget.h"
#include <iostream>

IMPLEMENT_CLASS(Ming3D::ButtonWidget)

namespace Ming3D
{
    void ButtonWidget::InitialiseClass()
    {
        
    }

    ButtonWidget::ButtonWidget()
    {
    }

    ButtonWidget::~ButtonWidget()
    {
    }

    void ButtonWidget::EnsureImageWidgetExists()
    {
        if (mImageWidget == nullptr)
        {
            mImageWidget = std::make_shared<ImageWidget>();
            WidgetTransform imageTransform{};
            imageTransform.anchorMin = glm::vec2(0.0f, 0.0f);
            imageTransform.anchorMax = glm::vec2(1.0f, 1.0f);
            mImageWidget->SetWidgetTransform(imageTransform);
            this->addWidget(mImageWidget);
        }
    }

    void ButtonWidget::EnsureTextWidgetExists()
    {
        if (mTextWidget == nullptr)
        {
            mTextWidget = std::make_shared<TextWidget>();
            WidgetTransform textTransform{};
            textTransform.anchorMin = glm::vec2(0.0f, 0.0f);
            textTransform.anchorMax = glm::vec2(1.0f, 1.0f);
            mTextWidget->SetWidgetTransform(textTransform);
            this->addWidget(mTextWidget);
        }
    }

    void ButtonWidget::SetColour(glm::vec4 colour)
    {
        EnsureImageWidgetExists();
        mImageWidget->SetColour(colour);
    }

    void ButtonWidget::SetTexture(std::shared_ptr<Texture> texture)
    {
        EnsureImageWidgetExists();
        mImageWidget->SetTexture(texture);
    }

    void ButtonWidget::SetText(const std::string& text)
    {
        EnsureTextWidgetExists();
        mTextWidget->SetText(text);
    }


    void ButtonWidget::OnInputEvent(InputEvent event)
    {
        Widget::OnInputEvent(event);
        
        if (mOnClick != nullptr && event.mType == InputEventType::MouseButtonUp)
        {
            mOnClick();
            std::cout << "Mouse button down: " << event.mMouseButton.mButton << std::endl;
        }
    }
}
