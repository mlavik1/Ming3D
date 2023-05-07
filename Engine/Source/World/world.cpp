#include "world.h"
#include "Source/Actors/actor.h"
#include "Source/Components/component.h"
#include <algorithm>
#include <functional>

namespace Ming3D
{
    World::World(GameEngine* gameEngine)
    {
        mGameEngine = gameEngine;
        mRenderScene = std::make_unique<RenderScene>();
    }

    void World::AddActor(std::shared_ptr<Actor> inActor)
    {
        mActors.push_back(inActor);
        for (Component* comp : inActor->GetComponents())
        {
            comp->InitialiseComponent();
        }
    }

    std::weak_ptr<Actor> World::SpawnActor()
    {
        return SpawnActor("");
    }

    std::weak_ptr<Actor> World::SpawnActor(const std::string& name)
    {
        std::shared_ptr<Actor> actor = std::make_shared<Actor>(this);
        if (!name.empty())
            actor->SetActorName(name);
        AddActor(actor);
        return actor;
    }

    std::vector<std::weak_ptr<Actor>> World::GetActors()
    {
        std::vector<std::weak_ptr<Actor>> actors;
        std::copy(mActors.begin(), mActors.end(), std::back_inserter(actors));
        return actors;
    }

    std::vector<std::weak_ptr<Actor>> World::GetActorsRecursive()
    {
        std::vector<std::weak_ptr<Actor>> actors;
        std::function<void(std::weak_ptr<Actor>&)> addActorRecursive;
        addActorRecursive = [&actors, &addActorRecursive](std::weak_ptr<Actor>& actor) {
            actors.push_back(actor);
            auto children = actor.lock()->GetChildren();
            std::for_each(children.begin(), children.end(), addActorRecursive);
        };
        std::for_each(mActors.begin(), mActors.end(), [&addActorRecursive](std::shared_ptr<Actor>& actor)
        {
            std::weak_ptr<Actor> ptr(actor);
            addActorRecursive(ptr);
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
