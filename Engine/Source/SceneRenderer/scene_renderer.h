#ifndef MING3D_SCENERENDERER_H
#define MING3D_SCENERENDERER_H

#include "render_scene.h"

namespace Ming3D
{
    class SceneRenderer
    {
    private:
        RenderScene* mRenderScene;

    public:
        SceneRenderer();
        ~SceneRenderer();

        void AddSceneObject(RenderSceneObject* inObject);

        void RenderObjects();
    };
}

#endif
