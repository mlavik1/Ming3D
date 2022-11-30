#pragma once
#include "widget.h"
#include "glm/vec4.hpp"
#include <memory>

namespace Ming3D
{
    class ImageVisual;
    class Texture;

    class ImageWidget : public Widget
    {
        DEFINE_CLASS(Ming3D::ImageWidget, Ming3D::Widget)

    private:
        static void InitialiseClass();

    private:
        std::shared_ptr<ImageVisual> mImageVisual;

    public:
        ImageWidget();
        ~ImageWidget();

        void SetColour(glm::vec4 colour);
        void SetTexture(std::shared_ptr<Texture> texture);
    };
}
