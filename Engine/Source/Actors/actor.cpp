#include "actor.h"
#include "Source/Components/component.h"

IMPLEMENT_CLASS(Ming3D::Actor)

namespace Ming3D
{
    void Actor::InitialiseClass()
    {

    }

    Actor::Actor()
    {
    }

    Actor::~Actor()
    {
    }

    void Actor::InitialiseActor()
    {
        mIsInitialised = true;
    }

    void Actor::Tick(float inDeltaTime)
    {

    }
}
