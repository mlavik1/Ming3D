#include "widget_component.h"
#include "GUI/widget_tree.h"
#include "Actors/actor.h"
#include "GameEngine/game_engine.h"
#include "SceneRenderer/scene_renderer.h"
#include "Debug/st_assert.h"

IMPLEMENT_CLASS(Ming3D::WidgetComponent)

namespace Ming3D
{
    WidgetComponent::WidgetComponent()
    {
        mRenderObject = new WidgetRenderObject();
        mWidgetTree = new WidgetTree();
        SetRenderMode(mRenderMode);
        GGameEngine->GetSceneRenderer()->AddSceneObject(mRenderObject);
    }

    WidgetComponent::~WidgetComponent()
    {
        delete mRenderObject;
    }

    void WidgetComponent::InitialiseClass()
    {

    }

    void WidgetComponent::InitialiseComponent()
    {
        Super::InitialiseComponent();
    }

    void WidgetComponent::Tick(float inDeltaTime)
    {
        Component::Tick(inDeltaTime);

        mWidgetTree->UpdateWidgetTree();
        mWidgetTree->UpdateRenderData();

        // TODO: If moved, update call mWidgetTree->SetTransform !!!

        if (mRenderMode == EWidgetRenderMode::World)
            mWidgetTree->SetTransform(GetParent()->GetTransform().GetWorldTransformMatrix());
        else
            mWidgetTree->SetTransform(glm::mat4());
    }

    void WidgetComponent::SetWidget(std::shared_ptr<Widget> widget)
    {
        mWidget = widget;
        mWidgetTree->SetRootWidget(widget);
        mRenderObject->SetWidgetTree(mWidgetTree);
    }

    void WidgetComponent::SetRenderMode(EWidgetRenderMode renderMode)
    {
        mRenderMode = renderMode;

        switch (renderMode)
        {
        case EWidgetRenderMode::World:
        {
            mRenderObject->SetRenderType(ERenderType::Transparent);
            break;
        }
        case EWidgetRenderMode::Overlay:
        {
            mRenderObject->SetRenderType(ERenderType::GUIOverlay);
            break;
        }
        default:
            __AssertComment(false, "Unimplemented widget render mode.");
        }
    }
}
