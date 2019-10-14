#ifndef MING3D_PHYSICSMANAGER_H
#define MING3D_PHYSICSMANAGER_H

#include "physics_scene.h"

namespace Ming3D
{
    class PhysicsManager
    {
    public:
        PhysicsManager() {}
        virtual ~PhysicsManager() {}
        
        virtual PhysicsScene* CreatePhysicsScene() = 0;
        
        virtual void SimulateScenes(float seconds) = 0;

        virtual PhysicsScene* GetPhysicsScene(int scene) = 0;
    };
}

#endif
