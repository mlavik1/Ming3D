#include "text_widget.h"
#include "text_visual.h"
#include "GameEngine/game_engine.h"
#include "GUI/font_manager.h"

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

    void TextWidget::SetFont(const std::string font, const int fontSize)
    {
        mFontFace = GGameEngine->GetFontManager()->GetFontFace(font, 14);
        mTextVisual->SetFontFace(mFontFace);
        mTextVisual->SetFontScale(fontSize / 12);
    }

    void TextWidget::SetColour(glm::vec4 colour)
    {
        mTextVisual->SetColour(colour);
    }

    void TextWidget::SetHorizontalAlignment(EHorizontalAlignment alignment)
    {
        mTextVisual->SetHorizontalAlignment(alignment);
    }

    void TextWidget::SetVerticalAlignment(EVerticalAlignment alignment)
    {
        mTextVisual->SetVerticalAlignment(alignment);
    }
}
