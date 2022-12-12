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

    World::~World()
    {
        for (Actor* actor : mActors)
            delete actor;
    }

    void World::AddActor(Actor* inActor)
    {
        mActors.push_back(inActor);
        for (Component* comp : inActor->GetComponents())
        {
            comp->InitialiseComponent();
        }
    }

    Actor* World::SpawnActor()
    {
        Actor* actor = new Actor(this);
        AddActor(actor);
        return actor;
    }

    Actor* World::SpawnActor(const std::string& name)
    {
        Actor* actor = SpawnActor();
        actor->SetActorName(name);
        return actor;
    }

    std::vector<Actor*> World::GetActorsRecursive()
    {
        std::vector<Actor*> actors;
        std::function<void(Actor*)> addActorRecursive;
        addActorRecursive = [&actors, &addActorRecursive](Actor* actor) {
            actors.push_back(actor);
            auto children = actor->GetChildren();
            std::for_each(children.begin(), children.end(), addActorRecursive);
        };
        std::for_each(mActors.begin(), mActors.end(), addActorRecursive);
        return mActors;
    }

    void World::Tick(float inDeltaTime)
    {
        for (Actor* actor : mActors)
        {
            actor->Tick(inDeltaTime);
        }
    }
}
