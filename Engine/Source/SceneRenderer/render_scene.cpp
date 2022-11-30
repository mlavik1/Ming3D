#include "render_scene.h"

namespace Ming3D
{
    RenderScene::RenderScene()
    {

    }

    RenderScene::~RenderScene()
    {

    }

    void RenderScene::AddCamera(Camera* inCamera)
    {
        mCameras.push_back(inCamera);
    }

    void RenderScene::RemoveCamera(Camera* inCamera)
    {
        mCameras.remove(inCamera);
    }

    void RenderScene::AddLightSource(LightSource* light)
    {
        mLightSources.push_back(light);
    }

    void RenderScene::RemoveLightSource(LightSource* light)
    {
        mLightSources.remove(light);
    }

    void RenderScene::AddSceneObject(RenderObject* inObject)
    {
        mSceneObjects.push_back(inObject);
    }

    void RenderScene::RemoveSceneObject(RenderObject* inObject)
    {
        mSceneObjects.remove(inObject);
    }
}
