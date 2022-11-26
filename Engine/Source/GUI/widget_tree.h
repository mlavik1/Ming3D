#pragma once
#include "widget_update_params.h"
#include <vector>
#include <memory>
#include "SceneRenderer/render_batch.h"

namespace Ming3D
{
    class Widget;
    class Visual;
    class VisualSubmeshNode;
    class GUIVertexData;
    class Material;

    namespace Rendering
    {
        class VertexData;
        class IndexData;
    }

    class WidgetTree
    {
    private:
        std::shared_ptr<Widget> mRootWidget;

        VisualSubmeshNode* mRootSubmeshNode;
        VisualSubmeshNode* mTailSubmeshNode;

        std::unique_ptr<Rendering::VertexData> mVertexData;
        std::unique_ptr<Rendering::IndexData> mIndexData;

        std::vector<RenderBatch> mRenderBatches;

        MeshBuffer* mMeshBuffer;
        Material* mMaterial;
        glm::mat4 mTransformMatrix;

        void UpdateWidgetRecursive(Widget* widget, WidgetUpdateParams params);

        void AddRenderBatch(RenderBatch batch);

        void OffsetSubmeshes(VisualSubmeshNode* node, int64_t vertexOffset, int64_t indexOffset);

        VisualSubmeshNode* ReallocSubmesh(VisualSubmeshNode* oldNode, unsigned int vertexCount, unsigned int indexCount);

    public:
        WidgetTree();
        ~WidgetTree();
        
        void SetRootWidget(std::shared_ptr<Widget> widget);
        void SetTransform(glm::mat4 transMat);

        void UpdateWidgetTree();
        void UpdateRenderData();
        size_t GetNumBatches();
        void GetBatch(size_t batchIndex, RenderBatch* outBatch);
    };
}
