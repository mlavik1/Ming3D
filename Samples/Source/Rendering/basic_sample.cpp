#include "basic_sample.h"
#include "Debug/debug.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "shader_parser.h"
#include "GameEngine/game_engine.h"

namespace Ming3D
{
    void BasicSample::init()
    {
        SampleBase::init();

        mRenderTarget = mRenderDevice->CreateRenderTarget(mRenderWindow);

        TextureInfo texInfo;
        texInfo.mWidth = mMainWindow->GetWidth();
        texInfo.mHeight = mMainWindow->GetHeight();

        mModels.push_back(LoadModel(GGameEngine->GetResourceDirectory() + std::string("/Mvr_PetCow_walk.dae")));
        mModels.push_back(LoadModel(GGameEngine->GetResourceDirectory() + std::string("/test.dae")));

        mModels[0]->mPosition = glm::vec3(2.0f, 0.0f, 0.0f);
        mModels[1]->mPosition = glm::vec3(-2.0f, 0.0f, 0.0f);

        for (ModelData* modelData : mModels)
        {
            for (MeshData* meshData : modelData->mMeshes)
            {
                Rendering::VertexData vertexData({ Rendering::EVertexComponent::Position, Rendering::EVertexComponent::Normal, Rendering::EVertexComponent::TexCoord }, meshData->mVertices.size());
                Rendering::IndexData indexData(meshData->mIndices.size());

                memcpy(vertexData.GetDataPtr(), meshData->mVertices.data(), meshData->mVertices.size() * sizeof(Vertex));
                memcpy(indexData.GetData(), meshData->mIndices.data(), meshData->mIndices.size() * sizeof(unsigned int));

                meshData->mVertexBuffer = mRenderDevice->CreateVertexBuffer(&vertexData, Rendering::EBufferUsage::StaticDraw);
                meshData->mIndexBuffer = mRenderDevice->CreateIndexBuffer(&indexData, Rendering::EBufferUsage::StaticDraw);

                if (meshData->mTexture != nullptr)
                {
                    meshData->mTextureBuffer = mRenderDevice->CreateTextureBuffer(meshData->mTexture->GetTextureInfo(), meshData->mTexture->GetTextureData());
                }
            }

            // TODO: Use different shaders, based on vertex layout?
            Rendering::ShaderParser parser;
            Rendering::ShaderParserParams params;
            params.mShaderProgramPath = GGameEngine->GetResourceDirectory() + std::string("/Shaders/defaultshader.cgp");
            Rendering::ParsedShaderProgram* prog = parser.ParseShaderProgram(params);
            modelData->mShaderProgram = mRenderDevice->CreateShaderProgram(prog);
        }

    }

    void BasicSample::tick()
    {
        SampleBase::tick();

        // TODO:
        float width = 800.0f;
        float height = 600.0f;

        mRenderDevice->BeginRenderWindow(mRenderWindow);
        mRenderDevice->SetRenderTarget(mRenderTarget.get());

        for (ModelData* modelData : mModels)
        {
            glm::mat4 Projection = glm::perspective<float>(glm::radians(45.0f), width / height, 0.1f, 100.0f);

            // Camera matrix
            glm::mat4 View = glm::lookAt(
                glm::vec3(0, 2, 6), // pos
                glm::vec3(0, 0, 0), // lookat
                glm::vec3(0, 1, 0)  // up
            );

            glm::mat4 Model = glm::mat4(1.0f);
            Model = glm::translate(Model, modelData->mPosition);

            glm::mat4 mvp = Projection * View * Model;

            mRenderDevice->SetActiveShaderProgram(modelData->mShaderProgram);

            mRenderDevice->SetShaderUniformMat4x4("MVP", mvp);
            mRenderDevice->SetShaderUniformVec4("test", glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));

            for (MeshData* meshData : modelData->mMeshes)
            {
                mRenderDevice->SetTexture(meshData->mTextureBuffer, 0); // temp
                mRenderDevice->RenderPrimitive(meshData->mVertexBuffer, meshData->mIndexBuffer, 0, meshData->mIndices.size());
            }
        }

        mRenderDevice->BlitRenderTargetToWindow(mRenderTarget.get(), mRenderWindow);
        mRenderDevice->EndRenderWindow(mRenderWindow);
    
    }
}
