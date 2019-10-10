#ifndef MING3D_COLLIDERCOMPONENT_H
#define MING3D_COLLIDERCOMPONENT_H

#include "component.h"

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
    
    public:
        ColliderComponent();
        virtual ~ColliderComponent();
    };
}

#endif
