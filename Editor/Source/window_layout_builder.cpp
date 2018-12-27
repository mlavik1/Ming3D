#include "window_layout_builder.h"

#include "button.h"
#include <typeinfo>
#include "Actors/transform.h"
#include <algorithm>

namespace Ming3D
{
    WindowLayoutBuilder::WindowLayoutBuilder(NativeUI::Control* inControl)
    {
        mParentControl = inControl;
    }

    void WindowLayoutBuilder::Space()
    {
        mCurrHeight += vertPadding;
    }

    ctrl_id WindowLayoutBuilder::GetControlKeyHash(int typeHash, int verticalIndex, int horizontalIndex)
    {
        return ((uint32_t)typeHash << 32) | ((uint32_t)verticalIndex << 16) | (uint32_t)horizontalIndex;
    }

    NativeUI::Control* WindowLayoutBuilder::GetControl(ctrl_id controlKeyHash)
    {
        auto ctrlIter = mControls.find(controlKeyHash);
        if (ctrlIter != mControls.end())
            return ctrlIter->second;
        else
            return nullptr;
    }

    std::string WindowLayoutBuilder::TextFieldInternal(std::string inText, uint64_t controlKeyHash, bool readOnly, const EditorControlStyle &inStyle)
    {
        // Create control if it doesn't exist
        NativeUI::TextBox* txtBox = (NativeUI::TextBox*)GetControl(controlKeyHash);
        const bool ctrlExists = txtBox != nullptr;
        if (!ctrlExists)
        {
            txtBox = new NativeUI::TextBox(mParentControl);
            txtBox->SetVerticalSizeMode(NativeUI::SizeMode::Absolute);
            txtBox->SetVerticalPositionMode(NativeUI::SizeMode::Absolute);
            txtBox->SetHorizontalSizeMode(NativeUI::SizeMode::Relative);
            txtBox->SetHorizontalPositionMode(NativeUI::SizeMode::Relative);
            txtBox->SetSize(1.0f, ctrlHeight);
            txtBox->SetPosition(0.0f, mCurrHeight);
            txtBox->SetText(inText.c_str());
            txtBox->SetFontSize(inStyle.fontSize);
            txtBox->SetReadonly(readOnly);
            mControls[controlKeyHash] = txtBox;
            CachedControlData cachedData;
            cachedData.stringValue = inText;
            mControlDataCache[controlKeyHash] = cachedData;
        }

        if (mHorizontalMode)
            mQueueHorizontalControls.push_back(txtBox); // TODO: Optimise
        else
            mCurrHeight += ctrlHeight;
        
        const EditorControlStyle oldStyle = mControlDataCache[controlKeyHash].style;
        if (oldStyle.fontSize != inStyle.fontSize)
            txtBox->SetFontSize(inStyle.fontSize);
        mControlDataCache[controlKeyHash].style = inStyle; // TODO

        // Modify existing control
        if(ctrlExists)
        {
            mControlsToUpdate.erase(controlKeyHash);

            // TODO: update position and size if needed

            // TODO: Use callback and set "changedByUser" bool instead (faster than string comparison)
            const bool updatedByUser = mControlDataCache[controlKeyHash].stringValue != txtBox->GetText();
            if (updatedByUser)
            {
                const std::string newText = txtBox->GetText();
                mControlDataCache[controlKeyHash].stringValue = newText;
                return newText;
            }
            else if(mControlDataCache[controlKeyHash].stringValue != inText)
            {
                txtBox->SetText(inText);
                mControlDataCache[controlKeyHash].stringValue = inText;
            }
        }
        return inText;
    }

    bool WindowLayoutBuilder::BoolField(bool inValue, const EditorControlStyle &inStyle)
    {
        return false; // TODO
    }

    bool WindowLayoutBuilder::BoolField(bool inValue)
    {
        return BoolField(inValue, mDefaultStyle);
    }

