#include "collider_component.h"

IMPLEMENT_CLASS(Ming3D::ColliderComponent)

namespace Ming3D
{
    ColliderComponent::ColliderComponent()
    {

    }

    ColliderComponent::~ColliderComponent()
    {
        if(mPxMaterial != nullptr)
            delete mPxMaterial;
        if (mPxShape != nullptr)
            delete mPxShape;
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
