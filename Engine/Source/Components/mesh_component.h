#ifndef MING3D_MESHCOMPONENT_H
#define MING3D_MESHCOMPONENT_H

#include "component.h"
#include "Model/material.h"
#include "Model/mesh.h"
#include "SceneRenderer/mesh_render_object.h"
#include <memory>

namespace Ming3D
{
    class MeshComponent : public Component
    {
        DEFINE_CLASS(Ming3D::MeshComponent, Ming3D::Component)

    private:
        static void InitialiseClass();

        std::shared_ptr<Mesh> mMesh = nullptr;
        std::shared_ptr<Material> mMaterial = nullptr;
        MeshRenderObject* mRenderObject = nullptr;

    public:
        MeshComponent();
        ~MeshComponent();
        virtual void InitialiseComponent();
        void SetMesh(std::shared_ptr<Mesh> inMesh, bool dynamic = false);
        void SetMaterial(std::shared_ptr<Material> inMat);
        void ReuploadVertexData();

        virtual void Tick(float inDeltaTime) override;

        std::shared_ptr<Mesh> GetMesh() { return mMesh; };
        std::shared_ptr<Material> GetMaterial() { return mMaterial; }
    };
}

#endif
