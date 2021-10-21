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

    public:
        TextWidget();
        ~TextWidget();

        void SetText(const std::wstring text);
        void SetFontFace(FontFace* fontFace);
    };
}
