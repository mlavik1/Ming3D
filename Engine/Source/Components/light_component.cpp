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

    void LightComponent::SetLightType(LightType type)
    {
        mLightSource->mType = type;
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
        mLightSource->mLightMat = mParent->GetTransform().GetWorldTransformMatrix();
    }
}
