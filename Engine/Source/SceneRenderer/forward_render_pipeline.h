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
        // TODO: Move to base class
        void SetupMainLight(const RenderPipelineContext& context);
        void SortNodeIndices(RenderPipelineParams& params);

        // TODO: Move to base class
        void UpdateUniforms(MaterialBuffer* inMat);
        void RenderObjects(RenderPipelineParams& params, ERenderType renderType, Camera* camera, LightSource* mainLightSource, bool shadowPass = false);

    public:
        virtual void Render(const RenderPipelineContext& context, RenderPipelineParams& params) override;
    };
}

#endif
