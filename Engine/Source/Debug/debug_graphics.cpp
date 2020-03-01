#include "debug_graphics.h"

#include "GameEngine/game_engine.h"
#include "SceneRenderer/scene_renderer.h"
#include "SceneRenderer/render_scene_object.h"
#include "render_device.h"
#include "glm/gtx/transform.hpp"
#include "glm/matrix.hpp"
#include "glm/gtc/quaternion.hpp"
#include "Model/primitive_factory.h"
#include "Model/material_factory.h"
#include "Model/mesh.h"

namespace Ming3D
{
    // TODO: add rotation parameter
    void DebugGraphics::DrawBox(const glm::vec3& boxPos, const glm::vec3& boxSize, const glm::vec4& boxColour)
    {
        RenderDevice* renderDevice = GGameEngine->GetRenderDevice();
        MeshBuffer* meshBuffer = new MeshBuffer();

        Mesh* mesh = PrimitiveFactory::CreateBox(boxSize);

        meshBuffer->mVertexBuffer = renderDevice->CreateVertexBuffer(mesh->mVertexData);
        meshBuffer->mIndexBuffer = renderDevice->CreateIndexBuffer(mesh->mIndexData);

        RenderSceneObject* renderSceneObject = new RenderSceneObject();
        renderSceneObject->mModelMatrix = glm::translate(glm::mat4(1.0f), boxPos) * glm::mat4(1.0f) * glm::scale(glm::mat4(1.0f), boxSize);
        renderSceneObject->mMesh = meshBuffer;
        Material* mat = MaterialFactory::CreateMaterial("Resources/Shaders/debuggraphics.cgp");
        renderSceneObject->mMaterial = mat->mMaterialBuffer;

        GGameEngine->GetSceneRenderer()->AddSceneObject(renderSceneObject);

        mat->SetShaderUniformVec4("colour", boxColour);

        // TODO: implement lifetime (delete object after X seconds)
    }
}
