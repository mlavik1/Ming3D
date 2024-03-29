#pragma once
#include "widget.h"
#include <string>
#include <memory>
#include "font_face.h"
#include "text_properties.h"

namespace Ming3D
{
    class TextVisual;
    class Texture;

    class TextWidget : public Widget
    {
        DEFINE_CLASS(Ming3D::TextWidget, Ming3D::Widget)

    private:
        static void InitialiseClass();
        std::string mText;
        std::string mFont; // TODO: Font asset?

    private:
        std::shared_ptr<TextVisual> mTextVisual;
        FontFace* mFontFace;

    public:
        TextWidget();
        ~TextWidget();

        void SetText(const std::string& text);
        void SetFont(const std::string& font, const int fontSize);
        void SetFontSize(const int fontSize);
        void SetColour(glm::vec4 colour);
        void SetHorizontalAlignment(EHorizontalAlignment alignment);
        void SetVerticalAlignment(EVerticalAlignment alignment);
        std::string GetText() const;
    };
}
