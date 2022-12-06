#include "scene_view_widget.h"
#include "GameEngine/game_engine.h"
#include "Actors/actor.h"
#include "Components/camera_component.h"
#include "Model/model_helper.h"
#include "World/world.h"
#include "SceneRenderer/camera.h"
#include "World/world.h"
#include "Window/render_window_handle.h"
#include "texture_info.h"
#include "render_target.h"

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
    }
}
