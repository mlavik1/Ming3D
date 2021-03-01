#include "image_widget.h"
#include "image_visual.h"
#include "Texture/texture.h"

namespace Ming3D
{
    ImageWidget::ImageWidget()
    {
        mImageVisual = new ImageVisual();
        mVisuals.push_back(mImageVisual);
    }

    ImageWidget::~ImageWidget()
    {

    }

    void ImageWidget::SetColour(glm::vec4 colour)
    {
        mImageVisual->SetColour(colour);
    }

    void ImageWidget::SetTexture(Texture* texture)
    {
        mImageVisual->SetTexture(texture);
    }
}
