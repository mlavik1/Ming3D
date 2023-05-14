#pragma once
#include "widget.h"
#include "glm/vec4.hpp"
#include <memory>
#include <functional>

namespace Ming3D
{
    class ImageWidget;
    class TextWidget;
    class Texture;

    class ButtonWidget : public Widget
    {
        DEFINE_CLASS(Ming3D::ButtonWidget, Ming3D::Widget)

    private:
        static void InitialiseClass();

    private:
        std::shared_ptr<ImageWidget> mImageWidget;
        std::shared_ptr<TextWidget> mTextWidget;

        void EnsureImageWidgetExists();
        void EnsureTextWidgetExists();

    public:
        ButtonWidget();
        virtual ~ButtonWidget();

        std::function<void()> mOnClick; // TODO: Multiple?

        std::shared_ptr<ImageWidget> GetImageWidget() { return mImageWidget; }
        std::shared_ptr<TextWidget> GetTextWidget() { return mTextWidget; }

        void SetColour(glm::vec4 colour);
        void SetTexture(std::shared_ptr<Texture> texture);
        void SetText(const std::string& text);

        virtual void OnInputEvent(InputEvent event) override;
    };
}
