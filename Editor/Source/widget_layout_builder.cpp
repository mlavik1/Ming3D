#include "widget_layout_builder.h"
#include "Actors/transform.h"
#include "GUI/button_widget.h"
#include "GUI/text_widget.h"
#include "GUI/image_widget.h"
#include "GUI/text_input_widget.h"
#include <typeinfo>
#include <algorithm>

namespace Ming3D
{
    WidgetLayoutBuilder::WidgetLayoutBuilder(Widget* containerWidget)
    {
        mContainerWidget = containerWidget;
    }

    void WidgetLayoutBuilder::Space()
    {
        mCurrHeight += mVertPadding;
    }

    widget_id WidgetLayoutBuilder::GetWidgetKeyHash(int typeHash, int verticalIndex, int horizontalIndex)
    {
        return ((widget_id)typeHash << 32) | ((widget_id)verticalIndex << 16) | (widget_id)horizontalIndex;
    }

    std::shared_ptr<Widget> WidgetLayoutBuilder::GetWidget(widget_id controlKeyHash)
    {
        auto ctrlIter = mWidgets.find(controlKeyHash);
        if (ctrlIter != mWidgets.end())
            return ctrlIter->second;
        else
            return nullptr;
    }

    void WidgetLayoutBuilder::LabelInternal(std::string text, uint64_t controlKeyHash, bool readOnly, const EditorControlStyle &style)
    {
        // Create control if it doesn't exist
        std::shared_ptr<TextWidget> textWidget = std::static_pointer_cast<TextWidget>(GetWidget(controlKeyHash));
        const bool ctrlExists = textWidget != nullptr;
        if (!ctrlExists)
        {
            textWidget = std::make_shared<TextWidget>();
            mContainerWidget->addWidget(textWidget);
            textWidget->SetText(text);
            textWidget->SetFontSize(style.fontSize);
            //textWidget->SetFont(style.fontSize); // TODO
            //textWidget->SetReadonly(readOnly); // TODO
            WidgetTransform transform{};
            transform.anchorMin = glm::vec2(0.0f, 0.0f);
            transform.anchorMax = glm::vec2(0.0f, 0.0f);
            transform.mPosition = glm::vec2(0.0f, mCurrHeight);
            transform.mSize = glm::vec2(mContainerRect.mSize.x, mWidgetheight);
            textWidget->SetWidgetTransform(transform);
            mWidgets[controlKeyHash] = textWidget;
            CachedControlData cachedData;
            cachedData.stringValue = text;
            mWidgetDataCache[controlKeyHash] = cachedData;
        }

        if (mHorizontalMode)
            mQueueHorizontalWidgets.push_back(textWidget); // TODO: Optimise
        else
            mCurrHeight += mWidgetheight;
        
        const EditorControlStyle oldStyle = mWidgetDataCache[controlKeyHash].style;
        // TODO
        //if (oldStyle.fontSize != style.fontSize)
        //    textWidget->SetFontSize(style.fontSize);
        mWidgetDataCache[controlKeyHash].style = style; // TODO

        // Modify existing control
        if(ctrlExists)
        {
            mWidgetsToUpdate.erase(controlKeyHash);

            // TODO: update position and size if needed
        }
    }

    std::string WidgetLayoutBuilder::TextFieldInternal(std::string text, uint64_t controlKeyHash, bool readOnly, const EditorControlStyle &style)
    {
        // Create control if it doesn't exist
        std::shared_ptr<TextInputWidget> textWidget = std::static_pointer_cast<TextInputWidget>(GetWidget(controlKeyHash));
        const bool ctrlExists = textWidget != nullptr;
        if (!ctrlExists)
        {
            textWidget = std::make_shared<TextInputWidget>();
            mContainerWidget->addWidget(textWidget);
            textWidget->SetText(text);
            textWidget->SetFontSize(style.fontSize);
            //textWidget->SetFont(style.fontSize); // TODO
            //textWidget->SetReadonly(readOnly); // TODO
            WidgetTransform transform{};
            transform.anchorMin = glm::vec2(0.0f, 0.0f);
            transform.anchorMax = glm::vec2(0.0f, 0.0f);
            transform.mPosition = glm::vec2(0.0f, mCurrHeight);
            transform.mSize = glm::vec2(mContainerRect.mSize.x, mWidgetheight);
            textWidget->SetWidgetTransform(transform);
            mWidgets[controlKeyHash] = textWidget;
            CachedControlData cachedData;
            cachedData.stringValue = text;
            mWidgetDataCache[controlKeyHash] = cachedData;
        }

        if (mHorizontalMode)
            mQueueHorizontalWidgets.push_back(textWidget); // TODO: Optimise
        else
            mCurrHeight += mWidgetheight;
        
        const EditorControlStyle oldStyle = mWidgetDataCache[controlKeyHash].style;
        // TODO
        //if (oldStyle.fontSize != style.fontSize)
        //    textWidget->SetFontSize(style.fontSize);
        mWidgetDataCache[controlKeyHash].style = style; // TODO

        // Modify existing control
        if(ctrlExists)
        {
            mWidgetsToUpdate.erase(controlKeyHash);

            // TODO: update position and size if needed

            // TODO: Use callback and set "changedByUser" bool instead (faster than string comparison)
            const bool updatedByUser = mWidgetDataCache[controlKeyHash].stringValue != textWidget->GetText();
            if (updatedByUser)
            {
                const std::string newText = textWidget->GetText();
                mWidgetDataCache[controlKeyHash].stringValue = newText;
                return newText;
            }
            else if(mWidgetDataCache[controlKeyHash].stringValue != text)
            {
                textWidget->SetText(text);
                mWidgetDataCache[controlKeyHash].stringValue = text;
            }
        }
        return text;
    }

