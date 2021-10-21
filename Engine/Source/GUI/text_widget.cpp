#include "text_widget.h"
#include "text_visual.h"

namespace Ming3D
{
    TextWidget::TextWidget()
    {
        mTextVisual = new TextVisual();
        mVisuals.push_back(mTextVisual);
    }

    TextWidget::~TextWidget()
    {

    }

    void TextWidget::SetText(const std::wstring text)
    {
        mTextVisual->SetText(text);
    }

    void TextWidget::SetFontFace(FontFace* fontFace)
    {
        mTextVisual->SetFontFace(fontFace);
    }
}
