#pragma once
#include "widget.h"
#include "glm/vec4.hpp"
#include <memory>

namespace Ming3D
{
    class ImageWidget;
    class Texture;

    class ButtonWidget : public Widget
    {
    private:
        std::shared_ptr<ImageWidget> mImageWidget;

    public:
        ButtonWidget();
        virtual ~ButtonWidget();

        std::shared_ptr<ImageWidget> GetImageWidget() { return mImageWidget; }

        void SetColour(glm::vec4 colour);
        void SetTexture(std::shared_ptr<Texture> texture);
    };
}