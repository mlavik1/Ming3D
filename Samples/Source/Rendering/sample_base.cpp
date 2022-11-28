#include "sample_base.h"

#include "glm/glm.hpp"
#include <vector>

#include "assimp/scene.h"
#include "assimp/postprocess.h"
#include "assimp/Importer.hpp"
#include "Texture/texture_loader.h"
#include "GameEngine/game_engine.h"

#include "Debug/debug.h"

#ifdef _WIN32
#include <Windows.h>
#include "Platform/platform_win32.h"
#else
#include "Platform/platform_linux.h"
#endif

namespace Ming3D
{
    void SampleBase::RunSample()
    {
        init();

        while (true)
        {
#ifdef _WIN32
            MSG msg;
            if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
            {
                TranslateMessage(&msg);
                DispatchMessage(&msg);

                if (msg.message == WM_QUIT)
                    break;
            }
#endif
            tick();
        }
    }

    void SampleBase::init()
    {

#ifdef _WIN32
        mPlatform = new PlatformWin32();
#else
        mPlatform = new PlatformLinux();
#endif

        mMainWindow = mPlatform->CreateOSWindow();
        mRenderDevice = mPlatform->CreateRenderDevice();
        mRenderWindow = mRenderDevice->CreateRenderWindow(mMainWindow);
    }

    void SampleBase::tick()
    {

    }

    SampleBase::ModelData* SampleBase::SampleBase::LoadModel(const std::string modelPath)
    {
        ModelData* modelData = new ModelData();

        Assimp::Importer importer;
        const aiScene * scene = importer.ReadFile(modelPath, aiProcess_Triangulate | aiProcess_OptimizeMeshes | aiProcess_JoinIdenticalVertices | aiProcess_RemoveRedundantMaterials | aiProcess_GenSmoothNormals);

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
                std::string texturePath = GGameEngine->GetResourceDirectory() + std::string("/") + std::string(path.C_Str());
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

    SampleBase::MeshData* SampleBase::CreateRectangleMesh(float inWidth, float inHeight)
    {
        const float halfWidth = inWidth / 2.0f;
        const float halfHeight = inHeight / 2.0f;
        MeshData* meshData = new MeshData();
        meshData->mHasNormals = true;
        meshData->mHasTexCoords = true;
        meshData->mVertices =
        { Vertex{ glm::vec3(-halfWidth, -halfHeight, 0.0f), glm::vec3(), glm::vec2(0.0f, 0.0f) },
            Vertex{ glm::vec3(-halfWidth, halfHeight, 0.0f), glm::vec3(), glm::vec2(0.0f, 1.0f) },
            Vertex{ glm::vec3(halfWidth, halfHeight, 0.0f), glm::vec3(), glm::vec2(1.0f, 1.0f) },
            Vertex{ glm::vec3(halfWidth, -halfHeight, 0.0f), glm::vec3(), glm::vec2(1.0f, 0.0f) } };
        meshData->mIndices = { 0, 2, 1, 0, 3, 2 };
        return meshData;
    }

}
