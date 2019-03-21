#ifndef MING3D_MESHCOMPONENT_H
#define MING3D_MESHCOMPONENT_H

#include "component.h"
#include "Model/material.h"

namespace Ming3D
{
    class MeshComponent : public Component
    {
        DEFINE_CLASS(Ming3D::MeshComponent, Ming3D::Component)

    private:
        static void InitialiseClass();

        Material* mMaterial = nullptr;

    public:
        MeshComponent();
        virtual void InitialiseComponent();
        void SetMesh(const char* meshFile);
    };
}

#endif
