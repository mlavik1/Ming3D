// THIS IS A TEMPORARY FILE
// DELETE LATER

#ifndef MING3D_MODELLOADER_H
#define MING3D_MODELLOADER_H

#include <vector>
#include "glm/glm.hpp"
#include "Actors/actor.h"
#include "graphics_data.h"

#define MODELLOADERFLAGS_UNLIT 1

// Forward declarations
class aiScene;
class aiMesh;
class aiMaterial;
class aiNode;

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
        static Material* CreateMaterial(aiMaterial* aiMat, const std::string modelPath, const int flags);
        static Mesh* CreateMesh(aiMesh* aiMesh);
        static Actor* CreateNode(aiNode* aiNode, const std::vector<Mesh*>& meshes, const std::vector<Material*>& materials, const aiScene* scene);
    };
}
#endif
