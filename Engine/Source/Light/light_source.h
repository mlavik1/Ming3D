#ifndef MING3D_LIGHTSOURCE_H
#define MING3D_LIGHTSOURCE_H

#include "light_type.h"
#include "shadow_type.h"
#include "glm/glm.hpp"

namespace Ming3D
{
    class Camera;

    class LightSource
    {
    public:
        ELightType mType = ELightType::DirectionalLight;
		EShadowType mShadowType = EShadowType::None;
        glm::mat4 mLightMat;
        glm::vec3 mPosition;
        glm::vec3 mDirection;
        Camera* mLightCamera = nullptr;
    };
}

#endif
