// THIS IS A TEMPORARY FILE
// DELETE LATER

#ifndef MING3D_MODELLOADER_H
#define MING3D_MODELLOADER_H

#include <vector>
#include <memory>
#include "glm/glm.hpp"
#include "Actors/actor.h"
#include "graphics_data.h"
#include "Object/object_ptr.h"

#define MODELLOADERFLAGS_UNLIT 1
#define MODELLOADERFLAGS_FORCE_OPAQUE 2
#define MODELLOADERFLAGS_FORCE_TRANSPARENT 4

// Forward declarations
struct aiScene;
struct aiMesh;
struct aiMaterial;
struct aiNode;

namespace Ming3D
{
    class Mesh;
    class Material;
    class Actor;

    class ModelLoader
    {
    public:
        static bool LoadModel(std::string modelPath, Actor* inActor, int inFlags = 0);

    private:
        static std::unique_ptr<Material> CreateMaterial(aiMaterial* aiMat, const std::string modelPath, const int flags);
        static std::unique_ptr<Mesh> CreateMesh(aiMesh* aiMesh);
        static ActorPtr CreateNode(aiNode* aiNode, const std::vector<std::shared_ptr<Mesh>>& meshes, const std::vector<std::shared_ptr<Material>>& materials, const aiScene* scene, Actor* parent);
    };
}
#endif
