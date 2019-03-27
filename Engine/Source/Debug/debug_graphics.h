#ifndef MING3D_DEBUGGRAPHICS_H
#define MING3D_DEBUGGRAPHICS_H

#include "glm/glm.hpp"

namespace Ming3D
{
    class DebugGraphics
    {
    public:
        static void DrawBox(const glm::vec3& boxPos, const glm::vec3& boxSize, const glm::vec4& boxColour);
    };
}

#endif
