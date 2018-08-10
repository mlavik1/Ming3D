#include "world.h"

#include "Source/Actors/actor.h"
#include "Source/Components/component.h"

namespace Ming3D
{
    void World::AddActor(Actor* inActor)
    {
        mActors.push_back(inActor);
        inActor->InitialiseActor();
        for (Component* comp : inActor->GetComponents())
        {
            comp->InitialiseComponent();
        }
    }
}
