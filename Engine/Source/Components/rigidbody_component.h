#ifndef MING3D_RIGIDBODYCOMPONENT_H
#define MING3D_RIGIDBODYCOMPONENT_H

#include "component.h"
#include "Physics/physics_actor.h"
#include "Physics/force_mode.h"

namespace Ming3D
{
    class RigidBodyComponent : public Component
    {
        DEFINE_CLASS(Ming3D::RigidBodyComponent, Ming3D::Component)

    private:
        static void InitialiseClass();

        PhysicsActor* mPhysicsActor = nullptr;
        bool mIsStatic = false;
        int mScene = 0;
        bool mIsKinematic = false;
        float mMass = 1.0f;

        void RecreatePhysicsActor();
        void DestroyPhysicsActor();
        void UpdateTransform();

    public:
        RigidBodyComponent();
        virtual ~RigidBodyComponent();

        virtual void InitialiseComponent() override;
        virtual void InitialTick() override;
        
        void SetStatic(bool setStatic);
        void SetMass(float mass);
        void SetKinematic(bool setKinematic);
        void SetScene(int scene);
        void AddForce(const glm::vec3& force, const ForceMode& forceMode);
        void AddForceAtPos(const glm::vec3& force, const glm::vec3& pos, const ForceMode& forceMode);

        virtual void PostMove() override;

        bool IsStatic() { return mIsStatic; }
        PhysicsActor* GetPhysicsActor() { return mPhysicsActor; }
    };
}

#endif
