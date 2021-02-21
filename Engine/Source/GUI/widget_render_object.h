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

    public:
        virtual int GetNumBatches() override;
        virtual void GetRenderBatch(int batchIndex, RenderBatch* outBatch) override;

        virtual ERenderType GetRenderType() override;
        virtual bool GetCastShadows() override;
        virtual glm::vec3 GetWorldPosition() override;

        void SetWidgetTree(WidgetTree* widgetTree);
    };
}