    int WindowLayoutBuilder::IntField(int inValue, const EditorControlStyle &inStyle)
    {
        int controlKeyHash = GetControlKeyHash(MING3D_FLOATFIELD_HASH, mCurrVertIndex, mCurrHorIndex);

        std::string newStrVal = TextFieldInternal(std::to_string(inValue), controlKeyHash, false, inStyle);

        mCurrVertIndex++; // TODO: handle horizontal mode

        return std::strtol(newStrVal.c_str(), 0, 0);
    }

    int WindowLayoutBuilder::IntField(int inValue)
    {
        return IntField(inValue, mDefaultStyle);
    }

    float WindowLayoutBuilder::FloatField(float inValue, const EditorControlStyle &inStyle)
    {
        // TODO: if(CachedControlData.updatedByUser || inVal != CachedControlData.floatValue)

        int controlKeyHash = GetControlKeyHash(MING3D_FLOATFIELD_HASH, mCurrVertIndex, mCurrHorIndex);

        std::string newStrVal = TextFieldInternal(std::to_string(inValue), controlKeyHash, false, inStyle);

        mCurrVertIndex++; // TODO: handle horizontal mode

        return std::strtof(newStrVal.c_str(), 0);
    }

    float WindowLayoutBuilder::FloatField(float inValue)
    {
        return FloatField(inValue, mDefaultStyle);
    }

    std::string WindowLayoutBuilder::TextField(const std::string& inText, const EditorControlStyle &inStyle)
    {
        int controlKeyHash = GetControlKeyHash(MING3D_FLOATFIELD_HASH, mCurrVertIndex, mCurrHorIndex);

        std::string newStr = TextFieldInternal(inText, controlKeyHash, false, inStyle);

        mCurrVertIndex++; // TODO: handle horizontal mode

        return newStr;
    }

    std::string WindowLayoutBuilder::TextField(const std::string& inText)
    {
        return TextField(inText, mDefaultStyle);
    }

    void WindowLayoutBuilder::LabelField(const std::string& inText, const EditorControlStyle &inStyle)
    {
        int controlKeyHash = GetControlKeyHash(MING3D_FLOATFIELD_HASH, mCurrVertIndex, mCurrHorIndex);

        TextFieldInternal(inText, controlKeyHash, true, inStyle);
        mCurrVertIndex++; // TODO: handle horizontal mode
    }

    void WindowLayoutBuilder::LabelField(const std::string& inText)
    {
        return LabelField(inText, mDefaultStyle);
    }

    bool WindowLayoutBuilder::Button(const std::string& inText, const EditorControlStyle &inStyle)
    {
        int controlKeyHash = GetControlKeyHash(MING3D_BUTTON_HASH, mCurrVertIndex, mCurrHorIndex);

        // Create control if it doesn't exist
        NativeUI::Button* btn = (NativeUI::Button*)GetControl(controlKeyHash);
        const bool ctrlExists = btn != nullptr;

        //const EditorControlStyle oldStyle = mControlDataCache[controlKeyHash].style;
        //if (oldStyle.fontSize != inStyle.fontSize)
        //    btn->SetFontSize(inStyle.fontSize);
        mControlDataCache[controlKeyHash].style = inStyle; // TODO

        if (!ctrlExists)
        {
            // TODO: Reuse existing controls when possible

            btn = new NativeUI::Button(mParentControl);
            btn->SetVerticalSizeMode(NativeUI::SizeMode::Absolute);
            btn->SetVerticalPositionMode(NativeUI::SizeMode::Absolute);
            btn->SetHorizontalSizeMode(NativeUI::SizeMode::Relative);
            btn->SetHorizontalPositionMode(NativeUI::SizeMode::Relative);
            btn->SetSize(1.0f, ctrlHeight);
            btn->SetPosition(0.0f, mCurrHeight);
            btn->SetText(inText.c_str());
            //btn->SetFontSize(inFontSize);
            mControls[controlKeyHash] = btn;
            CachedControlData cachedData;
            cachedData.stringValue = inText;
            mControlDataCache[controlKeyHash] = cachedData;

            btn->RegisterClickCallback([&, controlKeyHash]() { mControlDataCache[controlKeyHash].modifiedByUser = true; });
        }

        if (mHorizontalMode)
            mQueueHorizontalControls.push_back(btn); // TODO: Optimise
        else
            mCurrHeight += ctrlHeight;

        // Modify existing control
        if(ctrlExists)
        {
            mControlsToUpdate.erase(controlKeyHash);

            // TODO: update position and size if needed
            
            const bool userClicked = mControlDataCache[controlKeyHash].modifiedByUser;
            if (userClicked)
            {
                mControlDataCache[controlKeyHash].modifiedByUser = false;
                return true;
            }
        }
        return false;
    }

