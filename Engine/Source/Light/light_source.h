#ifndef MING3D_LIGHTSOURCE_H
#define MING3D_LIGHTSOURCE_H

#include "light_type.h"
#include "glm/glm.hpp"

namespace Ming3D
{
    class LightSource
    {
    public:
        LightType mType = LightType::DirectionalLight;
        glm::mat4 mLightMat;
    };
}

#endif
