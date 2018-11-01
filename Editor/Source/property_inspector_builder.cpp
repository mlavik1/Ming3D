#include "property_inspector_builder.h"

#include "button.h"
#include <typeinfo>

namespace Ming3D
{
    PropertyInspectorBuilder::PropertyInspectorBuilder(NativeUI::Control* inControl)
    {
        mParentControl = inControl;
    }

    void PropertyInspectorBuilder::Text(std::string inText, int inTextSize)
    {
        NativeUI::TextBox* textBox = CreateTextBox(inText, inTextSize, ctrlHeight, mCurrHeight);
        mCurrHeight += vertPadding;
    }

    void PropertyInspectorBuilder::Space()
    {
        mCurrHeight += vertPadding;
    }

    void PropertyInspectorBuilder::Button(std::string inText, std::function<void()> inCallbback)
    {
        NativeUI::Button* button = CreateButton(inText, 16, ctrlHeight, mCurrHeight);
        button->RegisterClickCallback(inCallbback);
        mCurrHeight += vertPadding;
    }

    void PropertyInspectorBuilder::PropertyField(Property* inProp, Object* inObject, std::string inName)
    {
        const std::string typeName = inProp->GetTypeName();
        const void* valPtr = inProp->GetPropertyHandle()->GetValuePtr(inObject);
        if (typeName == typeid(std::string).name())
        {
            std::string strHeader = inProp->GetPropertyName() + ":";
            std::string* stringPtr = (std::string*)valPtr;
            NativeUI::TextBox* txtHeader = CreateTextBox(strHeader, 12, ctrlHeight, mCurrHeight);
            NativeUI::TextBox* txtValue = CreateTextBox(*stringPtr, 12, ctrlHeight, mCurrHeight);
            // TODO: on text changed: set property value
            mCurrHeight += vertPadding;
        }
    }

    NativeUI::Button* PropertyInspectorBuilder::CreateButton(std::string inText, int inFontSize, float inCtrlHeight, float& outCurrHeight)
    {
        NativeUI::Button* btn = new NativeUI::Button(mParentControl);
        btn->SetVerticalSizeMode(NativeUI::SizeMode::Absolute);
        btn->SetVerticalPositionMode(NativeUI::SizeMode::Absolute);
        btn->SetHorizontalSizeMode(NativeUI::SizeMode::Relative);
        btn->SetHorizontalPositionMode(NativeUI::SizeMode::Relative);
        btn->SetSize(1.0f, inCtrlHeight);
        btn->SetPosition(0.0f, outCurrHeight);
        btn->SetText(inText.c_str());
        //btn->SetFontSize(inFontSize);
        outCurrHeight += inCtrlHeight;
        mControls.push_back(btn);
        return btn;
    }

    NativeUI::TextBox* PropertyInspectorBuilder::CreateTextBox(std::string inText, int inFontSize, float inCtrlHeight, float& outCurrHeight)
    {
        NativeUI::TextBox* txtBox = new NativeUI::TextBox(mParentControl);
        txtBox->SetVerticalSizeMode(NativeUI::SizeMode::Absolute);
        txtBox->SetVerticalPositionMode(NativeUI::SizeMode::Absolute);
        txtBox->SetHorizontalSizeMode(NativeUI::SizeMode::Relative);
        txtBox->SetHorizontalPositionMode(NativeUI::SizeMode::Relative);
        txtBox->SetSize(1.0f, inCtrlHeight);
        txtBox->SetPosition(0.0f, outCurrHeight);
        txtBox->SetText(inText.c_str());
        txtBox->SetFontSize(inFontSize);
        outCurrHeight += inCtrlHeight;
        mControls.push_back(txtBox);
        return txtBox;
    }

    void PropertyInspectorBuilder::Clear()
    {
        // Remove existing controls
        for (NativeUI::Control* ctrl : mControls)
            delete(ctrl);
        mControls.clear();
        mCurrHeight = 0.0f;
    }
}
