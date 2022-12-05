#include "debug_graphics.h"

#include "GameEngine/game_engine.h"
#include "SceneRenderer/render_scene.h"
#include "SceneRenderer/mesh_render_object.h"
#include "render_device.h"
#include "glm/gtx/transform.hpp"
#include "glm/matrix.hpp"
#include "glm/gtc/quaternion.hpp"
#include "Model/primitive_factory.h"
#include "Model/material_factory.h"
#include "Model/mesh.h"
#include "World/world.h"

namespace Ming3D
{
    // TODO: add rotation parameter
    void DebugGraphics::DrawBox(const glm::vec3& boxPos, const glm::vec3& boxSize, const glm::vec4& boxColour)
    {
        Mesh* mesh = PrimitiveFactory::CreateBox(boxSize);
        Material* mat = MaterialFactory::CreateMaterial(GGameEngine->GetResourceDirectory() + std::string("/Shaders/debuggraphics.cgp"));

        MeshRenderObject* renderObject = new MeshRenderObject();
        renderObject->SetMesh(mesh);
        renderObject->SetMaterial(mat);
        renderObject->SetTransform(glm::translate(glm::mat4(1.0f), boxPos) * glm::mat4(1.0f) * glm::scale(glm::mat4(1.0f), boxSize));

        GGameEngine->GetWorld().lock()->GetRenderScene()->AddSceneObject(renderObject);

        mat->SetShaderUniformVec4("colour", boxColour);

        // TODO: implement lifetime (delete object after X seconds)
    }
}
