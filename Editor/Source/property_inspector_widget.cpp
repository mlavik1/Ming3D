#include "property_inspector_widget.h"
#include "GameEngine/game_engine.h"
#include "Actors/actor.h"
#include "World/world.h"
#include "editor.h"
#include "Object/class.h"
#include "GUI/image_widget.h"

IMPLEMENT_CLASS(Ming3D::PropertyInspectorWidget)

namespace Ming3D
{
    PropertyInspectorWidget::PropertyInspectorWidget()
    {
        mBackgroundWidget = std::make_shared<ImageWidget>();
        mBackgroundWidget->SetColour(glm::vec4(0.15f, 0.15f, 0.15f, 1.0f));
        addWidget(mBackgroundWidget);
        mWidgetLayoutBuilder = std::make_unique<WidgetLayoutBuilder>(this);
        mHeaderTextStyle.fontSize = 24;
        mSubHeaderTextStyle.fontSize = 20;
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
        ActorPtr actor = GEditor->mSelectedActor;
        mWidgetLayoutBuilder->BeginBuilding();
        if (actor != nullptr)
        {
            mWidgetLayoutBuilder->Label(actor->GetActorName(), mHeaderTextStyle);
            mWidgetLayoutBuilder->Space();

            
            if (actor->GetClass()->GetAllProperties(true).size() > 0)
            {
                mWidgetLayoutBuilder->Label("Properties", mSubHeaderTextStyle);
            }

            for (Property* prop : actor->GetClass()->GetAllProperties(true))
            {
                mWidgetLayoutBuilder->PropertyField(prop, actor.Get(), prop->GetPropertyName().c_str());
            }
            mWidgetLayoutBuilder->Space();

            std::vector<Component*> components = actor->GetComponents();
            for (Component* component : components)
            {
                mWidgetLayoutBuilder->Label(component->GetClass()->GetName());
                mWidgetLayoutBuilder->Space();
            }
        }
        mWidgetLayoutBuilder->EndBuilding();
    }
}
