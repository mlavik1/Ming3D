#ifndef MING3D_COLLIDERCOMPONENT_H
#define MING3D_COLLIDERCOMPONENT_H

#include "component.h"
#include "Physics/physx_declarations.h"

namespace Ming3D
{
    class ColliderComponent : public Component
    {
        DEFINE_CLASS(Ming3D::ColliderComponent, Ming3D::Component)

    private:
        static void InitialiseClass();

    protected:
        virtual void InitialTick() override;
        
        virtual void RecreatePhysicsShape();
        virtual void UpdatePhysicsShape();

        physx::PxMaterial* mPxMaterial = nullptr;
        physx::PxShape* mPxShape = nullptr;
    
    public:
        ColliderComponent();
        virtual ~ColliderComponent();
    };
}

#endif
