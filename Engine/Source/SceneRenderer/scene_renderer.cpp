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
#include "constant_buffer_data.h"
#include "Model/material_buffer.h"
#include "Debug/st_assert.h"
#include "render_scene.h"
#include "Light/light_source.h"
#include <algorithm>

namespace Ming3D
{
    using namespace Rendering;

    ConstantBufferData<glm::vec3, glm::vec4, glm::vec3, float> cbDataGlobal; // TODO

    class CameraSorter
    {
    public:
        inline bool operator() (Camera* left, Camera* right)
        {
            auto leftRT = left->mRenderTarget;
            auto leftWnd = leftRT->GetRenderWindow();
            auto rightRT = right->mRenderTarget;
            auto rightWnd = rightRT->GetRenderWindow();
            // sort by window
            if (leftWnd != rightWnd)
                return (leftWnd < rightWnd); // TODO: use ID?
            // sort by render target
            else if(leftRT != rightRT)
                return (leftRT < rightRT); // TODO: use ID?
            else
                return (left->mRenderOrder < right->mRenderOrder);
        }
    };

    SceneRenderer::SceneRenderer()
    {
        mFallbackPipeline = new ForwardRenderPipeline();
    }

    SceneRenderer::~SceneRenderer()
    {
    }

    void SceneRenderer::Initialise()
    {
        cbDataGlobal.SetData(glm::vec3(), glm::vec4(), glm::vec3(), 0.0f);
        mGlobalCBuffer = GGameEngine->GetRenderDevice()->CreateConstantBuffer(cbDataGlobal.mSize);
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
                else if (obj->GetRenderType() == ERenderType::Transparent)
                    params.mTransparentNodeIndices.push_back(iNode++);
                else if (obj->GetRenderType() == ERenderType::GUIOverlay)
                    params.mGUIOverlayNodeIndices.push_back(iNode++);
                else
                    __AssertComment(false, "Unimplemented RenderType");

                // Get batch
                obj->GetRenderBatch(iBatch, &node->mRenderBatch);
                node->mRenderOrderOffset = iBatch; // will ensure that transparent batches are rendered in order

                node->mRenderType = obj->GetRenderType();
                node->mSquareDistance = glm::length2(glm::vec3(obj->GetWorldPosition()) - camPos);
            }
        }
    }

    void SceneRenderer::Render(std::vector<RenderScene*> scenes)
    {
        RenderDevice* renderDevice = GGameEngine->GetRenderDevice();
        
        std::vector<Camera*> cameras;
        for (auto scene : scenes)
        {
            for (auto camera : scene->mCameras){
                if (camera->mRenderTarget != nullptr)
                    cameras.push_back(camera);
            }
        }
        std::sort(cameras.begin(), cameras.end(), CameraSorter());

        RenderWindow* currRendWnd = nullptr;
        for (auto camera : cameras)
        {
            RenderWindow* rendWnd = camera->mRenderTarget->GetRenderWindow();
            if (rendWnd != nullptr && rendWnd != currRendWnd)
            {
                if (currRendWnd != nullptr)
                    renderDevice->EndRenderWindow(currRendWnd);
                renderDevice->BeginRenderWindow(rendWnd);
                currRendWnd = rendWnd;
            }

            auto viewport = camera->GetAbsoluteViewport();
            renderDevice->BeginViewport(viewport.x, viewport.y, viewport.x + viewport.width, viewport.y + viewport.height);

            // TODO: Probably won't have camera in more than one scene?
            for (auto scene : scenes)
            {
                for (auto sceneCamera : scene->mCameras){
                    if (sceneCamera == camera)
                    {
                        Render(scene, camera);
                        break;
                    }
                }
            }
        }
        if (currRendWnd != nullptr)
            renderDevice->EndRenderWindow(currRendWnd);
    }

    void SceneRenderer::Render(RenderScene* renderScene, Camera* camera)
    {
        RenderDevice* renderDevice = GGameEngine->GetRenderDevice();

        LightSource* mainLight = nullptr;
        if(!renderScene->mLightSources.empty())
            mainLight = renderScene->mLightSources.back(); // TODO: support multiple light sources

        RenderPipelineContext context;
        context.mScene = renderScene;
        context.mMainCamera = camera;
        context.mMainLight = mainLight;

        // Setup parameters for render pipeline
        mPipelineParams.mVisibleNodes.clear();
        mPipelineParams.mOpaqueNodeIndices.clear();
        mPipelineParams.mTransparentNodeIndices.clear();
        mPipelineParams.mGUIOverlayNodeIndices.clear();

        // Collect renderable objects in the scene
        CollectVisibleObjects(context, camera->mRenderPipeline, mPipelineParams);

        // Set per-camera constant buffer data
        cbDataGlobal.SetData(glm::vec3(0.0f, -1.0f, 0.0f)/* TODO */, glm::vec4(0.8f, 0.8f, 0.8f, 1.0f), camera->mCameraMatrix * glm::vec4(0.0f, 0.0f, 0.0f, 0.0f), GGameEngine->GetTime());
        GGameEngine->GetRenderDevice()->SetConstantBufferData(mGlobalCBuffer, cbDataGlobal.mDataPtr, cbDataGlobal.mSize);

        // Render using specified render pipeline
        camera->mRenderPipeline->Render(context, mPipelineParams);
    }
}
