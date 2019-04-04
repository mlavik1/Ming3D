#include "collider_component.h"

#include "PxShape.h"
#include "PxMaterial.h"

IMPLEMENT_CLASS(Ming3D::ColliderComponent)

namespace Ming3D
{
    ColliderComponent::ColliderComponent()
    {

    }

    ColliderComponent::~ColliderComponent()
    {
        if (mPxMaterial != nullptr)
            mPxMaterial->release();
        if (mPxShape != nullptr)
            mPxShape->release();
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