    bool WidgetLayoutBuilder::BoolField(bool value, const EditorControlStyle &style)
    {
        return false; // TODO
    }

    bool WidgetLayoutBuilder::BoolField(bool value)
    {
        return BoolField(value, mDefaultStyle);
    }

    int WidgetLayoutBuilder::IntField(int value, const EditorControlStyle &style)
    {
        widget_id controlKeyHash = GetWidgetKeyHash(MING3D_FLOATFIELD_HASH, mCurrVertIndex, mCurrHorIndex);

        std::string newStrVal = TextFieldInternal(std::to_string(value), controlKeyHash, false, style);

        mCurrVertIndex++; // TODO: handle horizontal mode

        return std::strtol(newStrVal.c_str(), 0, 0);
    }

    int WidgetLayoutBuilder::IntField(int value)
    {
        return IntField(value, mDefaultStyle);
    }

    float WidgetLayoutBuilder::FloatField(float value, const EditorControlStyle &style)
    {
        // TODO: if(CachedControlData.updatedByUser || inVal != CachedControlData.floatValue)

        widget_id controlKeyHash = GetWidgetKeyHash(MING3D_FLOATFIELD_HASH, mCurrVertIndex, mCurrHorIndex);

        std::string newStrVal = TextFieldInternal(std::to_string(value), controlKeyHash, false, style);

        mCurrVertIndex++; // TODO: handle horizontal mode

        return std::strtof(newStrVal.c_str(), 0);
    }

    float WidgetLayoutBuilder::FloatField(float value)
    {
        return FloatField(value, mDefaultStyle);
    }

    std::string WidgetLayoutBuilder::TextField(const std::string& text, const EditorControlStyle &style)
    {
        widget_id controlKeyHash = GetWidgetKeyHash(MING3D_TEXTFIELD_HASH, mCurrVertIndex, mCurrHorIndex);

        std::string newStr = TextFieldInternal(text, controlKeyHash, false, style);

        mCurrVertIndex++; // TODO: handle horizontal mode

        return newStr;
    }

    std::string WidgetLayoutBuilder::TextField(const std::string& text)
    {
        return TextField(text, mDefaultStyle);
    }

    void WidgetLayoutBuilder::Label(const std::string& text, const EditorControlStyle &style)
    {
        widget_id controlKeyHash = GetWidgetKeyHash(MING3D_LABEL_HASH, mCurrVertIndex, mCurrHorIndex);

        LabelInternal(text, controlKeyHash, true, style);
        mCurrVertIndex++; // TODO: handle horizontal mode
    }

    void WidgetLayoutBuilder::Label(const std::string& text)
    {
        return Label(text, mDefaultStyle);
    }

    bool WidgetLayoutBuilder::Button(const std::string& text, const EditorControlStyle &style)
    {
        widget_id controlKeyHash = GetWidgetKeyHash(MING3D_BUTTON_HASH, mCurrVertIndex, mCurrHorIndex);

        // Create control if it doesn't exist
        std::shared_ptr<ButtonWidget> buttonWidget = std::static_pointer_cast<ButtonWidget>(GetWidget(controlKeyHash));
        const bool ctrlExists = buttonWidget != nullptr;

        //const EditorControlStyle oldStyle = mWidgetDataCache[controlKeyHash].style;
        //if (oldStyle.fontSize != inStyle.fontSize)
        //    btn->SetFontSize(inStyle.fontSize);
        mWidgetDataCache[controlKeyHash].style = style; // TODO

        if (!ctrlExists)
        {
            // TODO: Reuse existing controls when possible

            buttonWidget = std::make_shared<ButtonWidget>();
            WidgetTransform transform{};
            transform.anchorMin = glm::vec2(0.0f, 0.0f);
            transform.anchorMax = glm::vec2(0.0f, 0.0f);
            transform.mPosition = glm::vec2(0.0f, mCurrHeight);
            transform.mSize = glm::vec2(mContainerRect.mSize.x, mCurrHeight);
            buttonWidget->SetWidgetTransform(transform);
            // TODO
            //buttonWidget->SetText(text);
            //btn->SetFontSize(inFontSize);
            mWidgets[controlKeyHash] = buttonWidget;
            CachedControlData cachedData;
            cachedData.stringValue = text;
            mWidgetDataCache[controlKeyHash] = cachedData;

            buttonWidget->mOnClick = [&, controlKeyHash]() { mWidgetDataCache[controlKeyHash].modifiedByUser = true; };
        }

        if (mHorizontalMode)
            mQueueHorizontalWidgets.push_back(buttonWidget); // TODO: Optimise
        else
            mCurrHeight += mWidgetheight;

        // Modify existing control
        if(ctrlExists)
        {
            mWidgetsToUpdate.erase(controlKeyHash);

            // TODO: update position and size if needed
            
            const bool userClicked = mWidgetDataCache[controlKeyHash].modifiedByUser;
            if (userClicked)
            {
                mWidgetDataCache[controlKeyHash].modifiedByUser = false;
                return true;
            }
        }
        return false;
    }