    bool WindowLayoutBuilder::Button(const std::string& inText)
    {
        return Button(inText, mDefaultStyle);
    }

    void WindowLayoutBuilder::PropertyField(Property* inProp, Object* inObject, std::string inName)
    {
        const std::string typeName = inProp->GetTypeName();
        const void* valPtr = inProp->GetPropertyHandle()->GetValuePtr(inObject);
        if (typeName == typeid(std::string).name())
        {
            BeginHorizontal();
            std::string strHeader = inProp->GetPropertyName() + ":";
            std::string* stringPtr = (std::string*)valPtr;
            LabelField(strHeader);
            std::string newValStr = TextField(*stringPtr);
            *(std::string*)valPtr = newValStr;
            EndHorizontal();
            mCurrHeight += vertPadding;
        }
        else if (typeName == typeid(Transform).name())
        {
            std::string strHeader = inProp->GetPropertyName() + ":";
            LabelField(strHeader);
            
            Transform* transform = (Transform*)valPtr;
            glm::vec3 localPos = transform->GetLocalPosition();
            glm::vec3 localScale = transform->GetLocalScale();
            glm::vec3 localRot = glm::eulerAngles(transform->GetLocalRotation());

            BeginHorizontal();
            LabelField("Position");
            localPos.x = FloatField(localPos.x);
            localPos.y = FloatField(localPos.y);
            localPos.z = FloatField(localPos.z);
            EndHorizontal();
            BeginHorizontal();
            LabelField("Scale");
            localScale.x = FloatField(localScale.x);
            localScale.y = FloatField(localScale.y);
            localScale.z = FloatField(localScale.z);
            EndHorizontal();
            BeginHorizontal();
            LabelField("Rotation");
            localRot.x = FloatField(localRot.x);
            localRot.y = FloatField(localRot.y);
            localRot.z = FloatField(localRot.z);
            EndHorizontal();
            mCurrHeight += vertPadding;

            transform->SetLocalPosition(localPos);
            transform->SetLocalScale(localScale);
            transform->SetLocalRotation(localRot);
        }
    }

    void WindowLayoutBuilder::BeginHorizontal()
    {
        mHorizontalMode = true;
    }

    void WindowLayoutBuilder::EndHorizontal()
    {
        mHorizontalMode = false;
        const size_t numCtrls = mQueueHorizontalControls.size();
        const float ctrlWidth = 1.0f / numCtrls;
        float maxHeight = 0.0f;
        for (size_t iCtrl = 0; iCtrl < numCtrls; iCtrl++)
        {
            NativeUI::Control* ctrl = mQueueHorizontalControls[iCtrl];
            ctrl->SetPosition(ctrlWidth * iCtrl, ctrl->GetPosition().y);
            ctrl->SetSize(ctrlWidth, ctrl->GetSize().y);
            maxHeight = std::max(maxHeight, ctrl->GetSize().y);
        }
        mCurrHeight += maxHeight;
        mQueueHorizontalControls.clear();
    }

    void WindowLayoutBuilder::BeginBuilding()
    {
        mCurrVertIndex = 0;
        mCurrHorIndex = 0;
        mCurrHeight = 0.0f;

        for (auto ctrlPair : mControls)
            mControlsToUpdate.insert(ctrlPair.first);
    }

    void WindowLayoutBuilder::EndBuilding()
    {
        if (mHorizontalMode)
            EndHorizontal();

        for (ctrl_id ctrlID : mControlsToUpdate)
        {
            NativeUI::Control* ctrl = mControls[ctrlID];
            mControls.erase(ctrlID);
            delete(ctrl);
        }
        mControlsToUpdate.clear();
    }
}
