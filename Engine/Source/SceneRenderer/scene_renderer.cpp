#include "scene_renderer.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "window_base.h"
#include "GameEngine/game_engine.h"
#include "render_device.h"
#include "Components/component.h"
#include "Actors/actor.h"
#include "Model/shader_uniform_data.h"
#include "forward_render_pipeline.h"
#include <algorithm>
#include "constant_buffer_data.h"

namespace Ming3D
{
    using namespace Rendering;

    ConstantBufferData<glm::vec3, glm::vec4, glm::vec3, float> cbDataGlobal; // TODO

    SceneRenderer::SceneRenderer()
    {
        mRenderScene = new RenderScene();
    }

    SceneRenderer::~SceneRenderer()
    {
        delete mRenderScene;
    }

    void SceneRenderer::Initialise()
    {
        cbDataGlobal.SetData(glm::vec3(), glm::vec4(), glm::vec3(), 0.0f);
        mGlobalCBuffer = GGameEngine->GetRenderDevice()->CreateConstantBuffer(cbDataGlobal.mSize);
    }

    void SceneRenderer::AddCamera(Camera* inCamera)
    {
        mCameras.push_back(inCamera);
    }

    void SceneRenderer::RemoveCamera(Camera* inCamera)
    {
        mCameras.remove(inCamera);
    }

    void SceneRenderer::AddLightSource(LightSource* light)
    {
        mLightSources.push_back(light);
    }

    void SceneRenderer::RemoveLightSource(LightSource* light)
    {
        mLightSources.remove(light);
    }

    void SceneRenderer::AddSceneObject(RenderSceneObject* inObject)
    {
        mRenderScene->mSceneObjects.push_back(inObject);
    }

    void SceneRenderer::RegisterMaterial(MaterialBuffer* inMat)
    {
        // Set _Globals, if present (shaders need not use this)
        if(inMat->mConstantBuffers.find("_Globals") != inMat->mConstantBuffers.end())
            GGameEngine->GetRenderDevice()->BindConstantBuffer(mGlobalCBuffer, "_Globals", inMat->mShaderProgram);
    }

    void SceneRenderer::Render()
    {
        LightSource* mainLight = nullptr;
        if(!mLightSources.empty())
            mainLight = mLightSources.back(); // TODO: support multiple light sources

        for (Camera* camera : mCameras)
        {
            RenderPipelineContext context;
            context.mScene = mRenderScene;
            context.mMainCamera = camera;
            context.mMainLight = mainLight;

            RenderPipelineParams* params = camera->mRenderPipelineParams;
            params->mCamera = camera;
            params->mVisibleNodes.clear();

            //CollectObjects(*params);
            //SortObjects(*params);

            cbDataGlobal.SetData(glm::vec3(0.0f, -1.0f, 0.0f)/* TODO */, glm::vec4(0.8f, 0.8f, 0.8f, 1.0f), camera->mCameraMatrix * glm::vec4(0.0f, 0.0f, 0.0f, 0.0f), GGameEngine->GetTime());
            GGameEngine->GetRenderDevice()->SetConstantBufferData(mGlobalCBuffer, cbDataGlobal.mDataPtr, cbDataGlobal.mSize);

            camera->mRenderPipeline->Render(context, *params);
        }
    }
}