    bool WidgetLayoutBuilder::Button(const std::string& text)
    {
        return Button(text, mDefaultStyle);
    }

    void WidgetLayoutBuilder::PropertyField(Property* property, Object* object, std::string name)
    {
        const std::string typeName = property->GetTypeName();
        const void* valPtr = property->GetPropertyHandle()->GetValuePtr(object);
        if (typeName == typeid(std::string).name())
        {
            BeginHorizontal();
            std::string strHeader = property->GetPropertyName() + ":";
            std::string* stringPtr = (std::string*)valPtr;
            Label(strHeader);
            std::string newValStr = TextField(*stringPtr);
            *(std::string*)valPtr = newValStr;
            EndHorizontal();
            mCurrHeight += mVertPadding;
        }
        else if (typeName == typeid(Transform).name())
        {
            std::string strHeader = property->GetPropertyName() + ":";
            Label(strHeader);
            
            Transform* transform = (Transform*)valPtr;
            glm::vec3 localPos = transform->GetLocalPosition();
            glm::vec3 localScale = transform->GetLocalScale();
            glm::vec3 localRot = glm::eulerAngles(transform->GetLocalRotation());

            BeginHorizontal();
            Label("Position");
            localPos.x = FloatField(localPos.x);
            localPos.y = FloatField(localPos.y);
            localPos.z = FloatField(localPos.z);
            EndHorizontal();
            BeginHorizontal();
            Label("Scale");
            localScale.x = FloatField(localScale.x);
            localScale.y = FloatField(localScale.y);
            localScale.z = FloatField(localScale.z);
            EndHorizontal();
            BeginHorizontal();
            Label("Rotation");
            localRot.x = FloatField(localRot.x);
            localRot.y = FloatField(localRot.y);
            localRot.z = FloatField(localRot.z);
            EndHorizontal();
            mCurrHeight += mVertPadding;

            transform->SetLocalPosition(localPos);
            transform->SetLocalScale(localScale);
            transform->SetLocalRotation(localRot);
        }
    }

    void WidgetLayoutBuilder::BeginHorizontal()
    {
        mHorizontalMode = true;
    }

    void WidgetLayoutBuilder::EndHorizontal()
    {
        mHorizontalMode = false;
        const size_t numWidgets = mQueueHorizontalWidgets.size();
        const float widgetWidth = mContainerRect.mSize.x / numWidgets;
        float maxHeight = 0.0f;
        for (size_t iWidget = 0; iWidget < numWidgets; iWidget++)
        {
            std::shared_ptr<Widget> widget = mQueueHorizontalWidgets[iWidget];
            WidgetTransform transform = widget->GetWidgetTransform();
            transform.mPosition.x = widgetWidth * iWidget;
            transform.mSize.x = widgetWidth;
            widget->SetWidgetTransform(transform);
            maxHeight = std::max(maxHeight, transform.mSize.y);
        }
        mCurrHeight += maxHeight;
        mQueueHorizontalWidgets.clear();
    }

    void WidgetLayoutBuilder::BeginBuilding()
    {
        mContainerRect = mContainerWidget->getAbsoluteRect();
        mCurrVertIndex = 0;
        mCurrHorIndex = 0;
        mCurrHeight = 0.0f;

        for (auto ctrlPair : mWidgets)
            mWidgetsToUpdate.insert(ctrlPair.first);
    }

    void WidgetLayoutBuilder::EndBuilding()
    {
        if (mHorizontalMode)
            EndHorizontal();

        for (widget_id ctrlID : mWidgetsToUpdate)
        {
            std::shared_ptr<Widget> widget = mWidgets[ctrlID];
            mWidgets.erase(ctrlID);
            mContainerWidget->removeWidget(widget.get());
        }
        mWidgetsToUpdate.clear();
    }
}
