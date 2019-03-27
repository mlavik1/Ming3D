#ifndef MING3D_SCENERENDERER_H
#define MING3D_SCENERENDERER_H

#include "render_scene.h"
#include "camera.h"

namespace Ming3D
{
    class SceneRenderer
    {
    private:
        RenderScene* mRenderScene;
        std::vector<Camera*> mCameras;

        void UpdateUniforms(MaterialBuffer* inMat);

    public:
        SceneRenderer();
        ~SceneRenderer();

        void AddCamera(Camera* inCamera);
        void AddSceneObject(RenderSceneObject* inObject);

        void RenderCameras();
        void RenderObjects();
    };
}

#endif
