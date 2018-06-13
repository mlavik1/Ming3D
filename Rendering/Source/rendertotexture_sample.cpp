#include "rendertotexture_sample.h"
#include "Debug/debug.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

namespace Ming3D
{
    void RenderToTextureSample::init()
    {
        SampleBase::init();

        mRenderTarget = mRenderDevice->CreateRenderTarget(mRenderWindow);

        TextureInfo texInfo;
        texInfo.mWidth = mMainWindow->GetWidth();
        texInfo.mHeight = mMainWindow->GetHeight();
        mTextureRenderTarget = mRenderDevice->CreateRenderTarget(texInfo, 1);

        mModels.push_back(LoadModel("Resources//Mvr_PetCow_walk.dae"));
        mModels.push_back(LoadModel("Resources//test.dae"));

        MeshData* screenRectMesh = CreateRectangleMesh(2.0f, 2.0f);
        //meshData->mTexture = mModels[1]->mMeshes[0]->mTexture;
        screenRectMesh->mTextureBuffer = mTextureRenderTarget->GetColourTextureBuffer(0); // use render target texture
        ModelData* rectModel = new ModelData();
        rectModel->mMeshes.push_back(screenRectMesh);
        mModels.push_back(rectModel);

        mModels[0]->mPosition = glm::vec3(2.0f, 0.0f, 0.0f);
        mModels[1]->mPosition = glm::vec3(-2.0f, 0.0f, 0.0f);
        mModels[2]->mPosition = glm::vec3(0.0f, 0.0f, 0.0f);

        for (ModelData* modelData : mModels)
        {
            for (MeshData* meshData : modelData->mMeshes)
            {
                VertexData vertexData({ EVertexComponent::Position, EVertexComponent::Normal, EVertexComponent::TexCoord }, meshData->mVertices.size());
                IndexData indexData(meshData->mIndices.size());

                memcpy(vertexData.GetDataPtr(), meshData->mVertices.data(), meshData->mVertices.size() * sizeof(Vertex));
                memcpy(indexData.GetData(), meshData->mIndices.data(), meshData->mIndices.size() * sizeof(unsigned int));

                meshData->mVertexBuffer = mRenderDevice->CreateVertexBuffer(&vertexData);
                meshData->mIndexBuffer = mRenderDevice->CreateIndexBuffer(&indexData);

                if (meshData->mTexture != nullptr)
                    meshData->mTextureBuffer = mRenderDevice->CreateTextureBuffer(meshData->mTexture->GetTextureInfo(), meshData->mTexture->GetTextureData());
            }

            // TODO: Use different shaders, based on vertex layout?
            modelData->mShaderProgram = mRenderDevice->CreateShaderProgram("Resources//shader_PNT.shader");
        }

    }

    void RenderToTextureSample::tick()
    {
        SampleBase::tick();

        // TODO:
        float width = 800.0f;
        float height = 600.0f;

        mRenderDevice->BeginRenderWindow(mRenderWindow);
        mRenderDevice->BeginRenderTarget(mTextureRenderTarget);

        for (ModelData* modelData : mModels)
        {
            if (modelData == mModels[2])
            {
                mRenderDevice->EndRenderTarget(mTextureRenderTarget);
                mRenderDevice->BeginRenderTarget(mRenderTarget);
            }

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

            if (modelData == mModels[2])
            {
                Projection = glm::ortho<float>(-1.0f, 1.0f, -1.0f, 1.0f, 0.0f, 100.0f);
                mvp = Projection * glm::lookAt(glm::vec3(0, 0, 0), glm::vec3(0, 0, -1), glm::vec3(0, 1, 0)) * Model;
            }

            mRenderDevice->SetActiveShaderProgram(modelData->mShaderProgram);

            mRenderDevice->SetShaderUniformMat4x4("MVP", mvp);
            mRenderDevice->SetShaderUniformVec4("test", glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));

            for (MeshData* meshData : modelData->mMeshes)
            {
                mRenderDevice->SetTexture(meshData->mTextureBuffer, 0); // temp
                mRenderDevice->RenderPrimitive(meshData->mVertexBuffer, meshData->mIndexBuffer);
            }
        }

        mRenderDevice->EndRenderTarget(mRenderTarget);
        mRenderDevice->EndRenderWindow(mRenderWindow);

    }
}
