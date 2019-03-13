#ifndef MING3D_PHYSICSMANAGER_H
#define MING3D_PHYSICSMANAGER_H

#include <vector>
#include "physics_scene.h"
#include "physx_declarations.h"

namespace Ming3D
{
    class PhysicsManager
    {
    private:
        std::vector<PhysicsScene*> mScenes;
        physx::PxFoundation* mPxFoundation;
        physx::PxPhysics* mPxPhysics;

        physx::PxDefaultErrorCallback* mDefaultErrorCallback;
        physx::PxDefaultAllocator* mDefaultAllocatorCallback;

    public:
        PhysicsManager();
        ~PhysicsManager();
        PhysicsScene* CreatePhysicsScene();
        
        void SimulateScenes(float seconds);

        physx::PxPhysics* GetPxPhysics() { return mPxPhysics; }
        PhysicsScene* GetPhysicsScene(int scene) { return mScenes[scene]; }
    };
}

#endif
