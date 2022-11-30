#include "widget_component.h"
#include "GUI/widget_tree.h"
#include "Actors/actor.h"
#include "GameEngine/game_engine.h"
#include "Debug/st_assert.h"
#include "Input/input_manager.h"
#include "World/world.h"
#include "window_base.h"
#include "World/world.h"
#include "SceneRenderer/render_scene.h"
#include <cmath>

IMPLEMENT_CLASS(Ming3D::WidgetComponent)

namespace Ming3D
{
    WidgetComponent::WidgetComponent()
    {
        mRenderObject = new WidgetRenderObject();
        mWidgetTree = new WidgetTree();
    }

    WidgetComponent::~WidgetComponent()
    {
        GetWorld()->GetRenderScene()->RemoveSceneObject(mRenderObject);
        delete mRenderObject;
    }

    void WidgetComponent::InitialiseClass()
    {

    }

    void WidgetComponent::InitialiseComponent()
    {
        Super::InitialiseComponent();
        SetRenderMode(mRenderMode);
        GetWorld()->GetRenderScene()->AddSceneObject(mRenderObject);
    }

    void WidgetComponent::Tick(float inDeltaTime)
    {
        Component::Tick(inDeltaTime);

        auto inputManager = GetWorld()->GetGameEngine()->GetInputManager();
        const std::vector<InputEvent>& events = inputManager->GetEvents();
        glm::ivec2 mousePosition = inputManager->GetMousePosition();
        if (mRenderMode == EWidgetRenderMode::World)
        {
            // TODO: Raycast
        }
        mWidgetTree->TickWidgets(inDeltaTime);
        mWidgetTree->HandleEvents(events, mousePosition);
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
        mWidgetTree->SetCanvasSize(mCanvasSize);
        mWidgetTree->SetWidget(widget);
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
            mWidgetTree->SetCanvasSize(mCanvasSize);
            break;
        }
        case EWidgetRenderMode::Overlay:
        {
            mRenderObject->SetRenderType(ERenderType::GUIOverlay);
            auto window = GetWorld()->GetGameEngine()->GetMainWindow();
            mWidgetTree->SetCanvasSize(glm::ivec2(window->GetWidth(), window->GetHeight()));
            break;
        }
        default:
            __AssertComment(false, "Unimplemented widget render mode.");
        }
    }

    void WidgetComponent::SetCanvasSize(glm::ivec2 canvasSize)
    {
        mCanvasSize = canvasSize;
        mWidgetTree->SetCanvasSize(canvasSize);
    }
}
