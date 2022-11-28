#pragma once
#include "render_object.h"
#include "glm/mat4x4.hpp"

namespace Ming3D
{
    class Mesh;
    class Material;
    class Transform;
    class MeshBuffer;
    class MaterialBuffer;

    class MeshRenderObject : public RenderObject
    {
    private:
        MeshBuffer* mMeshBuffer = nullptr;
        MaterialBuffer* mMaterialBuffer = nullptr;
        glm::mat4 mModelMatrix;

    public:
        void SetMesh(Mesh* mesh, bool dynamic = false);
        void SetMaterial(Material* material);
        void SetTransform(glm::mat4 trans);

        void ReuploadVertexData(Mesh* mesh);

        virtual int GetNumBatches() override;
        virtual void GetRenderBatch(int batchIndex, RenderBatch* outBatch) override;

        virtual ERenderType GetRenderType() override;
        virtual bool GetCastShadows() override;
        virtual glm::vec3 GetWorldPosition() override;
    };
}
