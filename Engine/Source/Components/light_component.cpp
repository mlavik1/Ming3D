#include "light_component.h"
#include "Actors/actor.h"
#include "World/world.h"
#include "SceneRenderer/render_scene.h"

namespace Ming3D
{
    LightComponent::LightComponent()
    {
        mLightSource = new LightSource();
    }

    LightComponent::~LightComponent()
    {
        GetWorld()->GetRenderScene()->RemoveLightSource(mLightSource);
        delete mLightSource;
    }

    void LightComponent::SetLightType(ELightType type)
    {
        mLightSource->mType = type;
    }

	void LightComponent::SetShadowType(EShadowType type)
	{
		mLightSource->mShadowType = type;
	}

    const LightSource* LightComponent::GetLightSource()
    {
        return mLightSource;
    }

    void LightComponent::InitialTick()
    {
        GetWorld()->GetRenderScene()->AddLightSource(mLightSource);
    }

    void LightComponent::Tick(float inDeltaTime)
    {
        switch (mLightSource->mType)
        {
            case ELightType::DirectionalLight:
            mLightSource->mLightMat = glm::mat4(mParent->GetTransform().GetWorldRotation());
            mLightSource->mPosition = mParent->GetTransform().GetWorldPosition();
            mLightSource->mDirection = mParent->GetTransform().GetForward();
            break;
        }
        //mLightSource->mLightMat = mParent->GetTransform().GetWorldTransformMatrix();
    }
}
