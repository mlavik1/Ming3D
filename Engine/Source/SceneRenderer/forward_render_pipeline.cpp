#include "forward_render_pipeline.h"
#include "GameEngine/game_engine.h"
#include "render_device.h"
#include "Model/material_buffer.h"
#include "Model/shader_uniform_data.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

namespace Ming3D
{
    using namespace Rendering;

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

            glm::mat4 Projection = glm::perspective<float>(glm::radians(45.0f), (float)window->GetWidth() / (float)window->GetHeight(), 0.1f, 1000.0f);

            // matrices
            glm::mat4 view = params.mCamera->mCameraMatrix;
            glm::mat4 model = node->mModelMatrix;

            glm::mat4 mvp = Projection * view * model;
            glm::mat4 mv = view * model;

            renderDevice->SetShaderUniformMat4x4("MVP", mvp);
            renderDevice->SetShaderUniformMat4x4("modelViewMat", mv);

            // TODO: Don't bind vertex/index buffer if same mesh as last frame

            renderDevice->RenderPrimitive(node->mMesh->mVertexBuffer, node->mMesh->mIndexBuffer);

            nodeIter++;
        }
    }

    void ForwardRenderPipeline::Render(RenderPipelineParams& params)
    {
        if (params.mCamera->mRenderTarget == nullptr)
            return;
        GGameEngine->GetRenderDevice()->BeginRenderTarget(params.mCamera->mRenderTarget);
        RenderObjects(params);
        GGameEngine->GetRenderDevice()->EndRenderTarget(params.mCamera->mRenderTarget);
    }
}
