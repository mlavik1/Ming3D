#include "camera_component.h"
#include "GameEngine/game_engine.h"
#include "glm/gtx/rotate_vector.hpp"
#include "Actors/actor.h"

IMPLEMENT_CLASS(Ming3D::CameraComponent)

namespace Ming3D
{
    void CameraComponent::InitialiseClass()
    {

    }

    CameraComponent::CameraComponent()
    {
        mCamera = new Camera();
        mCamera->mRenderTarget = GGameEngine->GetMainRenderTarget();
    }

    CameraComponent::~CameraComponent()
    {
        GGameEngine->RemoveCamera(this);
        delete mCamera;
    }

    void CameraComponent::InitialTick()
    {
        GGameEngine->AddCamera(this);
    }

    void CameraComponent::Tick(float inDeltaTime)
    {
        // TODO
        glm::mat4 mat = mParent->GetTransform().GetWorldTransformMatrix();
        glm::vec3 pos = mParent->GetTransform().GetWorldPosition();
        glm::vec3 forward = mat * glm::vec4(0.0f, 0.0, -1.0f, 0.0f);
        glm::vec3 up = mat * glm::vec4(0.0f, 1.0, 0.0f, 0.0f);

        mCamera->mCameraMatrix = glm::lookAt(pos, pos + forward, up);
    }
}
