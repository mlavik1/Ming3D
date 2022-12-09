#include "text_input_widget.h"
#include "text_widget.h"
#include "image_widget.h"

IMPLEMENT_CLASS(Ming3D::TextInputWidget)

namespace Ming3D
{
    void TextInputWidget::InitialiseClass()
    {
        
    }

    TextInputWidget::TextInputWidget()
    {
        mDefaultStyle.mBackgroundColour = glm::vec4(0.35f, 0.35, 0.35f, 1.0f);
        mDefaultStyle.mBorderColour = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
        mHoveredStyle.mBackgroundColour = glm::vec4(0.5f, 0.5f, 0.5f, 1.0f);
        mHoveredStyle.mBorderColour = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);

        mBorderWidget = std::make_shared<ImageWidget>();
        this->addWidget(mBorderWidget);
        mBackgroundWidget = std::make_shared<ImageWidget>();
        this->addWidget(mBackgroundWidget);
        mTextWidget = std::make_shared<TextWidget>();
        this->addWidget(mTextWidget);

        mTextWidget->SetColour(glm::vec4(0.7f, 0.7f, 0.7f, 1.0f));
        UpdateStyle();
    }

    TextInputWidget::~TextInputWidget()
    {

    }

    void TextInputWidget::SetText(const std::string& text)
    {
        mText = text;
        mTextWidget->SetText(text);
    }

    std::string TextInputWidget::GetText() const
    {
        return mText;
    }

    void TextInputWidget::SetFont(const std::string& font, const int fontSize)
    {
        mTextWidget->SetFont(font, fontSize);
    }

    void TextInputWidget::SetFontSize(const int fontSize)
    {
        mTextWidget->SetFontSize(fontSize);
    }

    void TextInputWidget::SetTextColour(glm::vec4 colour)
    {
        mTextWidget->SetColour(colour);
    }

    void TextInputWidget::SetHorizontalAlignment(EHorizontalAlignment alignment)
    {
        mTextWidget->SetHorizontalAlignment(alignment);
    }

    void TextInputWidget::SetVerticalAlignment(EVerticalAlignment alignment)
    {
        mTextWidget->SetVerticalAlignment(alignment);
    }

    void TextInputWidget::SetDefaultStyle(TextInputStyle style)
    {

    }

    void TextInputWidget::SetHoveredStyle(TextInputStyle style)
    {

    }

    void TextInputWidget::OnInputEvent(InputEvent event)
    {
        Widget::OnInputEvent(event);
        
        if (event.mType == InputEventType::MouseEnter)
        {
            mIsHovered = true;
            UpdateStyle();
        }
        else if (event.mType == InputEventType::MouseLeave)
        {
            mIsHovered = false;
            UpdateStyle();
        }
    }

    void TextInputWidget::UpdateStyle()
    {
        TextInputStyle style = mIsHovered ? mHoveredStyle : mDefaultStyle;
        mBorderWidget->SetColour(style.mBorderColour);
        mBackgroundWidget->SetColour(style.mBackgroundColour);
    }
}
