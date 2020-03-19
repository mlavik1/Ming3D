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
        Rendering::RenderTarget* mShadowsRT = nullptr;
        glm::mat4 lightPorjMat; // TODO

        void UpdateUniforms(MaterialBuffer* inMat);
        void RenderObjects(RenderPipelineParams& params);
        void RenderShadowCasters(RenderPipelineParams& params);

    public:
        virtual void Initialise() override;
        virtual void Render(RenderPipelineParams& params) override;
    };
}

#endif
