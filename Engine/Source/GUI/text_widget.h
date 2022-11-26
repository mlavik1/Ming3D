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
    private:
        std::shared_ptr<TextVisual> mTextVisual;
        FontFace* mFontFace;

    public:
        TextWidget();
        ~TextWidget();

        void SetText(const std::wstring& text);
        void SetFont(const std::string& font, const int fontSize);
        void SetColour(glm::vec4 colour);
        void SetHorizontalAlignment(EHorizontalAlignment alignment);
        void SetVerticalAlignment(EVerticalAlignment alignment);
    };
}
