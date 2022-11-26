#include "button_widget.h"
#include "image_widget.h"

namespace Ming3D
{
    ButtonWidget::ButtonWidget()
    {
        mImageWidget = std::make_shared<ImageWidget>();
        mImageWidget->setPosition(0.0f, 0.0f, WidgetSizeMode::Relative);
        mImageWidget->setSize(1.0f, 1.0f, WidgetSizeMode::Relative);
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
}
