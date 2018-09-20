#include "model_helper.h"

#include "assimp/scene.h"
#include "assimp/postprocess.h"
#include "assimp/Importer.hpp"
#include "texture_loader.h"
#include "Debug/debug.h"
#include "Debug/st_assert.h"

namespace Ming3D
{
    ModelData* ModelLoader::LoadModel(const char* inModel)
    {
        ModelData* modelData = new ModelData();

        Assimp::Importer importer;
        const aiScene * scene = importer.ReadFile(inModel, aiProcess_Triangulate | aiProcess_OptimizeMeshes | aiProcess_JoinIdenticalVertices | aiProcess_RemoveRedundantMaterials | aiProcess_GenSmoothNormals);

        __Assert(scene != nullptr);

        for (unsigned int m = 0; m < scene->mNumMeshes; m++)
        {
            MeshData* meshData = new MeshData();

            modelData->mMeshes.push_back(meshData);

            // Get Vertices
            if (scene->mMeshes[m]->mNumVertices > 0)
            {
                meshData->mHasNormals = scene->mMeshes[m]->HasNormals();
                meshData->mHasTexCoords = scene->mMeshes[m]->HasTextureCoords(0);

                for (unsigned int i = 0; i < scene->mMeshes[m]->mNumVertices; ++i)
                {
                    Vertex vertex;

                    aiVector3D &v = scene->mMeshes[m]->mVertices[i];
                    vertex.mVertex = glm::vec3(v.x, v.y, v.z);

                    if (scene->mMeshes[m]->HasNormals())
                    {
                        aiVector3D &vn = scene->mMeshes[m]->mNormals[i];
                        vertex.mNormal = glm::vec3(vn.x, vn.y, vn.z);
                    }
                    if (scene->mMeshes[m]->HasTextureCoords(0))
                    {
                        aiVector3D vt = scene->mMeshes[m]->mTextureCoords[0][i];
                        vertex.mTexCoord = glm::vec2(vt.x, vt.y);
                    }

                    meshData->mVertices.push_back(vertex);
                }
            }

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
                meshData->mTexture = new Texture();
                TextureLoader::LoadTextureData(texturePath.c_str(), meshData->mTexture);
            }
            else
            {
                // TODO: set default (white?) texture
                LOG_WARNING() << "Material has no valid texture";
                meshData->mTexture = new Texture;
                TextureLoader::LoadTextureData("Resources//texture.jpg", meshData->mTexture);
            }

        }
        return modelData;
    }
}
