#ifndef MING3D_LIGHTSOURCE_H
#define MING3D_LIGHTSOURCE_H

#include "light_type.h"
#include "glm/glm.hpp"

namespace Ming3D
{
    class Camera;

    class LightSource
    {
    public:
        ELightType mType = ELightType::DirectionalLight;
        glm::mat4 mLightMat;
        Camera* mLightCamera = nullptr;
    };
}

#endif
