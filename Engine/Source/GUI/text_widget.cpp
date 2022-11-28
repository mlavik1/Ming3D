#include "text_widget.h"
#include "text_visual.h"
#include "GameEngine/game_engine.h"
#include "GUI/font_manager.h"

namespace Ming3D
{
    TextWidget::TextWidget()
    {
        mTextVisual = std::make_shared<TextVisual>();
        mVisuals.push_back(mTextVisual);
    }

    TextWidget::~TextWidget()
    {

    }

    void TextWidget::SetText(const std::wstring& text)
    {
        mTextVisual->SetText(text);
    }

    void TextWidget::SetFont(const std::string& font, const int fontSize)
    {
        // TODO: Find a smarter way of detecting if the font atlas will become too large.
        const int MAX_FONT_SIZE = 20;
        const int actualFontSize = std::min(fontSize, MAX_FONT_SIZE);
        mFontFace = GGameEngine->GetFontManager()->GetFontFace(font, actualFontSize);
        mTextVisual->SetFontFace(mFontFace);
        mTextVisual->SetFontScale(fontSize / fontSize);
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
