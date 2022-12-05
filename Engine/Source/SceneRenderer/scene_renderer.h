#ifndef MING3D_SCENERENDERER_H
#define MING3D_SCENERENDERER_H

#include "render_pipeline.h"

namespace Ming3D
{
    namespace Rendering
    {
        class ConstantBuffer;
    }

    class MaterialBuffer;
    class RenderScene;

    class SceneRenderer
    {
    private:
        Rendering::ConstantBuffer* mGlobalCBuffer;
        RenderPipeline* mFallbackPipeline;
        RenderPipelineParams mPipelineParams;

        void Render(RenderScene* renderScene, Camera* camera);

    public:
        SceneRenderer();
        ~SceneRenderer();

        void RegisterMaterial(MaterialBuffer* inMat);
        
        void Initialise();

        void CollectVisibleObjects(const RenderPipelineContext& context, RenderPipeline* renderPipeline, RenderPipelineParams& params);

        void Render(std::vector<RenderScene*> renderScene);
    };
}

#endif
