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
        mTransform = new Transform();
        mTransform->mActor = this;
    }

    Actor::~Actor()
    {
        delete mTransform;
    }

    void Actor::InitialiseActor()
    {
        mIsInitialised = true;
    }

    void Actor::Tick(float inDeltaTime)
    {

    }

    void Actor::AddChild(Actor* inActor)
    {
        mChildren.push_back(inActor);
        inActor->mTransform->mParentTransform = mTransform;
    }
}
