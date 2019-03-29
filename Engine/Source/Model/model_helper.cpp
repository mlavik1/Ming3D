#include "model_helper.h"

#include "assimp/scene.h"
#include "assimp/postprocess.h"
#include "assimp/Importer.hpp"
#include "texture_loader.h"
#include "Debug/debug.h"
#include "Debug/st_assert.h"
#include "Components/mesh_component.h"
#include "mesh.h"
#include "material_factory.h"
#include "shader_program.h"
#include "texture.h"

namespace Ming3D
{
    ModelData* ModelDataImporter::ImportModelData(const char* inModel)
    {
        ModelData* modelData = new ModelData();

        Assimp::Importer importer;
        const aiScene * scene = importer.ReadFile(inModel, aiProcess_Triangulate | aiProcess_OptimizeMeshes | aiProcess_OptimizeGraph | aiProcess_JoinIdenticalVertices | aiProcess_RemoveRedundantMaterials | aiProcess_GenSmoothNormals);

        __Assert(scene != nullptr);

        for (unsigned int m = 0; m < scene->mNumMeshes; m++)
        {
            MeshData* meshData = new MeshData();

            modelData->mMeshes.push_back(meshData);

            VertexLayout vertLayout;
            vertLayout.VertexComponents.push_back(EVertexComponent::Position);
            if (scene->mMeshes[m]->HasNormals())
                vertLayout.VertexComponents.push_back(EVertexComponent::Normal);
            if (scene->mMeshes[m]->HasTextureCoords(0))
                vertLayout.VertexComponents.push_back(EVertexComponent::TexCoord);
            
            VertexData* vertData = new VertexData(vertLayout, scene->mMeshes[m]->mNumVertices);
            size_t vertSize = vertData->GetVertexSize();
            char* currVert = (char*)vertData->GetDataPtr();

            // Get Vertices
            for (unsigned int i = 0; i < scene->mMeshes[m]->mNumVertices; ++i)
            {
                const aiVector3D aiv = scene->mMeshes[m]->mVertices[i];
                glm::vec3 v(aiv.x, aiv.y, aiv.z);
                memcpy(currVert, &v, sizeof(v));
                currVert += sizeof(v);

                if (scene->mMeshes[m]->HasNormals())
                {
                    const aiVector3D aivn = scene->mMeshes[m]->mNormals[i];
                    glm::vec3 vn(aivn.x, aivn.y, aivn.z);
                    memcpy(currVert, &vn, sizeof(vn));
                    currVert += sizeof(vn);
                }
                if (scene->mMeshes[m]->HasTextureCoords(0))
                {
                    const aiVector3D aivt = scene->mMeshes[m]->mTextureCoords[0][i];
                    glm::vec2 vt(aivt.x, aivt.y);
                    memcpy(currVert, &vt, sizeof(vt));
                    currVert += sizeof(vt);
                }
            }

            meshData->mVertexData = vertData;

            for (unsigned int f = 0; f < scene->mMeshes[m]->mNumFaces; f++)
            {
                const aiFace& face = scene->mMeshes[m]->mFaces[f];

                for (int i = 0; i < 3; i++)
                {
                    meshData->mIndices.push_back(face.mIndices[i]);
                }
            }

            int matIndex = scene->mMeshes[m]->mMaterialIndex;
            aiString path;  // filename
            if (scene->mMaterials[matIndex]->GetTexture(aiTextureType::aiTextureType_DIFFUSE, 0, &path) == AI_SUCCESS)
            {
                std::string texturePath = std::string("Resources//") + std::string(path.C_Str());
                meshData->mTexture = TextureLoader::LoadTextureData(texturePath.c_str());
            }
            else
            {
                // TODO: set default (white?) texture
                LOG_WARNING() << "Material has no valid texture";
                meshData->mTexture = nullptr;
            }

        }
        return modelData;
    }

    bool ModelLoader::LoadModel(const char* inModel, Actor* inActor)
    {
        ModelData* modelData = ModelDataImporter::ImportModelData(inModel);

        if (modelData == nullptr)
            return false;

        for (MeshData* meshData : modelData->mMeshes)
        {
            Actor* childActor = new Actor();
            childActor->GetTransform().SetParent(&inActor->GetTransform());

            Mesh* mesh = new Mesh();
            mesh->mVertexData = meshData->mVertexData;
            mesh->mIndexData = new IndexData(meshData->mIndices.size());
            if (meshData->mIndices.size() > 0)
                memcpy(mesh->mIndexData->GetData(), &meshData->mIndices[0], meshData->mIndices.size() * sizeof(meshData->mIndices[0]));

            MeshComponent* meshComp = childActor->AddComponent<MeshComponent>();
            meshComp->SetMesh(mesh);
            Material* material = MaterialFactory::CreateMaterial("Resources//shader_PNT.shader"); // TODO: Generate shader based on vertex layout
            meshComp->SetMaterial(material);
            if(meshData->mTexture != nullptr)
                material->SetTexture(0, meshData->mTexture);
        }
    }
}
