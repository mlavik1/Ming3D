#include "forward_render_pipeline.h"
#include "GameEngine/game_engine.h"
#include "render_device.h"
#include "Model/material_buffer.h"
#include "Model/shader_uniform_data.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "render_scene.h"
#include "glm/gtx/norm.hpp"
#include <algorithm>

#define MING3D_SHADOWRT_W 1366
#define MING3D_SHADOWRT_H 768

namespace Ming3D
{
    using namespace Rendering;

    class OpaqueNodeSorter
    {
    private:
        RenderPipelineNodeCollection* mNodeCollection;

    public:
        OpaqueNodeSorter(RenderPipelineNodeCollection* nodeCollection)
        {
            this->mNodeCollection = nodeCollection;
        }

        inline bool operator() (const unsigned int leftIndex, const unsigned int rightIndex)
        {
            RenderPipelineNode* leftNode = mNodeCollection->at(leftIndex);
            RenderPipelineNode* rightNode = mNodeCollection->at(rightIndex);
            // sort by material
            if (leftNode->mMaterial != rightNode->mMaterial)
                return (leftNode->mMaterial < rightNode->mMaterial); // TODO: use material ID?
            // sort by mesh
            //else if (leftNode->mMesh != rightNode->mMesh)
            //    return (leftNode->mMesh < rightNode->mMesh); // TODO: use mesh ID?
            // sort front to back
                return (leftNode->mSquareDistance < rightNode->mSquareDistance);
        }
    };

    class TransparentNodeSorter
    {
    private:
        RenderPipelineNodeCollection* mNodeCollection;

    public:
        TransparentNodeSorter(RenderPipelineNodeCollection* nodeCollection)
        {
            this->mNodeCollection = nodeCollection;
        }

        inline bool operator() (const unsigned int leftIndex, const unsigned int rightIndex)
        {
            RenderPipelineNode* leftNode = mNodeCollection->at(leftIndex);
            RenderPipelineNode* rightNode = mNodeCollection->at(rightIndex);
            // sort back to front
            return (leftNode->mSquareDistance > rightNode->mSquareDistance);
        }
    };

