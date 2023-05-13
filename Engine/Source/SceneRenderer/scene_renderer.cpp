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

    bool CameraSortAsc(Camera* left, Camera* right)
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
            size_t numBatches = obj->GetNumBatches();

            // Get batches of RenderObject
            for (size_t iBatch = 0; iBatch < numBatches; iBatch++)
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
                node->mRenderOrderOffset = static_cast<int>(iBatch); // will ensure that transparent batches are rendered in order - TODO: Do something smarter

                node->mRenderType = obj->GetRenderType();
                node->mSquareDistance = glm::length2(glm::vec3(obj->GetWorldPosition()) - camPos);
            }
        }
    }

    void SceneRenderer::Render(const std::vector<RenderScene*>& scenes)
    {
        RenderDevice* renderDevice = GGameEngine->GetRenderDevice();
        // TODO: Clean up this mess
        std::vector<Camera*> cameras;
        for (auto scene : scenes)
        {
            std::copy_if(scene->mCameras.begin(), scene->mCameras.end(), std::back_inserter(cameras),
                [](const Camera* camera) { return camera->mRenderTarget != nullptr; });
        }
        std::sort(cameras.begin(), cameras.end(), CameraSortAsc);

        RenderWindow* currRendWnd = nullptr;
        RenderTarget* currRenderTarget = nullptr;
        for (auto camera : cameras)
        {
            // TODO: Only one RT per window?
            RenderWindow* rendWnd = camera->mRenderTarget->GetRenderWindow();
            if (camera->mRenderTarget.get() != currRenderTarget && currRendWnd != nullptr)
                renderDevice->BlitRenderTargetToWindow(currRenderTarget, currRendWnd);
            currRenderTarget = camera->mRenderTarget.get();
            if (rendWnd != nullptr && rendWnd != currRendWnd)
            {
                if (currRendWnd != nullptr)
                    renderDevice->EndRenderWindow(currRendWnd);
                renderDevice->BeginRenderWindow(rendWnd);
                currRendWnd = rendWnd;
            }

            // TODO: Do this (set RT and VP) in render pipeline?
            renderDevice->SetRenderTarget(camera->mRenderTarget.get());
            auto viewport = camera->GetAbsoluteViewport();
            renderDevice->BeginViewport(
                static_cast<unsigned int>(viewport.x), static_cast<unsigned int>(viewport.y),
                static_cast<unsigned int>(viewport.width), static_cast<unsigned int>(viewport.height)
            );

            // TODO: Probably won't have camera in more than one scene?
            for (auto scene : scenes)
            {
                if (std::any_of(
                    scene->mCameras.begin(), scene->mCameras.end(),
                    [&camera](const Camera* sceneCamera) { return sceneCamera == camera; }))
                {
                    Render(scene, camera);
                    break;
                }
            }
        }
        if (currRendWnd != nullptr)
            renderDevice->BlitRenderTargetToWindow(currRenderTarget, currRendWnd);
        if (currRendWnd != nullptr)
            renderDevice->EndRenderWindow(currRendWnd);
    }

    void SceneRenderer::Render(RenderScene* renderScene, Camera* camera)
    {
        LightSource* mainLight = nullptr;
        if(!renderScene->mLightSources.empty())
            mainLight = renderScene->mLightSources.back(); // TODO: support multiple light sources

        RenderPipelineContext context;
        context.mScene = renderScene;
        context.mMainCamera = camera;
        context.mMainLight = mainLight;

        static const glm::vec3 defaultLightDir = glm::normalize(glm::vec3(0.2f, -1.0f, 0.2f));
        const glm::vec3 lightDir = context.mMainLight != nullptr ? context.mMainLight->mDirection : defaultLightDir;

        // Setup parameters for render pipeline
        mPipelineParams.mVisibleNodes.clear();
        mPipelineParams.mOpaqueNodeIndices.clear();
        mPipelineParams.mTransparentNodeIndices.clear();
        mPipelineParams.mGUIOverlayNodeIndices.clear();

        // Collect renderable objects in the scene
        CollectVisibleObjects(context, camera->mRenderPipeline, mPipelineParams);

        // Set per-camera constant buffer data
        cbDataGlobal.SetData(lightDir/* TODO: Point lights */, glm::vec4(0.8f, 0.8f, 0.8f, 1.0f), camera->mPosition, GGameEngine->GetTime());
        GGameEngine->GetRenderDevice()->SetConstantBufferData(mGlobalCBuffer, cbDataGlobal.mDataPtr, cbDataGlobal.mSize);

        // Render using specified render pipeline
        camera->mRenderPipeline->Render(context, mPipelineParams);
    }
}
