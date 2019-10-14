#include "collider_component.h"

IMPLEMENT_CLASS(Ming3D::ColliderComponent)

namespace Ming3D
{
    ColliderComponent::ColliderComponent()
    {

    }

    ColliderComponent::~ColliderComponent()
    {

    }

    void ColliderComponent::InitialiseClass()
    {

    }

    void ColliderComponent::InitialTick()
    {
        Component::InitialTick();

        RecreatePhysicsShape();
    }

    void ColliderComponent::RecreatePhysicsShape()
    {

    }

    void ColliderComponent::UpdatePhysicsShape()
    {

    }
}
