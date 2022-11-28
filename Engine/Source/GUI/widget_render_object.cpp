#include "widget_render_object.h"
#include "widget_tree.h"
#include "SceneRenderer/camera.h"
#include "render_target.h"
#include "render_window.h" // TODO

namespace Ming3D
{
    int WidgetRenderObject::GetNumBatches()
    {
        return mWidgetTree->GetNumBatches();
    }

    void WidgetRenderObject::GetRenderBatch(int batchIndex, RenderBatch* outBatch)
    {
        mWidgetTree->GetBatch(batchIndex, outBatch);
    }

    ERenderType WidgetRenderObject::GetRenderType()
    {
        return mRenderType;
    }

    bool WidgetRenderObject::GetCastShadows()
    {
        return false;
    }

    glm::vec3 WidgetRenderObject::GetWorldPosition()
    {
        return glm::vec3(0.0f, 0.0f, 0.0f);
    }
    
    void WidgetRenderObject::SetWidgetTree(WidgetTree* widgetTree)
    {
        mWidgetTree = widgetTree;
    }

    void WidgetRenderObject::SetRenderType(ERenderType renderType)
    {
        mRenderType = renderType;
    }
}
