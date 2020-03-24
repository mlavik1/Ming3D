#ifndef MING3D_LIGHTCOMPONENT_H
#define MING3D_LIGHTCOMPONENT_H

#include "component.h"
#include "Light/light_source.h"

namespace Ming3D
{
    class LightComponent : public Component
    {
    private:
        LightSource* mLightSource;

    protected:
        virtual void InitialTick() override;
        virtual void Tick(float inDeltaTime) override;

    public:
        LightComponent();
        virtual ~LightComponent() override;

        void SetLightType(ELightType type);
        const LightSource* GetLightSource();
    };
}

#endif
