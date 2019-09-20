#ifndef MING3D_FORWARDRENDERPIPELINE_H
#define MING3D_FORWARDRENDERPIPELINE_H

#include "render_pipeline.h"

namespace Ming3D
{
    class MaterialBuffer;

    class ForwardRenderPipeline : public RenderPipeline
    {
    private:
        void UpdateUniforms(MaterialBuffer* inMat);
        void RenderObjects(RenderPipelineParams& params);

    public:
        virtual void Render(RenderPipelineParams& params) override;
    };
}

#endif
