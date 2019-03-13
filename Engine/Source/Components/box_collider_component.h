#ifndef MING3D_BOXCOLLIDERCOMPONENT_H
#define MING3D_BOXCOLLIDERCOMPONENT_H

#include "collider_component.h"
#include "rigidbody_component.h"
#include "glm/vec3.hpp"

namespace Ming3D
{
    class BoxColliderComponent : public ColliderComponent
    {
        DEFINE_CLASS(Ming3D::BoxColliderComponent, Ming3D::ColliderComponent)

    private:
        glm::vec3 mSize = glm::vec3(1.0f, 1.0f, 1.0f);

        static void InitialiseClass();

        virtual void InitialTick() override;
        virtual void InitialiseComponent() override;

        virtual void RecreatePhysicsShape() override;
        virtual void UpdatePhysicsShape() override;

        virtual void PostMove() override;
    
    public:
        void SetSize(glm::vec3 size);
    };
}

#endif
