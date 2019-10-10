#ifndef MING3D_DYNAMICPHYSICSACTORPHYSX_H
#define MING3D_DYNAMICPHYSICSACTORPHYSX_H

#include "Physics/dynamic_physics_actor.h"
#include "iphysx_actor.h"

namespace Ming3D
{
    class DynamicPhysicsActorPhysX : public DynamicPhysicsActor, public IPhysXActor
    {
    private:
        physx::PxRigidDynamic* mPxRigidDynamic = nullptr;

        bool mIsKinematic = false;

    public:
        DynamicPhysicsActorPhysX();
        virtual ~DynamicPhysicsActorPhysX() {}

        virtual physx::PxRigidActor* GetRigidActor() override;

        virtual void UpdateTransform(const Transform& inTrans) override;

        virtual void SetKinematic(bool inKinematic);
        virtual void SetMass(float mass);
        virtual void AddForce(const glm::vec3& force, const ForceMode& forceMode);
        virtual void AddForceAtPos(const glm::vec3& force, const glm::vec3& pos, const ForceMode& forceMode);

        virtual bool IsKinematic() const override { return mIsKinematic; };
    };
}

#endif
