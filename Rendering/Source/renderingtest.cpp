#pragma once

#define MING3D_USE_OPENGL

#include "renderingtest.h"

#include "Debug/debug.h"

#ifdef MING3D_USE_OPENGL
#include <SDL.h>
#include "sdl_window.h"
#include "render_device_gl.h"
#else
#include <Windows.h>
#include "render_device_d3d11.h"
#include "winapi_window.h"
#endif

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "assimp/scene.h"
#include "assimp/postprocess.h"
#include "assimp/Importer.hpp"

#include "graphics_data.h"

namespace Ming3D
{
	void RenderingTest::LoadModel()
	{
		ModelData* modelData = new ModelData();
		mModelData = modelData;

		Assimp::Importer importer;
		const aiScene * scene = importer.ReadFile("Resources//test.dae", aiProcess_Triangulate | aiProcess_OptimizeMeshes | aiProcess_JoinIdenticalVertices | aiProcess_RemoveRedundantMaterials | aiProcess_GenSmoothNormals);

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
						vertex.mNormal = glm::vec3(vt.x, vt.y, vt.z);
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
				// TODO: load texture
			}
			else
			{
				// TODO: set colour
			}

		}

	}

	void RenderingTest::Test()
	{
#ifdef MING3D_USE_OPENGL
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

		WindowBase* mainWindow;
#ifdef MING3D_USE_OPENGL
		mainWindow = new SDLWindow();
#else
		mainWindow = new WinAPIWindow();
#endif
		mainWindow->Initialise();

#ifdef MING3D_USE_OPENGL
		LOG_INFO() << "Using OpenGL, version " << glGetString(GL_VERSION);
		mRenderDevice = new RenderDeviceGL();
#else
		mRenderDevice = new RenderDeviceD3D11();
#endif
		RenderTarget* renderTarget = mRenderDevice->CreateRenderTarget(mainWindow);

		LoadModel();

		for (MeshData* meshData : mModelData->mMeshes)
		{
			VertexData vertexData({ EVertexComponent::Position, EVertexComponent::Normal, EVertexComponent::TexCoord }, meshData->mVertices.size());
			IndexData indexData(meshData->mIndices.size());

			memcpy(vertexData.GetDataPtr(), meshData->mVertices.data(), meshData->mVertices.size() * sizeof(Vertex));
			memcpy(indexData.GetData(), meshData->mIndices.data(), meshData->mIndices.size() * sizeof(unsigned int));

			meshData->mVertexBuffer = mRenderDevice->CreateVertexBuffer(&vertexData);
			meshData->mIndexBuffer = mRenderDevice->CreateIndexBuffer(&indexData);
		}

#ifdef MING3D_USE_OPENGL
		std::ifstream vertexShaderFile("Resources//shader.vs");
		std::ifstream fragmentShaderFile("Resources//shader.fs");
#else
		std::ifstream vertexShaderFile("Resources//vs.hlsl");
		std::ifstream fragmentShaderFile("Resources//ps.hlsl");
#endif

		std::string vertexShaderStr((std::istreambuf_iterator<char>(vertexShaderFile)), std::istreambuf_iterator<char>());
		std::string fragmentShaderStr((std::istreambuf_iterator<char>(fragmentShaderFile)), std::istreambuf_iterator<char>());

		ShaderProgramConstructionInfo constructionInfo;
		constructionInfo.Uniforms.push_back(ShaderUniformInfo(ShaderVariableType::Mat4x4, "MVP"));
		constructionInfo.Uniforms.push_back(ShaderUniformInfo(ShaderVariableType::Vec4, "test"));
		ShaderProgram* shaderProgram = mRenderDevice->CreateShaderProgram(vertexShaderStr, fragmentShaderStr, constructionInfo);

		float width = 800.0f;
		float height = 600.0f;

		float t = 0.0f;
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

			t += 0.005f;

			glm::mat4 Projection = glm::perspective<float>(glm::radians(45.0f), width / height, 0.1f, 100.0f);


			// Camera matrix
			glm::mat4 View = glm::lookAt(
				glm::vec3(0, 2, 6), // pos
				glm::vec3(0, 0, 0), // lookat
				glm::vec3(0, 1, 0)  // up
			);

			glm::mat4 Model = glm::mat4(1.0f);
			Model = glm::rotate(Model, t, glm::vec3(1.0f, 0.5f, 0.5f));

			glm::mat4 mvp = Projection * View * Model;

			mRenderDevice->SetActiveShaderProgram(shaderProgram);

			mRenderDevice->SetShaderUniformMat4x4("MVP", mvp);
			mRenderDevice->SetShaderUniformVec4("test", glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));

			mRenderDevice->SetRenderTarget(renderTarget);
			mRenderDevice->BeginRendering();
			renderTarget->BeginRendering();

			for (MeshData* meshData : mModelData->mMeshes)
			{
				mRenderDevice->RenderPrimitive(meshData->mVertexBuffer, meshData->mIndexBuffer);
			}

			renderTarget->EndRendering();
			mRenderDevice->EndRendering();
		}
		
	}
}
