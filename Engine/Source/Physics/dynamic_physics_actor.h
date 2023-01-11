#ifndef MING3D_DYNAMICPHYSICSACTOR_H
#define MING3D_DYNAMICPHYSICSACTOR_H

#include "physics_actor.h"
#include "Actors/transform.h"
#include "force_mode.h"

namespace Ming3D
{
    class DynamicPhysicsActor : public PhysicsActor
    {
    public:
        DynamicPhysicsActor() {};
        virtual ~DynamicPhysicsActor() override = default;

        virtual void UpdateTransform(const Transform& inTrans) override = 0;

        virtual void SetKinematic(bool inKinematic) = 0;
        virtual void SetMass(float mass) = 0;
        virtual void AddForce(const glm::vec3& force, const ForceMode& forceMode) = 0;
        virtual void AddForceAtPos(const glm::vec3& force, const glm::vec3& pos, const ForceMode& forceMode) = 0;

        virtual bool IsKinematic() const = 0;
    };
}

#endif
