#include "forward_render_pipeline.h"
#include "GameEngine/game_engine.h"
#include "render_device.h"
#include "Model/material_buffer.h"
#include "Model/shader_uniform_data.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#define MING3D_SHADOWRT_W 1366
#define MING3D_SHADOWRT_H 768

namespace Ming3D
{
    using namespace Rendering;

    void ForwardRenderPipeline::Initialise()
    {
        RenderDevice* renderdev = GGameEngine->GetRenderDevice();
        TextureInfo rtinfo;
        rtinfo.mWidth = MING3D_SHADOWRT_W;
        rtinfo.mHeight = MING3D_SHADOWRT_H;
        rtinfo.mPixelFormat = PixelFormat::RGBA;
        rtinfo.mBytesPerPixel = 4;
        mShadowsRT = renderdev->CreateRenderTarget(rtinfo, 1);
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

    void ForwardRenderPipeline::RenderObjects(RenderPipelineParams& params)
    {
        WindowBase* window = GGameEngine->GetMainWindow();
        RenderDevice* renderDevice = GGameEngine->GetRenderDevice();

        MaterialBuffer* currMaterial = nullptr;

        auto nodeIter = params.mNodes.begin();
        while (nodeIter != params.mNodes.end())
        {
            RenderPipelineNode* node = *nodeIter;

            assert(node->mMaterial != nullptr);

            // if new material, update per-material data
            if (node->mMaterial != currMaterial)
            {
                currMaterial = node->mMaterial;
                
                size_t depthTexID = currMaterial->GetTextureID("inDepthTexture");

                // set shader program
                renderDevice->SetActiveShaderProgram(currMaterial->mShaderProgram);

                // set textures
                for (size_t iTexture = 0; iTexture < currMaterial->mTextureBuffers.size(); iTexture++)
                {
                    const TextureBuffer* texture = currMaterial->mTextureBuffers[iTexture];
                    if (texture != nullptr)
                        renderDevice->SetTexture(texture, iTexture); // temp
                }
                // set depth texture
                renderDevice->SetTexture(mShadowsRT->GetDepthTextureBuffer(), depthTexID); // TEMP TEST

                // update uniforms
                UpdateUniforms(currMaterial);
            }

            glm::mat4 Projection = glm::perspective<float>(glm::radians(45.0f), (float)window->GetWidth() / (float)window->GetHeight(), 0.1f, 1000.0f);

            // matrices
            glm::mat4 view = params.mCamera->mCameraMatrix;
            glm::mat4 model = node->mModelMatrix;

            glm::mat4 mvp = Projection * view * model;

            glm::mat4 lightMVP = lightPorjMat * model;
            glm::mat4 biasMatrix(
            0.5, 0.0, 0.0, 0.0,
            0.0, 0.5, 0.0, 0.0,
            0.0, 0.0, 0.5, 0.0,
            0.5, 0.5, 0.5, 1.0
            );
            glm::mat4 lightMat = biasMatrix * lightMVP;

            renderDevice->SetShaderUniformMat4x4("MVP", mvp);
            renderDevice->SetShaderUniformMat4x4("modelMat", model);
            renderDevice->SetShaderUniformMat4x4("lightMat", lightMat);

            // TODO: Don't bind vertex/index buffer if same mesh as last frame

            renderDevice->RenderPrimitive(node->mMesh->mVertexBuffer, node->mMesh->mIndexBuffer);

            nodeIter++;
        }
    }

    void ForwardRenderPipeline::RenderShadowCasters(RenderPipelineParams& params)
    {
        RenderDevice* renderDevice = GGameEngine->GetRenderDevice();

        MaterialBuffer* currMaterial = nullptr;

        auto nodeIter = params.mNodes.begin();
        while (nodeIter != params.mNodes.end())
        {
            RenderPipelineNode* node = *nodeIter;

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

                // update uniforms
                UpdateUniforms(currMaterial);
            }

            glm::mat4 Projection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, 0.1f, 20.0f);

            // matrices
            glm::vec3 lookTarget = params.mCamera->mCameraMatrix * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
            glm::vec3 lightpos = lookTarget - params.mMainLightDir * 15.0f;
            glm::mat4 view = glm::lookAt(lightpos, lookTarget, glm::vec3(0.0f, 0.0f, 1.0f));
            glm::mat4 model = node->mModelMatrix;

            glm::mat4 mvp = Projection * view * model;
            lightPorjMat = Projection * view;

            renderDevice->SetShaderUniformMat4x4("MVP", mvp);
            renderDevice->SetShaderUniformMat4x4("modelMat", model);

            // TODO: Don't bind vertex/index buffer if same mesh as last frame

            renderDevice->RenderPrimitive(node->mMesh->mVertexBuffer, node->mMesh->mIndexBuffer);

            nodeIter++;
        }
    }

    void ForwardRenderPipeline::Render(RenderPipelineParams& params)
    {
        if (params.mCamera->mRenderTarget == nullptr)
            return;

        GGameEngine->GetRenderDevice()->BeginRenderTarget(mShadowsRT);
        RenderShadowCasters(params);
        GGameEngine->GetRenderDevice()->EndRenderTarget(mShadowsRT);


        GGameEngine->GetRenderDevice()->BeginRenderTarget(params.mCamera->mRenderTarget);
        RenderObjects(params);
        GGameEngine->GetRenderDevice()->EndRenderTarget(params.mCamera->mRenderTarget);
    }
}
