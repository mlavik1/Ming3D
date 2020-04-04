#include "light_component.h"
#include "Actors/actor.h"
#include "GameEngine/game_engine.h"

namespace Ming3D
{
    LightComponent::LightComponent()
    {
        mLightSource = new LightSource();
    }

    LightComponent::~LightComponent()
    {
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
        GGameEngine->AddLightSource(mLightSource);
    }

    void LightComponent::Tick(float inDeltaTime)
    {
        switch (mLightSource->mType)
        {
            case ELightType::DirectionalLight:
            mLightSource->mLightMat = glm::mat4(mParent->GetTransform().GetWorldRotation());
            break;
        }
        //mLightSource->mLightMat = mParent->GetTransform().GetWorldTransformMatrix();
    }
}
