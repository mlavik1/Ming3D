#include "widget.h"
#include <string>
#include "font_face.h"

namespace Ming3D
{
    class TextVisual;
    class Texture;

    class TextWidget : public Widget
    {
    private:
        TextVisual* mTextVisual;
        FontFace* mFontFace;

    public:
        TextWidget();
        ~TextWidget();

        void SetText(const std::wstring text);
        void SetFont(const std::string font, const int fontSize);
        void SetColour(glm::vec4 colour);
    };
}
