#include "world.h"

#include "Source/Actors/actor.h"
#include "Source/Components/component.h"

namespace Ming3D
{
    World::World(GameEngine* gameEngine)
    {
        mGameEngine = gameEngine;
    }

    World::~World()
    {
        // TODO: Destroy actors?
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
}
