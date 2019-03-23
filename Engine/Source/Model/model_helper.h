// THIS IS A TEMPORARY FILE
// DELETE LATER

#ifndef MING3D_MODELLOADER_H
#define MING3D_MODELLOADER_H

#include <vector>
#include "glm/glm.hpp"
#include "Actors/actor.h"
#include "vertex.h"
#include "graphics_data.h"

namespace Ming3D
{
    class Texture;

    class MeshData
    {
    public:
        VertexData* mVertexData = nullptr;
        std::vector<unsigned int> mIndices;

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
