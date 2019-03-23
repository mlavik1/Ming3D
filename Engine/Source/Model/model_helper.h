// THIS IS A TEMPORARY FILE
// DELETE LATER

#ifndef MING3D_MODELLOADER_H
#define MING3D_MODELLOADER_H

#include <vector>
#include "glm/glm.hpp"
#include "Actors/actor.h"
#include "vertex.h"

namespace Ming3D
{
    class Texture;

    class MeshData
    {
    public:
        std::vector<Vertex> mVertices;
        std::vector<unsigned int> mIndices;
        bool mHasNormals = false;
        bool mHasTexCoords = false;

        // TODO: bones

        Texture* mTexture;
    };

    class ModelData
    {
    public:
        std::vector<MeshData*> mMeshes;
        glm::vec3 mPosition;
    };

    class ModelDataImporter
    {
    public:
        static ModelData* ImportModelData(const char* inModel);
    };

    class ModelLoader
    {
    public:
        static bool LoadModel(const char* inModel, Actor* inActor);
    };
}
#endif