    void ForwardRenderPipeline::SetupMainLight(const RenderPipelineContext& context)
    {
        if (context.mMainLight == nullptr || context.mMainLight->mShadowType == EShadowType::None)
            return;

        if (context.mMainLight->mLightCamera == nullptr)
        {
            Camera* lightCam = new Camera();
            lightCam->mCameraMatrix = context.mMainLight->mLightMat;
            lightCam->mProjectionMatrix = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, -10.0f, 20.0f);
            lightCam->mRenderPipelineParams = new RenderPipelineParams();
            lightCam->mRenderPipelineParams->mCamera = lightCam;
            lightCam->mRenderPipelineParams->mIsShadowPass = true;
            RenderDevice* renderdev = GGameEngine->GetRenderDevice();
            TextureInfo rtinfo;
            rtinfo.mWidth = MING3D_SHADOWRT_W;
            rtinfo.mHeight = MING3D_SHADOWRT_H;
            rtinfo.mPixelFormat = PixelFormat::RGBA;
            rtinfo.mBytesPerPixel = 4;
            lightCam->mRenderTarget = renderdev->CreateRenderTarget(rtinfo, 1);
            context.mMainLight->mLightCamera = lightCam;
        }
    }

    void ForwardRenderPipeline::CollectVisibleObjects(const RenderPipelineContext& context, RenderPipelineParams& params)
    {
        params.mVisibleNodes.clear();

        glm::vec3 camPos = glm::inverse(params.mCamera->mCameraMatrix) * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);

        for (RenderSceneObject* obj : context.mScene->mSceneObjects)
        {
			ERenderType nodeRenderType = obj->mMaterial->mRenderType;
			
			if (params.mIsShadowPass && nodeRenderType == ERenderType::Transparent)
                continue;
            if (params.mIsShadowPass && !obj->mMaterial->mCastShadows)
                continue;

			// TODO: view frustum culling

			RenderPipelineNode* node = params.mVisibleNodes.push_back();
            node->mMaterial = obj->mMaterial;
            node->mMesh = obj->mMesh;
            node->mModelMatrix = obj->mModelMatrix;
            node->mRenderType = obj->mMaterial->mRenderType;
            node->mSquareDistance = glm::length2(glm::vec3(obj->mModelMatrix * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)) - camPos);
        }
    }

    void ForwardRenderPipeline::SetupNodeIndices(RenderPipelineParams& params)
    {
        params.mOpaqueNodeIndices.clear();
        params.mTransparentNodeIndices.clear();

        for (size_t iNode = 0; iNode < params.mVisibleNodes.size(); iNode++)
        {
            RenderPipelineNode* node = params.mVisibleNodes.at(iNode);
            switch (node->mRenderType)
            {
            case ERenderType::Opaque:
            {
                params.mOpaqueNodeIndices.push_back(iNode);
                break;
            }
            case ERenderType::Transparent:
            {
                params.mTransparentNodeIndices.push_back(iNode);
                break;
            }
            }
        }

        // sort opaque nodes
        OpaqueNodeSorter opaqueSorter(&params.mVisibleNodes);
        std::sort(params.mOpaqueNodeIndices.begin(), params.mOpaqueNodeIndices.end(), opaqueSorter);

        // sort transparent nodes
        TransparentNodeSorter transparentSorter(&params.mVisibleNodes);
        std::sort(params.mTransparentNodeIndices.begin(), params.mTransparentNodeIndices.end(), transparentSorter);
    }

    void ForwardRenderPipeline::UpdateUniforms(MaterialBuffer* inMat)
    {
        RenderDevice* renderDevice = GGameEngine->GetRenderDevice();

        for (const std::string& uniformName : inMat->mModifiedUniforms)
        {
            auto it = inMat->mShaderUniformMap.find(uniformName);
            assert(it != inMat->mShaderUniformMap.end());
            const ShaderUniformData* uniformData = it->second;
            switch (uniformData->mTypeInfo.mDatatype)
            {
            case EShaderDatatype::Float:
            {
                float val;
                uniformData->GetData(&val);
                renderDevice->SetShaderUniformFloat(uniformName.c_str(), val); // TODO: pass pointer instead of uniform name string?
                break;
            }
            case EShaderDatatype::Int:
            {
                int val;
                uniformData->GetData(&val);
                renderDevice->SetShaderUniformInt(uniformName.c_str(), val); // TODO: pass pointer instead of uniform name string?
                break;
            }
            case EShaderDatatype::Mat4x4:
            {
                glm::mat4 val;
                uniformData->GetData(&val);
                renderDevice->SetShaderUniformMat4x4(uniformName.c_str(), val); // TODO: pass pointer instead of uniform name string?
                break;
            }
            case EShaderDatatype::Vec2:
            {
                glm::vec2 val;
                uniformData->GetData(&val);
                renderDevice->SetShaderUniformVec2(uniformName.c_str(), val); // TODO: pass pointer instead of uniform name string?
                break;
            }
            case EShaderDatatype::Vec3:
            {
                glm::vec3 val;
                uniformData->GetData(&val);
                renderDevice->SetShaderUniformVec3(uniformName.c_str(), val); // TODO: pass pointer instead of uniform name string?
                break;
            }
            case EShaderDatatype::Vec4:
            {
                glm::vec4 val;
                uniformData->GetData(&val);
                renderDevice->SetShaderUniformVec4(uniformName.c_str(), val); // TODO: pass pointer instead of uniform name string?
                break;
            }
            default:
                assert(0); // TODO: add support for other types
            }
        }
        inMat->mModifiedUniforms.clear();
    }

    void ForwardRenderPipeline::RenderObjects(RenderPipelineParams& params, ERenderType renderType, LightSource* mainLightSource)
    {
        WindowBase* window = GGameEngine->GetMainWindow();
        RenderDevice* renderDevice = GGameEngine->GetRenderDevice();

        MaterialBuffer* currMaterial = nullptr;

        std::vector<unsigned int>* nodeIndices;
        switch (renderType)
        {
        case ERenderType::Opaque:
            nodeIndices = &params.mOpaqueNodeIndices;
            break;
        case ERenderType::Transparent:
            nodeIndices = &params.mTransparentNodeIndices;
            break;
        }

        for (unsigned int nodeIndex : *nodeIndices)
        {
            RenderPipelineNode* node = params.mVisibleNodes.at(nodeIndex);

            assert(node->mMaterial != nullptr);

            // if new material, update per-material data
            if (node->mMaterial != currMaterial)
            {
                currMaterial = node->mMaterial;

                // set shader program
                renderDevice->SetActiveShaderProgram(currMaterial->mShaderProgram);

                // set textures
                for (size_t iTexture = 0; iTexture < currMaterial->mTextureBuffers.size(); iTexture++)
                {
                    const TextureBuffer* texture = currMaterial->mTextureBuffers[iTexture];
                    if (texture != nullptr)
                        renderDevice->SetTexture(texture, iTexture); // temp
                }
                // set shadowmap depth texture
                if (mainLightSource != nullptr && mainLightSource->mLightCamera != nullptr)
                {
                    size_t depthTexSlotID = currMaterial->GetTextureID("depthTexture");
                    renderDevice->SetTexture(mainLightSource->mLightCamera->mRenderTarget->GetDepthTextureBuffer(), depthTexSlotID);
                }

                // update uniforms
                UpdateUniforms(currMaterial);
            }

            glm::mat4 Projection = params.mCamera->mProjectionMatrix;

            // matrices
            glm::mat4 view = params.mCamera->mCameraMatrix;
            glm::mat4 model = node->mModelMatrix;

            glm::mat4 mvp = Projection * view * model;

            renderDevice->SetShaderUniformMat4x4("MVP", mvp);
            renderDevice->SetShaderUniformMat4x4("modelMat", model);

            // set light matrix
            if (mainLightSource != nullptr && mainLightSource->mLightCamera != nullptr)
            {
                glm::mat4 lightVPMat = mainLightSource->mLightCamera->mProjectionMatrix * mainLightSource->mLightCamera->mCameraMatrix;
                glm::mat4 lightMVP = lightVPMat * model;
#ifdef MING3D_D3D11
				glm::mat4 biasMatrix(0.5, 0.0, 0.0, 0.0,
					0.0, 0.5, 0.0, 0.0,
					0.0, 0.0, 1.0, 0.0,
					0.5, 0.5, 0.0, 1.0);
#else
                glm::mat4 biasMatrix(   0.5, 0.0, 0.0, 0.0,
                                        0.0, 0.5, 0.0, 0.0,
                                        0.0, 0.0, 0.5, 0.0,
                                        0.5, 0.5, 0.5, 1.0);
#endif
                glm::mat4 lightMat = biasMatrix * lightMVP;
                renderDevice->SetShaderUniformMat4x4("lightMat", lightMat);
            }

            // TODO: Don't bind vertex/index buffer if same mesh as last frame

            renderDevice->RenderPrimitive(node->mMesh->mVertexBuffer, node->mMesh->mIndexBuffer);
        }
    }

    void ForwardRenderPipeline::Render(const RenderPipelineContext& context, RenderPipelineParams& params)
    {
        if (params.mCamera->mRenderTarget == nullptr)
            return;

        SetupMainLight(context);

        if(context.mMainLight != nullptr && context.mMainLight->mShadowType != EShadowType::None)
        {
            // set light projection matrix
            context.mMainLight->mLightCamera->mProjectionMatrix = glm::ortho(-15.0f, 15.0f, -15.0f, 15.0f, -50.1f, 50.0f);

            // set light view matrix
            glm::vec3 lightDir = glm::normalize(context.mMainLight->mLightMat * glm::vec4(0.0f, 0.0f, -1.0f, 0.0f));
            glm::vec3 lookTarget = glm::inverse(context.mMainCamera->mCameraMatrix) * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
            glm::vec3 lightpos = lookTarget - lightDir; // TODO
            context.mMainLight->mLightCamera->mCameraMatrix = glm::lookAt(lightpos, lookTarget, glm::vec3(0.0f, 0.0f, 1.0f));

            CollectVisibleObjects(context, *context.mMainLight->mLightCamera->mRenderPipelineParams);
            SetupNodeIndices(*context.mMainLight->mLightCamera->mRenderPipelineParams);

            GGameEngine->GetRenderDevice()->BeginRenderTarget(context.mMainLight->mLightCamera->mRenderTarget);
            RenderObjects(*context.mMainLight->mLightCamera->mRenderPipelineParams, ERenderType::Opaque, nullptr);
            GGameEngine->GetRenderDevice()->EndRenderTarget(context.mMainLight->mLightCamera->mRenderTarget);
        }

        // set camera projection matrix
        WindowBase* window = GGameEngine->GetMainWindow(); // TODO
        params.mCamera->mProjectionMatrix = glm::perspective<float>(glm::radians(45.0f), (float)window->GetWidth() / (float)window->GetHeight(), 0.1f, 1000.0f);

        CollectVisibleObjects(context, params);
        SetupNodeIndices(params);

        GGameEngine->GetRenderDevice()->BeginRenderTarget(params.mCamera->mRenderTarget);
        RenderObjects(params, ERenderType::Opaque, context.mMainLight);
        RenderObjects(params, ERenderType::Transparent, context.mMainLight);
        GGameEngine->GetRenderDevice()->EndRenderTarget(params.mCamera->mRenderTarget);
    }
}
