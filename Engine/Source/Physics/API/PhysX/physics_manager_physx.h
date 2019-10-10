#ifndef MING3D_PHYSICSMANAGERPHYSX_H
#define MING3D_PHYSICSMANAGERPHYSX_H

#include "Physics/physics_manager.h"
#include "physics_scene_physx.h"
#include <vector>
#include "physx_declarations.h"

namespace Ming3D
{
    class PhysicsManagerPhysX : public PhysicsManager
    {
    private:
        std::vector<PhysicsScenePhysX*> mScenes;
        physx::PxFoundation* mPxFoundation;
        physx::PxPhysics* mPxPhysics;

        physx::PxDefaultErrorCallback* mDefaultErrorCallback;
        physx::PxDefaultAllocator* mDefaultAllocatorCallback;

    public:
        PhysicsManagerPhysX();
        virtual ~PhysicsManagerPhysX();
        virtual PhysicsScene* CreatePhysicsScene() override;
        
        virtual void SimulateScenes(float seconds) override;

        physx::PxPhysics* GetPxPhysics() { return mPxPhysics; }
        virtual PhysicsScene* GetPhysicsScene(int scene) override { return mScenes[scene]; }
    };
}

#endif
