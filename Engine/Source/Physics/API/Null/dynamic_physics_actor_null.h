#ifndef MING3D_DYNAMICPHYSICSACTORNULL_H
#define MING3D_DYNAMICPHYSICSACTORNULL_H

#include "Physics/dynamic_physics_actor.h"

namespace Ming3D
{
    class DynamicPhysicsActorNull : public DynamicPhysicsActor
    {
    public:
        DynamicPhysicsActorNull() {};
        virtual ~DynamicPhysicsActorNull() {}

        virtual void UpdateTransform(const Transform& inTrans) override {};

        virtual void SetKinematic(bool inKinematic) {};
        virtual void SetMass(float mass) {};
        virtual void AddForce(const glm::vec3& force, const ForceMode& forceMode) {};
        virtual void AddForceAtPos(const glm::vec3& force, const glm::vec3& pos, const ForceMode& forceMode) {};

        virtual bool IsKinematic() const override { return false; };
    };
}

#endif
