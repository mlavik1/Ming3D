#include "image_widget.h"
#include "image_visual.h"
#include "Texture/texture.h"

IMPLEMENT_CLASS(Ming3D::ImageWidget)

namespace Ming3D
{
    void ImageWidget::InitialiseClass()
    {
        
    }

    ImageWidget::ImageWidget()
    {
        mImageVisual = std::make_shared<ImageVisual>();
        mVisuals.push_back(mImageVisual);
    }

    ImageWidget::~ImageWidget()
    {

    }

    void ImageWidget::SetColour(glm::vec4 colour)
    {
        mImageVisual->SetColour(colour);
    }

    void ImageWidget::SetTexture(std::shared_ptr<Texture> texture)
    {
        mImageVisual->SetTexture(texture);
    }
}
