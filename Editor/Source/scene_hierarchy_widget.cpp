#include "scene_hierarchy_widget.h"
#include "GUI/tree_view_widget.h"
#include "GameEngine/game_engine.h"
#include "World/world.h"
#include "GUI/tree_view_widget.h"
#include "Actors/actor.h"
#include <algorithm>
#include "Debug/debug.h"
#include "editor.h"
#include "GUI/image_widget.h"

IMPLEMENT_CLASS(Ming3D::SceneHierarchyWidget)

namespace Ming3D
{
    SceneHierarchyWidget::SceneHierarchyWidget()
    {
        mBackgroundWidget = std::make_shared<ImageWidget>();
        mBackgroundWidget->SetColour(glm::vec4(0.15f, 0.15f, 0.15f, 1.0f));
        addWidget(mBackgroundWidget);

        mTreeView = std::make_shared<TreeViewWidget>();
        mTreeView->mOnItemSelected = [this](int clickedId){
            this->OnItemSelected(clickedId);
        };
        addWidget(mTreeView);
    }

    SceneHierarchyWidget::~SceneHierarchyWidget()
    {

    }

    void SceneHierarchyWidget::InitialiseClass()
    {
        
    }

    void SceneHierarchyWidget::AddActorRecursive(Actor* actor, int depth)
    {
        mTreeView->AddItem(static_cast<int>(actor->GetGuid()), actor->GetActorName(), depth);
        
        for (Actor* child : actor->GetChildren())
        {
            AddActorRecursive(child, depth++);
        }
    }

    void SceneHierarchyWidget::OnItemSelected(int id)
    {
        LOG_INFO() << "Selected ID: " << id;
        auto actors = GGameEngine->GetWorld().lock()->GetActorsRecursive();
        auto itActor = std::find_if(
            actors.begin(),
            actors.end(),
            [id](Actor* candidate){
                return static_cast<int>(candidate->GetGuid()) == id; // TODO: Use uint for IDs?
            });
        if (itActor != actors.end())
        {
            GEditor->mSelectedActor = *itActor;
        }
    }

    void SceneHierarchyWidget::Start()
    {
        std::vector<Actor*> actors = GGameEngine->GetWorld().lock()->GetActors();
        std::vector<Actor*> rootActors;
        std::copy_if(actors.begin(),  actors.end(), std::back_inserter(rootActors), [](Actor* actor){ return actor->GetTransform().GetParent() == nullptr; });
        for (auto actor : rootActors)
        {
            AddActorRecursive(actor, 0);
        }
    }

    void SceneHierarchyWidget::Tick(float inDeltaTime)
    {
    }
}
