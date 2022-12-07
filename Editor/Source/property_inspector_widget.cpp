#include "property_inspector_widget.h"
#include "GameEngine/game_engine.h"
#include "Actors/actor.h"
#include "World/world.h"
#include "widget_layout_builder.h"
#include "editor.h"
#include "Object/class.h"

IMPLEMENT_CLASS(Ming3D::PropertyInspectorWidget)

namespace Ming3D
{
    PropertyInspectorWidget::PropertyInspectorWidget()
    {
        mWidgetLayoutBuilder = std::make_unique<WidgetLayoutBuilder>(this);
    }

    PropertyInspectorWidget::~PropertyInspectorWidget()
    {
    }

    void PropertyInspectorWidget::InitialiseClass()
    {
        
    }

    void PropertyInspectorWidget::Start()
    {
    }

    void PropertyInspectorWidget::Tick(float deltaTime)
    {
        Actor* actor = GEditor->mSelectedActor;
        mWidgetLayoutBuilder->BeginBuilding();
        if (actor != nullptr)
        {
            mWidgetLayoutBuilder->TextField(actor->GetActorName());
            mWidgetLayoutBuilder->Space();
            std::vector<Component*> components = actor->GetComponents();
            for (Component* component : components)
            {
                mWidgetLayoutBuilder->TextField(component->GetClass()->GetName());
                mWidgetLayoutBuilder->Space();
            }
        }
        mWidgetLayoutBuilder->EndBuilding();
    }
}
