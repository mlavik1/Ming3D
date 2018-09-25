#include "property_inspector.h"
#include "Components/component.h"
#include "Components/mesh_component.h"

namespace Ming3D
{
    PropertyInspector::PropertyInspector(Control* arg_parent)
        : UserControl(arg_parent)
    {
    }

    void PropertyInspector::SelectActor(Actor* inActor)
    {
        mSelectedActor = inActor;
        Refresh();
    }

    void PropertyInspector::Refresh()
    {
        mNeedsRefresh = true;
    }

    void PropertyInspector::RefreshInternal()
    {
        if (mSelectedActor == nullptr)
            return;

        mNeedsRefresh = false;

        // Remove existing controls
        for (Control* ctrl : mControls)
            delete(ctrl);
        mControls.clear();

        const float ctrlHeight = 20.0f;
        const float vertPadding = 10.0f;

        float currHeight = 0.0f;

        // Header text
        NativeUI::TextBox* txtHeader = CreateTextBox(mSelectedActor->GetClass()->GetName().c_str(), 30, ctrlHeight, currHeight);

        currHeight += vertPadding * 2.0f;

        if (mSelectedActor->GetClass()->GetAllProperties(true).size() > 0)
        {
            NativeUI::TextBox* txtPropHeader = CreateTextBox("Properties:", 20, ctrlHeight, currHeight);
            currHeight += vertPadding;
        }

        for (Property* prop : mSelectedActor->GetClass()->GetAllProperties(true))
        {
            NativeUI::TextBox* txtProp = CreateTextBox(prop->GetPropertyName().c_str(), 16, ctrlHeight, currHeight);
            currHeight += vertPadding;
        }

        currHeight += vertPadding;

        if (mSelectedActor->GetComponents().size() > 0)
        {
            NativeUI::TextBox* txtCompHeader = CreateTextBox("Components:", 20, ctrlHeight, currHeight);
            currHeight += vertPadding;
        }

        for (Component* comp : mSelectedActor->GetComponents())
        {
            // TODO: Show component properties + custom inspectors
            NativeUI::TextBox* txtCompName = CreateTextBox(comp->GetClass()->GetName().c_str(), 16, ctrlHeight, currHeight);
            currHeight += vertPadding;

            if (comp->GetClass() == MeshComponent::GetStaticClass())
            {
                NativeUI::Button* btnAddComp = CreateButton("Set mesh", 16, ctrlHeight, currHeight);
                btnAddComp->RegisterClickCallback([comp]()
                {
                    ((MeshComponent*)comp)->SetMesh("Resources//Mvr_PetCow_walk.dae"); // TEMP TEST
                });
                currHeight += vertPadding;
            }
        }

        for (Class* compClass : Component::GetStaticClass()->GetChildClasses())
        {
            NativeUI::Button* btnAddComp = CreateButton(std::string("Add ") + compClass->GetName(), 16, ctrlHeight, currHeight);
            btnAddComp->RegisterClickCallback([this, compClass]()
            {
                mSelectedActor->AddComponent((Component*)compClass->CreateInstance());
                Refresh();
            });
            currHeight += vertPadding;
        }
    }

    NativeUI::TextBox* PropertyInspector::CreateTextBox(std::string inText, int inFontSize, float inCtrlHeight, float& outCurrHeight)
    {
        NativeUI::TextBox* txtBox = new NativeUI::TextBox(this);
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

    NativeUI::Button* PropertyInspector::CreateButton(std::string inText, int inFontSize, float inCtrlHeight, float& outCurrHeight)
    {
        NativeUI::Button* btn = new NativeUI::Button(this);
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

    void PropertyInspector::OnTick()
    {
        if (mNeedsRefresh)
            RefreshInternal();
    }
}
