#include "scene_hierarchy_widget.h"
#include "GUI/tree_view_widget.h"
#include "GameEngine/game_engine.h"
#include "World/world.h"
#include "GUI/tree_view_widget.h"
#include "Actors/actor.h"

IMPLEMENT_CLASS(Ming3D::SceneHierarchyWidget)

namespace Ming3D
{
    SceneHierarchyWidget::SceneHierarchyWidget()
    {
        mTreeView = std::make_shared<TreeViewWidget>();
        addWidget(mTreeView);
    }

    SceneHierarchyWidget::~SceneHierarchyWidget()
    {

    }

    void SceneHierarchyWidget::InitialiseClass()
    {
        
    }

    void SceneHierarchyWidget::Tick(float inDeltaTime)
    {
        if (isInitialisedHACK)
            return;
        isInitialisedHACK = true; // TODO: InitialTick? (we want to do this whenever the scene changes though!)

        auto actors = GGameEngine->GetWorld()->GetActors();
        for (size_t i = 0; i < actors.size(); ++i)
        {
            auto actor = actors[i];
            int depth = 0;
            auto parent = actor->GetTransform().GetParent();
            while (parent != nullptr)
            {
                depth++;
                parent = parent->GetParent();
            }
            mTreeView->AddItem(static_cast<int>(i), actor->GetActorName(), depth);
        }
    }
}
