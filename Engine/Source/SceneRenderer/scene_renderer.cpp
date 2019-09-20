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

namespace Ming3D
{
    struct RenderNodeSorter
    {
        inline bool operator() (const RenderPipelineNode* left, const RenderPipelineNode* right)
        {
            if (left->mMaterial != right->mMaterial)
                return (left->mMaterial < right->mMaterial); // TODO: use material ID?
            else if (left->mMesh != right->mMesh)
                return (left->mMesh < right->mMesh); // TODO: use mesh ID?
        }
    };

    SceneRenderer::SceneRenderer()
    {
        mRenderScene = new RenderScene();
        mRenderPipeline = new ForwardRenderPipeline();
    }

    SceneRenderer::~SceneRenderer()
    {
        delete mRenderPipeline;
        delete mRenderScene;
    }

    void SceneRenderer::AddCamera(Camera* inCamera)
    {
        mCameras.push_back(inCamera);
    }

    void SceneRenderer::RemoveCamera(Camera* inCamera)
    {
        mCameras.remove(inCamera);
    }

    void SceneRenderer::AddSceneObject(RenderSceneObject* inObject)
    {
        mRenderScene->mSceneObjects.push_back(inObject);
    }

    void SceneRenderer::Render()
    {
        for (Camera* camera : mCameras)
        {
            RenderPipelineParams* params = camera->mRenderPipelineParams;
            params->mCamera = camera;
            params->mNodes.clear();

            CollectObjects(*params);
            SortObjects(*params);

            mRenderPipeline->Render(*params);
        }
    }

    void SceneRenderer::CollectObjects(RenderPipelineParams& params)
    {
        for (RenderSceneObject* obj : mRenderScene->mSceneObjects)
        {
            RenderPipelineNode* node = params.mNodes.push_back();
            node->mMaterial = obj->mMaterial;
            node->mMesh = obj->mMesh;
            node->mModelMatrix = obj->mModelMatrix;
        }
    }

    void SceneRenderer::SortObjects(RenderPipelineParams& params)
    {
        RenderNodeSorter sorter;
        std::sort(params.mNodes.begin(), params.mNodes.end(), sorter);
    }
}
