#include "world.h"
#include "Source/Actors/actor.h"
#include "Source/Components/component.h"
#include <algorithm>
#include <functional>
#include <iterator>

namespace Ming3D
{
    World::World(GameEngine* gameEngine)
    {
        mGameEngine = gameEngine;
        mRenderScene = std::make_unique<RenderScene>();
    }

    void World::InitialiseActor(Actor* actor)
    {
        for (Component* comp : actor->GetComponents())
        {
            comp->InitialiseComponent();
        }
    }

    ActorPtr World::SpawnActor()
    {
        return SpawnActor("");
    }

    ActorPtr World::SpawnActor(const std::string& name)
    {
        std::unique_ptr<Actor> actor = std::make_unique<Actor>(this);
        ActorPtr ptr = actor.get();
        if (!name.empty())
            ptr->SetActorName(name);
        InitialiseActor(ptr.Get());
        mActors.push_back(std::move(actor));
        return ptr;
    }

    std::vector<ActorPtr> World::GetActors()
    {
        std::vector<ActorPtr> actors;
        std::transform(mActors.begin(), mActors.end(), std::back_inserter(actors), [](std::unique_ptr<Actor>& actor) { return actor.get(); } );
        return actors;
    }

    std::vector<ActorPtr> World::GetActorsRecursive()
    {
        std::vector<ActorPtr> actors;
        std::function<void(Actor*)> addActorRecursive;
        addActorRecursive = [&actors, &addActorRecursive](Actor* actor) {
            actors.push_back(actor);
            auto children = actor->GetChildren();
            std::for_each(children.begin(), children.end(), [&addActorRecursive](ActorPtr& actor)
            {
                addActorRecursive(actor.Get());
            });
        };
        std::for_each(mActors.begin(), mActors.end(), [&addActorRecursive](std::unique_ptr<Actor>& actor)
        {
            addActorRecursive(actor.get());
        });
        return actors;
    }

    void World::Tick(float inDeltaTime)
    {
        for (auto& actor : mActors)
        {
            actor->Tick(inDeltaTime);
        }
    }
}
