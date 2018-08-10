#include "scene_renderer.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "window_base.h"
#include "GameEngine/game_engine.h"
#include "render_device.h"

namespace Ming3D
{
    SceneRenderer::SceneRenderer()
    {
        mRenderScene = new RenderScene();
    }

    SceneRenderer::~SceneRenderer()
    {
        delete mRenderScene;
    }

    void SceneRenderer::AddSceneObject(RenderSceneObject* inObject)
    {
        mRenderScene->mSceneObjects.push_back(inObject);
    }

    void SceneRenderer::RenderObjects()
    {
        WindowBase* window = GGameEngine->GetMainWindow();
        RenderDevice* renderDevice = GGameEngine->GetRenderDevice();

        for (RenderSceneObject* obj : mRenderScene->mSceneObjects)
        {
            glm::mat4 Projection = glm::perspective<float>(glm::radians(45.0f), (float)window->GetWidth() / (float)window->GetHeight(), 0.1f, 100.0f);

            // Camera matrix
            glm::mat4 view = glm::lookAt(
                glm::vec3(0, 2, 6), // pos
                glm::vec3(0, 0, 0), // lookat
                glm::vec3(0, 1, 0)  // up
            );

            glm::mat4 model = obj->mModelMatrix;//glm::mat4(1.0f);
            //glm::vec3 pos = obj->mPosition;// glm::vec3(2.0f, 0.0f, 0.0f);
            //model = glm::translate(model, pos);

            glm::mat4 mvp = Projection * view * model;

            renderDevice->SetActiveShaderProgram(obj->mShaderProgram);

            renderDevice->SetShaderUniformMat4x4("MVP", mvp);
            renderDevice->SetShaderUniformVec4("test", glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));

            for (MeshBuffer* meshBuffer : obj->mMeshes)
            {
                renderDevice->SetTexture(meshBuffer->mTextureBuffer, 0); // temp
                renderDevice->RenderPrimitive(meshBuffer->mVertexBuffer, meshBuffer->mIndexBuffer);
            }
        }
    }
}
