#pragma once
#include "widget_update_params.h"
#include <vector>
#include <memory>
#include "SceneRenderer/render_batch.h"
#include "Input/input_event.h"
#include "glm/vec2.hpp"

namespace Ming3D
{
    class Widget;
    class Visual;
    class VisualSubmeshNode;
    struct GUIVertexData;
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
        unsigned int mVertexIndex = 0;
        unsigned int mTriangleIndex = 0;

        std::vector<RenderBatch> mRenderBatches;

        MeshBuffer* mMeshBuffer;
        Material* mMaterial;
        glm::mat4 mTransformMatrix;
        glm::ivec2 mCanvasSize;
        bool mVisualsInvalidated = true;

        void TickWidgetsRecursive(Widget* widget, float deltaTime);

        void UpdateWidgetRecursive(Widget* widget, WidgetUpdateParams params);

        void AddRenderBatch(RenderBatch batch);

        WidgetRect ToScreenSpaceRect(const WidgetRect& rect);

        void HandleEventRecursive(Widget* widget, InputEvent event, glm::ivec2 mousePosition);

    public:
        WidgetTree();
        ~WidgetTree();
        
        void SetWidget(std::shared_ptr<Widget> widget);
        void SetTransform(glm::mat4 transMat);
        void SetCanvasSize(glm::ivec2 canvasSize);

        void HandleEvents(const std::vector<InputEvent>& events, glm::ivec2 mousePosition);
        
        void TickWidgets(float deltaTime);
        void UpdateWidgetTree();
        void UpdateRenderData();
        size_t GetNumBatches() const;
        void GetBatch(size_t batchIndex, RenderBatch* outBatch);
    };
}
