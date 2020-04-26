#ifndef MING3D_FORWARDRENDERPIPELINE_H
#define MING3D_FORWARDRENDERPIPELINE_H

#include "render_pipeline.h"

namespace Ming3D
{
    class MaterialBuffer;
    namespace Rendering
    {
        class RenderTarget;
    }

    class ForwardRenderPipeline : public RenderPipeline
    {
    private:
        void SetupMainLight(const RenderPipelineContext& context);
        void CollectVisibleObjects(const RenderPipelineContext& context, RenderPipelineParams& params);
        void SetupNodeIndices(RenderPipelineParams& params);

        void UpdateUniforms(MaterialBuffer* inMat);
        void RenderObjects(RenderPipelineParams& params, ERenderType renderType, LightSource* mainLightSource);

    public:
        virtual void Render(const RenderPipelineContext& context, RenderPipelineParams& params) override;
    };
}

#endif
