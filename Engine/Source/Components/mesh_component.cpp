#include "mesh_component.h"

#include "Model/model_helper.h"
#include "SceneRenderer/render_scene.h"
#include "GameEngine/game_engine.h"
#include "render_device.h"
#include "SceneRenderer/scene_renderer.h"
#include "Actors/actor.h"

IMPLEMENT_CLASS(Ming3D::MeshComponent)

namespace Ming3D
{
    void MeshComponent::InitialiseClass()
    {

    }

    void MeshComponent::InitialiseComponent()
    {
        Super::InitialiseComponent();
    }

    void MeshComponent::SetMesh(const char* meshFile)
    {
        ModelLoader modelLoader;
        ModelData* modelData = modelLoader.LoadModel(meshFile);
        RenderSceneObject* renderSceneObject = new RenderSceneObject();
        for(MeshData* meshData : modelData->mMeshes)
        {
            RenderDevice* renderDevice = GGameEngine->GetRenderDevice();
            MeshBuffer* meshBuffer = new MeshBuffer();
            
            VertexData vertexData({ EVertexComponent::Position, EVertexComponent::Normal, EVertexComponent::TexCoord }, meshData->mVertices.size());
            IndexData indexData(meshData->mIndices.size());

            memcpy(vertexData.GetDataPtr(), meshData->mVertices.data(), meshData->mVertices.size() * sizeof(Vertex));
            memcpy(indexData.GetData(), meshData->mIndices.data(), meshData->mIndices.size() * sizeof(unsigned int));

            meshBuffer->mVertexBuffer = renderDevice->CreateVertexBuffer(&vertexData);
            meshBuffer->mIndexBuffer = renderDevice->CreateIndexBuffer(&indexData);

            if (meshData->mTexture != nullptr)
                meshBuffer->mTextureBuffer = renderDevice->CreateTextureBuffer(meshData->mTexture->GetTextureInfo(), meshData->mTexture->GetTextureData());

            renderSceneObject->mOwnerComponent = this;
            renderSceneObject->mModelMatrix = mParent->GetTransform().GetWorldTransformMatrix();
            renderSceneObject->mMeshes.push_back(meshBuffer);
            renderSceneObject->mShaderProgram = renderDevice->CreateShaderProgram("Resources//shader_PNT.shader");
        }

        GGameEngine->GetSceneRenderer()->AddSceneObject(renderSceneObject);
    }
}
