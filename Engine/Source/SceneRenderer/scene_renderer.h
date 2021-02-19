#ifndef MING3D_SCENERENDERER_H
#define MING3D_SCENERENDERER_H

#include "render_scene.h"
#include "camera.h"
#include "Light/light_source.h"
#include <list>
#include "render_pipeline.h"

namespace Ming3D
{
    namespace Rendering
    {
        class ConstantBuffer;
    }

    class MaterialBuffer;

    class SceneRenderer
    {
    private:
        RenderScene* mRenderScene;
        std::list<Camera*> mCameras;
        std::list<LightSource*> mLightSources;
        Rendering::ConstantBuffer* mGlobalCBuffer;
        RenderPipeline* mFallbackPipeline;
        RenderPipelineParams mPipelineParams;

    public:
        SceneRenderer();
        ~SceneRenderer();

        void Initialise();

        void AddCamera(Camera* inCamera);
        void RemoveCamera(Camera* inCamera);
        void AddLightSource(LightSource* light);
        void RemoveLightSource(LightSource* light);
        void AddSceneObject(RenderObject* inObject);
        void RegisterMaterial(MaterialBuffer* inMat);

        void CollectVisibleObjects(const RenderPipelineContext& context, RenderPipeline* renderPipeline, RenderPipelineParams& params);

        void Render();
    };
}

#endif
