#include "component.h"
#include "Source/World/world.h"

IMPLEMENT_CLASS(Ming3D::Component)

namespace Ming3D
{
    void Component::InitialiseClass()
    {

    }

    Component::Component()
    {
        SetObjectFlag(ObjectFlag::Serialise); // serialised by default
    }

    Component::~Component()
    {
    }

    void Component::InitialiseComponent()
    {
        
    }

    void Component::Tick(float inDeltaTime)
    {

    }

    void Component::InitialTick()
    {

    }

    void Component::PostMove()
    {
        
    }
}
