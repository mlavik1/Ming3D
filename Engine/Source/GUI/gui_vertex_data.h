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

    class VisualSubmeshNode
    {
    public:
        unsigned int mVertexStartIndex;
        unsigned int mTriangleStartIndex;
        unsigned int mNumVertices;
        unsigned int mNumIndices;
        VisualSubmeshNode* mNextNode = nullptr;
    };
}
