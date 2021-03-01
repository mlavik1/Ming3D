#include "scene_renderer.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/norm.hpp"
#include "window_base.h"
#include "GameEngine/game_engine.h"
#include "render_device.h"
#include "Components/component.h"
#include "Actors/actor.h"
#include "Model/shader_uniform_data.h"
#include "forward_render_pipeline.h"
#include <algorithm>
#include "constant_buffer_data.h"
#include "Model/material_buffer.h"

namespace Ming3D
{
    using namespace Rendering;

    ConstantBufferData<glm::vec3, glm::vec4, glm::vec3, float> cbDataGlobal; // TODO

    SceneRenderer::SceneRenderer()
    {
        mRenderScene = new RenderScene();
        mFallbackPipeline = new ForwardRenderPipeline();
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

    void SceneRenderer::AddSceneObject(RenderObject* inObject)
    {
        mRenderScene->mSceneObjects.push_back(inObject);
    }

    void SceneRenderer::RegisterMaterial(MaterialBuffer* inMat)
    {
        // Set _Globals, if present (shaders need not use this)
        if(inMat->mConstantBuffers.find("_Globals") != inMat->mConstantBuffers.end())
            GGameEngine->GetRenderDevice()->BindConstantBuffer(mGlobalCBuffer, "_Globals", inMat->mShaderProgram);
    }

    void SceneRenderer::CollectVisibleObjects(const RenderPipelineContext& context, RenderPipeline* renderPipeline, RenderPipelineParams& params)
    {
        glm::vec3 camPos = glm::inverse(context.mMainCamera->mCameraMatrix) * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);

        int iNode = 0;

        // Add batches from RenderObjects
        for (RenderObject* obj : context.mScene->mSceneObjects)
        {
            int numBatches = obj->GetNumBatches();

            // Get batches of RenderObject
            for (int iBatch = 0; iBatch < numBatches; iBatch++)
            {
                // TODO: view frustum culling

                RenderPipelineNode* node = nullptr;

                node = params.mVisibleNodes.push_back();
                if (obj->GetRenderType() == ERenderType::Opaque)
                    params.mOpaqueNodeIndices.push_back(iNode++);
                else
                    params.mTransparentNodeIndices.push_back(iNode++);

                // Get batch
                obj->GetRenderBatch(iBatch, &node->mRenderBatch);
                node->mRenderOrderOffset = iBatch; // will ensure that transparent batches are rendered in order

                node->mRenderType = obj->GetRenderType();
                node->mSquareDistance = glm::length2(glm::vec3(obj->GetWorldPosition()) - camPos);
            }
        }
    }

    void SceneRenderer::Render()
    {
        RenderDevice* renderDevice = GGameEngine->GetRenderDevice();

        LightSource* mainLight = nullptr;
        if(!mLightSources.empty())
            mainLight = mLightSources.back(); // TODO: support multiple light sources

        for (Camera* camera : mCameras)
        {
            if (camera->mRenderTarget == nullptr)
                continue;

            RenderWindow* renderWindow = camera->mRenderTarget->GetRenderWindow();

            // Is render target a window? => begin window rendering
            if(renderWindow != nullptr)
                renderDevice->BeginRenderWindow(renderWindow);

            RenderPipelineContext context;
            context.mScene = mRenderScene;
            context.mMainCamera = camera;
            context.mMainLight = mainLight;

            // Setup parameters for render pipeline
            mPipelineParams.mVisibleNodes.clear();
            mPipelineParams.mOpaqueNodeIndices.clear();
            mPipelineParams.mTransparentNodeIndices.clear();

            // Collect renderable objects in the scene
            CollectVisibleObjects(context, camera->mRenderPipeline, mPipelineParams);

            // Set per-camera constant buffer data
            cbDataGlobal.SetData(glm::vec3(0.0f, -1.0f, 0.0f)/* TODO */, glm::vec4(0.8f, 0.8f, 0.8f, 1.0f), camera->mCameraMatrix * glm::vec4(0.0f, 0.0f, 0.0f, 0.0f), GGameEngine->GetTime());
            GGameEngine->GetRenderDevice()->SetConstantBufferData(mGlobalCBuffer, cbDataGlobal.mDataPtr, cbDataGlobal.mSize);

            // Render using specified render pipeline
            camera->mRenderPipeline->Render(context, mPipelineParams);

            if (renderWindow != nullptr)
                renderDevice->EndRenderWindow(renderWindow);
        }
    }
}
