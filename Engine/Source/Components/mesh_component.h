#ifndef MING3D_MESHCOMPONENT_H
#define MING3D_MESHCOMPONENT_H

#include "component.h"
#include "Model/material.h"
#include "Model/mesh.h"
#include "SceneRenderer/mesh_render_object.h"

namespace Ming3D
{
    class MeshComponent : public Component
    {
        DEFINE_CLASS(Ming3D::MeshComponent, Ming3D::Component)

    private:
        static void InitialiseClass();

        Mesh* mMesh = nullptr;
        Material* mMaterial = nullptr;
        MeshRenderObject* mRenderObject = nullptr;

    public:
        MeshComponent();
        virtual void InitialiseComponent();
        void SetMesh(Mesh* inMesh);
        void SetMaterial(Material* inMat);
    
        virtual void Tick(float inDeltaTime) override;

        Mesh* GetMesh() { return mMesh; };
        Material* GetMaterial() { return mMaterial; }
    };
}

#endif
