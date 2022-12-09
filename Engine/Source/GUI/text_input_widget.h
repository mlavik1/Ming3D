#pragma once
#include "widget.h"
#include <string>
#include <memory>
#include "font_face.h"
#include "text_properties.h"

namespace Ming3D
{
    class ImageWidget;
    class TextWidget;
    class Texture;

    struct TextInputStyle
    {
        glm::vec4 mBorderColour;
        glm::vec4 mBackgroundColour;
    };

    class TextInputWidget : public Widget
    {
        DEFINE_CLASS(Ming3D::TextInputWidget, Ming3D::Widget)

    private:
        static void InitialiseClass();

    private:
        std::shared_ptr<ImageWidget> mBorderWidget;
        std::shared_ptr<ImageWidget> mBackgroundWidget;
        std::shared_ptr<TextWidget> mTextWidget;
        std::string mText;
        TextInputStyle mDefaultStyle;
        TextInputStyle mHoveredStyle;
        bool mIsHovered = false;

        void UpdateStyle();

    public:
        TextInputWidget();
        ~TextInputWidget();

        void SetText(const std::string& text);
        void SetFont(const std::string& font, const int fontSize);
        void SetFontSize(const int fontSize);
        void SetTextColour(glm::vec4 colour);
        void SetHorizontalAlignment(EHorizontalAlignment alignment);
        void SetVerticalAlignment(EVerticalAlignment alignment);
        void SetDefaultStyle(TextInputStyle style);
        void SetHoveredStyle(TextInputStyle style);
        std::string GetText() const;

        virtual void OnInputEvent(InputEvent event) override;
    };
}
