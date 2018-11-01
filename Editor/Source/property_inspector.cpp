#include "property_inspector.h"
#include "Components/component.h"
#include "Components/mesh_component.h"

namespace Ming3D
{
    PropertyInspector::PropertyInspector(Control* arg_parent)
        : UserControl(arg_parent)
    {
        propInspectorBuilder = new PropertyInspectorBuilder(this);
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

        propInspectorBuilder->Clear();

        // Header text
        propInspectorBuilder->Text(mSelectedActor->GetClass()->GetName().c_str(), 20);

        propInspectorBuilder->Space();

        if (mSelectedActor->GetClass()->GetAllProperties(true).size() > 0)
        {
            propInspectorBuilder->Text("Properties:", 16);
        }

        for (Property* prop : mSelectedActor->GetClass()->GetAllProperties(true))
        {
            propInspectorBuilder->PropertyField(prop, mSelectedActor, prop->GetPropertyName().c_str());
        }

        propInspectorBuilder->Space();

        if (mSelectedActor->GetComponents().size() > 0)
        {
            propInspectorBuilder->Text("Components:", 16);
        }

        for (Component* comp : mSelectedActor->GetComponents())
        {
            // TODO: Show component properties + custom inspectors
            propInspectorBuilder->Text(comp->GetClass()->GetName().c_str(), 14);

            if (comp->GetClass() == MeshComponent::GetStaticClass())
            {
                propInspectorBuilder->Button("Set mesh", [comp]()
                {
                    ((MeshComponent*)comp)->SetMesh("Resources//Mvr_PetCow_walk.dae"); // TEMP TEST
                });
            }
        }

        for (Class* compClass : Component::GetStaticClass()->GetChildClasses())
        {
            propInspectorBuilder->Button(std::string("Add ") + compClass->GetName(), [this, compClass]()
            {
                mSelectedActor->AddComponent((Component*)compClass->CreateInstance());
                Refresh();
            });
        }
    }

    void PropertyInspector::OnTick()
    {
        if (mNeedsRefresh)
            RefreshInternal();
    }
}
