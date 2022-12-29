#pragma once
#include "glm/vec4.hpp"
#include "glm/vec3.hpp"
#include "glm/vec2.hpp"
#include <cstdint>

namespace Ming3D
{
    struct GUIVertexData
    {
        glm::vec3 mPosition;
        glm::vec4 mColour;
        glm::vec2 mTexCoord;
    };
}
