#ifndef MING3D_SCENERENDERER_H
#define MING3D_SCENERENDERER_H

#include "render_scene.h"
#include "camera.h"
#include <list>
#include "render_pipeline.h"

namespace Ming3D
{
    class ConstantBuffer;

    class SceneRenderer
    {
    private:
        RenderScene* mRenderScene;
        std::list<Camera*> mCameras;
        RenderPipeline* mRenderPipeline;
        ConstantBuffer* mGlobalCBuffer;

        void UpdateUniforms(MaterialBuffer* inMat);

    public:
        SceneRenderer();
        ~SceneRenderer();

        void Initialise();

        void AddCamera(Camera* inCamera);
        void RemoveCamera(Camera* inCamera);
        void AddSceneObject(RenderSceneObject* inObject);
        void RegisterMaterial(MaterialBuffer* inMat);

        void Render();
        void CollectObjects(RenderPipelineParams& params);
        void SortObjects(RenderPipelineParams& params);
        void RenderCameras();
        void RenderObjects(Camera* camera);
    };
}

#endif
