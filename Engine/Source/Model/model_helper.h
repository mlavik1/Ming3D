// THIS IS A TEMPORARY FILE
// DELETE LATER

#ifndef MING3D_MODELLOADER_H
#define MING3D_MODELLOADER_H

#include <vector>
#include "shader_program.h"
#include "glm/glm.hpp"
#include "texture.h"

namespace Ming3D
{
    class Vertex
    {
    public:
        glm::vec3 mVertex;
        glm::vec3 mNormal;
        glm::vec2 mTexCoord;
    };

    class MeshData
    {
    public:
        std::vector<Vertex> mVertices;
        std::vector<unsigned int> mIndices;
        bool mHasNormals = false;
        bool mHasTexCoords = false;

        Texture* mTexture;
    };

    class ModelData
    {
    public:
        std::vector<MeshData*> mMeshes;
        glm::vec3 mPosition;
    };

    class ModelLoader
    {
    public:
        ModelData* LoadModel(const char* inModel);
    };
}
#endif
