#pragma once
#include "SceneRenderer/render_object.h"

namespace Ming3D
{
    class WidgetTree;
    class Camera;

    class WidgetRenderObject : public RenderObject
    {
    private:
        WidgetTree* mWidgetTree = nullptr;
        Camera* mCamera = nullptr;
        ERenderType mRenderType = ERenderType::Transparent;

    public:
        virtual size_t GetNumBatches() override;
        virtual void GetRenderBatch(size_t batchIndex, RenderBatch* outBatch) override;

        virtual ERenderType GetRenderType() override;
        virtual bool GetCastShadows() override;
        virtual glm::vec3 GetWorldPosition() override;

        void SetWidgetTree(WidgetTree* widgetTree);
        void SetRenderType(ERenderType renderType);
    };
}
