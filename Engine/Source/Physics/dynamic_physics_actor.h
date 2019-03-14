#ifndef MING3D_DYNAMICPHYSICSACTOR_H
#define MING3D_DYNAMICPHYSICSACTOR_H

#include "physics_actor.h"
#include "Actors/transform.h"
#include "force_mode.h"

namespace Ming3D
{
    class DynamicPhysicsActor : public PhysicsActor
    {
    private:
        physx::PxRigidDynamic* mPxRigidDynamic = nullptr;

        bool mIsKinematic = false;

    public:
        DynamicPhysicsActor();
        virtual physx::PxRigidActor* GetRigidActor() override;

        virtual void UpdateTransform(const Transform& inTrans) override;

        void SetKinematic(bool inKinematic);
        void SetMass(float mass);
        void AddForce(const glm::vec3& force, const ForceMode& forceMode);
        void AddForceAtPos(const glm::vec3& force, const glm::vec3& pos, const ForceMode& forceMode);

        bool IsKinematic() const { return mIsKinematic; };
    };
}

#endif
