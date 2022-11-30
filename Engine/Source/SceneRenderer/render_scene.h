#ifndef MING3D_RENDERSCENE_H
#define MING3D_RENDERSCENE_H

#include <vector>
#include <list>
#include "render_object.h"

namespace Ming3D
{
    class RenderObject;
    class Camera;
    class LightSource;

    class RenderScene
    {
        friend class SceneRenderer;
    private:
        std::list<RenderObject*> mSceneObjects;
        std::list<Camera*> mCameras;
        std::list<LightSource*> mLightSources;

    public:
        RenderScene();
        ~RenderScene();
        void AddCamera(Camera* inCamera);
        void RemoveCamera(Camera* inCamera);
        void AddLightSource(LightSource* light);
        void RemoveLightSource(LightSource* light);
        void AddSceneObject(RenderObject* inObject);
        void RemoveSceneObject(RenderObject* inObject);
    };
}

#endif
