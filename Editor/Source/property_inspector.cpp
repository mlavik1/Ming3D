#include "property_inspector.h"
#include "Components/component.h"
#include "Components/mesh_component.h"

namespace Ming3D
{
    PropertyInspector::PropertyInspector(Control* arg_parent)
        : UserControl(arg_parent)
    {
        layoutBuilder = new WindowLayoutBuilder(this);
        headerTextStyle.fontSize = 22;
        subHeaderTextStyle.fontSize = 18;
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

        layoutBuilder->BeginBuilding();

        // Header text
        layoutBuilder->LabelField(mSelectedActor->GetClass()->GetName(), headerTextStyle);

        layoutBuilder->Space();

        if (mSelectedActor->GetClass()->GetAllProperties(true).size() > 0)
        {
            layoutBuilder->TextField("Properties:", subHeaderTextStyle);
        }

        for (Property* prop : mSelectedActor->GetClass()->GetAllProperties(true))
        {
            layoutBuilder->PropertyField(prop, mSelectedActor, prop->GetPropertyName().c_str());
        }

        layoutBuilder->Space();

        if (mSelectedActor->GetComponents().size() > 0)
        {
            layoutBuilder->TextField("Components:", subHeaderTextStyle);
        }

        for (Component* comp : mSelectedActor->GetComponents())
        {
            // TODO: Show component properties + custom inspectors
            layoutBuilder->TextField(comp->GetClass()->GetName());

            if (comp->GetClass() == MeshComponent::GetStaticClass())
            {
                if(layoutBuilder->Button("Set mesh"))
                {
                    ((MeshComponent*)comp)->SetMesh("Resources//Mvr_PetCow_walk.dae"); // TEMP TEST
                }
            }
        }

        for (Class* compClass : Component::GetStaticClass()->GetChildClasses())
        {
            if(layoutBuilder->Button(std::string("Add ") + compClass->GetName()))
            {
                mSelectedActor->AddComponent((Component*)compClass->CreateInstance());
                Refresh();
            }
        }

        layoutBuilder->EndBuilding();
    }

    void PropertyInspector::OnTick()
    {
        // TODO: only refresh when window is visible (or when window has focus? - add setting for it)
        //if (mNeedsRefresh)
        RefreshInternal();
    }
}
