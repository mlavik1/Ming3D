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

    class SceneRenderer
    {
    private:
        RenderScene* mRenderScene;
        std::list<Camera*> mCameras;
        std::list<LightSource*> mLightSources;
        Rendering::ConstantBuffer* mGlobalCBuffer;

        void UpdateUniforms(MaterialBuffer* inMat);

    public:
        SceneRenderer();
        ~SceneRenderer();

        void Initialise();

        void AddCamera(Camera* inCamera);
        void RemoveCamera(Camera* inCamera);
        void AddLightSource(LightSource* light);
        void RemoveLightSource(LightSource* light);
        void AddSceneObject(RenderSceneObject* inObject);
        void RegisterMaterial(MaterialBuffer* inMat);

        void Render();
        void RenderCameras();
        void RenderObjects(Camera* camera);
    };
}

#endif
