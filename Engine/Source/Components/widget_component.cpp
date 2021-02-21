#include "widget_component.h"
#include "GUI/widget_tree.h"
#include "Actors/actor.h"
#include "GameEngine/game_engine.h"
#include "SceneRenderer/scene_renderer.h"

IMPLEMENT_CLASS(Ming3D::WidgetComponent)

namespace Ming3D
{
    WidgetComponent::WidgetComponent()
    {
        mRenderObject = new WidgetRenderObject();
        mWidgetTree = new WidgetTree();

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
    }

    void WidgetComponent::SetWidget(Widget* widget)
    {
        mWidget = widget;
        mWidgetTree->SetRootWidget(widget);
        mWidgetTree->SetTransform(GetParent()->GetTransform().GetWorldTransformMatrix());
        mRenderObject->SetWidgetTree(mWidgetTree);
    }
}
