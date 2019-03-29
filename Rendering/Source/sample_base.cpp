#include "sample_base.h"

#include "glm/glm.hpp"
#include <vector>

#include "assimp/scene.h"
#include "assimp/postprocess.h"
#include "assimp/Importer.hpp"
#include "texture_loader.h"

#include "Debug/debug.h"

#ifdef MING3D_FORCE_OPENGL
#include <SDL.h>
#include "sdl_window.h"
#include "render_device_gl.h"
#else
#include <Windows.h>
#include "render_device_d3d11.h"
#include "winapi_window.h"
#endif

#ifdef _WIN32
#include <Windows.h>
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
#ifdef MING3D_FORCE_OPENGL
        if (SDL_Init(SDL_INIT_VIDEO) != 0)
            LOG_ERROR() << "Failed to initialise SDL";
        else
        {
            SDL_version linkedver; SDL_version compiledver;
            SDL_GetVersion(&linkedver);
            SDL_VERSION(&compiledver);
            LOG_INFO() << "SDL compiled version: " << (int)compiledver.major << "." << (int)compiledver.minor << ", pathch: " << (int)compiledver.patch;
            LOG_INFO() << "SDL linked version: " << (int)linkedver.major << "." << (int)linkedver.minor << ", pathch: " << (int)linkedver.patch;
        }
#endif

#ifdef MING3D_FORCE_OPENGL
        mMainWindow = new SDLWindow();
#else
        mMainWindow = new WinAPIWindow();
#endif
        mMainWindow->Initialise();

#ifdef MING3D_FORCE_OPENGL
        LOG_INFO() << "Using OpenGL, version " << glGetString(GL_VERSION);
        mRenderDevice = new RenderDeviceGL();
#else
        mRenderDevice = new RenderDeviceD3D11();
#endif

        mRenderWindow = mRenderDevice->CreateRenderWindow(mMainWindow);
    }

    void SampleBase::tick()
    {

    }
    
    SampleBase::ModelData* SampleBase::SampleBase::LoadModel(const char* inModel)
    {
        ModelData* modelData = new ModelData();

        Assimp::Importer importer;
        const aiScene * scene = importer.ReadFile(inModel, aiProcess_Triangulate | aiProcess_OptimizeMeshes | aiProcess_JoinIdenticalVertices | aiProcess_RemoveRedundantMaterials | aiProcess_GenSmoothNormals);

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
