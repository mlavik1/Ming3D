#include "camera_component.h"
#include "GameEngine/game_engine.h"
#include "glm/gtx/rotate_vector.hpp"
#include "Actors/actor.h"
#include "Window/render_window_handle.h"
#include "World/world.h"
#include "SceneRenderer/render_scene.h"
#include "SceneRenderer/camera.h"

IMPLEMENT_CLASS(Ming3D::CameraComponent)

namespace Ming3D
{
    void CameraComponent::InitialiseClass()
    {

    }

    CameraComponent::CameraComponent()
    {
        mCamera = new Camera();
        mRenderWindow = GGameEngine->GetMainRenderWindow(); // TODO
    }

    CameraComponent::~CameraComponent()
    {
        GetWorld()->GetRenderScene()->RemoveCamera(mCamera);
        delete mCamera;
    }

    void CameraComponent::SetRenderTarget(Rendering::RenderTarget* target)
    {
        mRenderWindow = nullptr;
        mCamera->mRenderTarget = target;
    }

    void CameraComponent::SetRenderTarget(RenderWindowHandle* wndHandle)
    {
        mRenderWindow = wndHandle;
        mCamera->mRenderTarget = wndHandle->mRenderTarget;
    }

    void CameraComponent::SetRenderOrder(int renderOrder)
    {
        mCamera->mRenderOrder = renderOrder;
    }

    void CameraComponent::SetViewport(float x, float y, float width, float height)
    {
        mCamera->mViewport = { x, y, width, height };
    }

    void CameraComponent::InitialTick()
    {
        GetWorld()->GetRenderScene()->AddCamera(mCamera);
    }

    void CameraComponent::Tick(float inDeltaTime)
    {
        mCamera->mCameraMatrix = glm::inverse(mParent->GetTransform().GetWorldTransformMatrix());

        // Update Camera's render target if we're rendering to a window, in case the render target was re-created
        // TODO: Maybe we should instead use a RenderTargetHandle, instead of storing the RenderTarget directly in the Camera?
        if (mRenderWindow != nullptr)
            mCamera->mRenderTarget = mRenderWindow->mRenderTarget;
    }
}
