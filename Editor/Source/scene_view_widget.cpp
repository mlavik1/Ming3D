#include "scene_view_widget.h"
#include "GameEngine/game_engine.h"
#include "Actors/actor.h"
#include "Components/camera_component.h"
#include "Model/model_helper.h"
#include "World/world.h"
#include "SceneRenderer/camera.h"
#include "Window/render_window_handle.h"
#include "texture_info.h"
#include "render_target.h"
#include "glm/gtx/euler_angles.hpp"
#include "Input/input_manager.h"
#include "glm/gtc/matrix_transform.hpp"

IMPLEMENT_CLASS(Ming3D::SceneViewWidget)

namespace Ming3D
{
    SceneViewWidget::SceneViewWidget()
    {
        mCamera = std::make_unique<Camera>();
    }

    SceneViewWidget::~SceneViewWidget()
    {
        if (!mGameWorld.expired())
        {
            mGameWorld.lock()->GetRenderScene()->RemoveCamera(mCamera.get());
        }
    }

    void SceneViewWidget::InitialiseClass()
    {
        
    }

    void SceneViewWidget::Start()
    {
        auto editorWorld = GetWorld();
        mGameWorld = editorWorld->GetGameEngine()->GetWorld();
        mCamera->mRenderOrder = 1;
        mCamera->mRenderTarget = editorWorld->GetGameEngine()->GetMainRenderWindow()->mRenderTarget; // TODO
        mGameWorld.lock()->GetRenderScene()->AddCamera(mCamera.get());
    }

    void SceneViewWidget::Tick(float inDeltaTime)
    {
        const TextureInfo rtInfo = mCamera->mRenderTarget->mTextureInfo;
        WidgetRect screenRect = this->getAbsoluteRect();
        screenRect.mPosition.x /= rtInfo.mWidth;
        screenRect.mPosition.y /= rtInfo.mHeight;
        screenRect.mSize.x /= rtInfo.mWidth;
        screenRect.mSize.y /= rtInfo.mHeight;
        mCamera->mViewport = { screenRect.mPosition.x, 1.0f - screenRect.mPosition.y - screenRect.mSize.y, screenRect.mSize.x, screenRect.mSize.y };
    
        const float deltaTime = GetWorld()->GetGameEngine()->GetDeltaTime();
        const InputManager* inputManager = GetWorld()->GetGameEngine()->GetInputManager();

        // Handle movement by key input
        if(mMousePressed)
        {
            glm::vec3 movementDir;
            movementDir.z -= inputManager->GetKey(KeyCode::Key_W) ? mMovementSpeed : 0.0f;
            movementDir.z += inputManager->GetKey(KeyCode::Key_S) ? mMovementSpeed : 0.0f;
            movementDir.x += inputManager->GetKey(KeyCode::Key_D) ? mMovementSpeed : 0.0f;
            movementDir.x -= inputManager->GetKey(KeyCode::Key_A) ? mMovementSpeed : 0.0f;
            movementDir.y += inputManager->GetKey(KeyCode::Key_E) ? mMovementSpeed : 0.0f;
            movementDir.y -= inputManager->GetKey(KeyCode::Key_Q) ? mMovementSpeed : 0.0f;
            movementDir *= deltaTime;

            mPosition += glm::vec3(mRotationMatrix * glm::vec4(movementDir, 0.0f));
        }

        mCamera->mCameraMatrix = glm::inverse(glm::translate(glm::mat4(1.0f), mPosition) * mRotationMatrix);
    }

    void SceneViewWidget::OnInputEvent(InputEvent event)
    {
        Widget::OnInputEvent(event);
        
        WidgetRect rect = this->getAbsoluteRect();

        const float deltaTime = GetWorld()->GetGameEngine()->GetDeltaTime();

        glm::vec2 relativeMouse = glm::vec2(
            static_cast<float>(event.mMousePosition.x - rect.mPosition.x) / rect.mSize.x,
            static_cast<float>(event.mMousePosition.y - rect.mPosition.y) / rect.mSize.y
        );

        if (event.mType == InputEventType::MouseButtonDown && event.mMouseButton.mButton == 1)
        {
            mPrevMousePos = relativeMouse;
            mMousePressed = true;
        }
        else if (event.mType == InputEventType::MouseButtonUp || event.mType == InputEventType::MouseLeave)
        {
            mMousePressed = false;
        }

        // Handle rotation by mouse movement
        if(mMousePressed && event.mType == InputEventType::MouseMove)
        {
            glm::vec2 mouseOffset = relativeMouse - mPrevMousePos;
            mPitch -= mouseOffset.y * mRotationSpeed * deltaTime;
            mYaw -= mouseOffset.x * mRotationSpeed * deltaTime;
            glm::quat quaternion;
            quaternion = glm::rotate(quaternion, mYaw, glm::vec3(0.0f, 1.0f, 0.0f));
            quaternion = glm::rotate(quaternion, mPitch, glm::vec3(1.0f, 0.0f, 0.0f));
            mRotationMatrix = glm::mat4_cast(quaternion);
            mPrevMousePos = relativeMouse;
        }
    }
}
