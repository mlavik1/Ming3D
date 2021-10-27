#ifndef MING3D_FORWARDRENDERPIPELINE_H
#define MING3D_FORWARDRENDERPIPELINE_H

#include "render_pipeline.h"

namespace Ming3D
{
    class MaterialBuffer;
    namespace Rendering
    {
        class RenderTarget;
        class BlendState;
        class DepthStencilState;
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

        Rendering::BlendState* mOpaqueBlendState = nullptr; // TODO: allow custom blend states
        Rendering::BlendState* mTransparentBlendState = nullptr; // TODO: allow custom blend states
        Rendering::DepthStencilState* mOpaqueDepthStencilState = nullptr; // TODO: allow custom blend states
        Rendering::DepthStencilState* mTransparentDepthStencilState = nullptr; // TODO: allow custom blend states
        
        bool mInitialised = false;

    public:
        virtual void Render(const RenderPipelineContext& context, RenderPipelineParams& params) override;
    
        void Initialise();
    };
}

#endif
