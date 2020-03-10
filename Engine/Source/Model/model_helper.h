// THIS IS A TEMPORARY FILE
// DELETE LATER

#ifndef MING3D_MODELLOADER_H
#define MING3D_MODELLOADER_H

#include <vector>
#include "glm/glm.hpp"
#include "Actors/actor.h"
#include "graphics_data.h"

namespace Ming3D
{
    class Texture;

    class MeshData
    {
    public:
        Rendering::VertexData* mVertexData = nullptr;
        std::vector<unsigned int> mIndices;

        // TODO: bones

        int mMaterialIndex;
    };

    class MaterialData
    {
    public:
        Texture * mTexture;
        glm::vec4 mDiffuseColour = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
        glm::vec4 mSpecularColour = glm::vec4(0.3f, 0.3f, 0.3f, 1.0f);
        glm::vec4 mAmbientColour = glm::vec4(0.2f, 0.2f, 0.2f, 1.0f);
        float mShininess = 32.0f;
    };

    class ModelData
    {
    public:
        std::vector<MeshData*> mMeshes;
        std::vector<MaterialData*> mMaterials;
        glm::vec3 mPosition;
    };

    class ModelDataImporter
    {
    public:
        static ModelData* ImportModelData(const char* inModel);
    };

#define MODELLOADERFLAGS_UNLIT 1

    class ModelLoader
    {
    public:
        static bool LoadModel(const char* inModel, Actor* inActor, int inFlags = 0);
    };
}
#endif
