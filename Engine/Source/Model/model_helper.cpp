#include "model_helper.h"
#include "GameEngine/game_engine.h"
#include "assimp/scene.h"
#include "assimp/postprocess.h"
#include "assimp/Importer.hpp"
#include "Texture/texture.h"
#include "Texture/texture_loader.h"
#include "Debug/debug.h"
#include "Debug/st_assert.h"
#include "Components/mesh_component.h"
#include "mesh.h"
#include "material_factory.h"
#include "shader_program.h"
#include <memory>

#define MODELLOADERFLAGS_UNLIT 1

namespace Ming3D
{
    std::unique_ptr<Material> ModelLoader::CreateMaterial(aiMaterial* aiMat, const std::string modelPath, const int flags)
    {
        std::shared_ptr<Texture> diffuseTexture = nullptr;
        glm::vec4 diffuseColour = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
        glm::vec4 specularColour = glm::vec4(0.3f, 0.3f, 0.3f, 1.0f);
        glm::vec4 ambientColour = glm::vec4(0.2f, 0.2f, 0.2f, 1.0f);
        float shininess = 32.0f;

        // Import diffuse texture
        aiString path;  // filename
        if (aiMat->GetTexture(aiTextureType::aiTextureType_DIFFUSE, 0, &path) == AI_SUCCESS)
        {
            std::string texturePath = modelPath;
            size_t iLastSlash = texturePath.find_last_of('/');
            if (iLastSlash != std::string::npos)
                texturePath = texturePath.substr(0, iLastSlash + 1) + std::string(path.C_Str());
            else
                texturePath = std::string(path.C_Str());
            diffuseTexture = std::shared_ptr<Texture>(TextureLoader::LoadTextureData(texturePath.c_str()));
        }

        // Read material properties
        aiColor4D diffCol;
        if (aiGetMaterialColor(aiMat, AI_MATKEY_COLOR_DIFFUSE, &diffCol) == AI_SUCCESS)
            diffuseColour = glm::vec4(diffCol.r, diffCol.g, diffCol.b, diffCol.a);
        aiColor4D specCol;
        if (aiGetMaterialColor(aiMat, AI_MATKEY_COLOR_DIFFUSE, &specCol) == AI_SUCCESS)
            specularColour = glm::vec4(specCol.r, specCol.g, specCol.b, specCol.a);
        aiColor4D ambCol;
        if (aiGetMaterialColor(aiMat, AI_MATKEY_COLOR_AMBIENT, &ambCol) == AI_SUCCESS)
            ambientColour = glm::vec4(ambCol.r, ambCol.g, ambCol.b, ambCol.a);
        float aiShininess;
        if (aiGetMaterialFloat(aiMat, AI_MATKEY_SHININESS, &aiShininess) == AI_SUCCESS)
            shininess = aiShininess;

        // Setup material parameters (for shader parsing)
        MaterialParams matParams;
        matParams.mShaderProgramPath = GGameEngine->GetResourceDirectory() + std::string("/Shaders/defaultshader.cgp");
        if (diffuseTexture == nullptr)
            matParams.mPreprocessorDefinitions.emplace("use_mat_colour", "");
        
        if(flags & MODELLOADERFLAGS_UNLIT)
            matParams.mPreprocessorDefinitions.emplace("unlit_mode", "");

        // Create material
        std::unique_ptr<Material> material = MaterialFactory::CreateMaterial(matParams); // TODO: Generate shader based on vertex layout
        
        if (diffuseTexture != nullptr)
            material->SetTexture("mainTexture", diffuseTexture);
        else
            material->SetColour(diffuseColour);
        material->SetShaderUniformVec4("_colourSpecular", specularColour);
        material->SetShaderUniformFloat("_shininess", shininess);
        
        if (flags & MODELLOADERFLAGS_FORCE_OPAQUE)
            material->SetRenderType(ERenderType::Opaque);
        else if ((flags & MODELLOADERFLAGS_FORCE_TRANSPARENT)
            || diffuseColour.a < 1.0f)
            material->SetRenderType(ERenderType::Transparent);
        else
            material->SetRenderType(ERenderType::Opaque);

        return material;
    }

    std::unique_ptr<Mesh> ModelLoader::CreateMesh(aiMesh* aiMesh)
    {
        Rendering::VertexLayout vertLayout;
        vertLayout.VertexComponents.push_back(Rendering::EVertexComponent::Position);
        if (aiMesh->HasNormals())
            vertLayout.VertexComponents.push_back(Rendering::EVertexComponent::Normal);
        if (aiMesh->HasTextureCoords(0))
            vertLayout.VertexComponents.push_back(Rendering::EVertexComponent::TexCoord);
        
        Rendering::VertexData* vertData = new Rendering::VertexData(vertLayout, aiMesh->mNumVertices);
        char* currVert = (char*)vertData->GetDataPtr();

        // Get Vertices
        for (unsigned int i = 0; i < aiMesh->mNumVertices; ++i)
        {
            const aiVector3D aiv = aiMesh->mVertices[i];
            glm::vec3 v(aiv.x, aiv.y, aiv.z);
            memcpy(currVert, &v, sizeof(v));
            currVert += sizeof(v);

            if (aiMesh->HasNormals())
            {
                const aiVector3D aivn = aiMesh->mNormals[i];
                glm::vec3 vn(aivn.x, aivn.y, aivn.z);
                memcpy(currVert, &vn, sizeof(vn));
                currVert += sizeof(vn);
            }
            if (aiMesh->HasTextureCoords(0))
            {
                const aiVector3D aivt = aiMesh->mTextureCoords[0][i];
                glm::vec2 vt(aivt.x, aivt.y);
                memcpy(currVert, &vt, sizeof(vt));
                currVert += sizeof(vt);
            }
        }

        std::vector<unsigned int> indices;
        for (unsigned int f = 0; f < aiMesh->mNumFaces; f++)
        {
            const aiFace& face = aiMesh->mFaces[f];

            for (int i = 0; i < 3; i++)
            {
                indices.push_back(face.mIndices[i]);
            }
        }

        std::unique_ptr<Mesh> mesh = std::make_unique<Mesh>();
        mesh->mVertexData = vertData;
        mesh->mIndexData = new Rendering::IndexData(indices.size());
        if (indices.size() > 0)
            memcpy(mesh->mIndexData->GetData(), &indices[0], indices.size() * sizeof(indices[0]));

        return mesh;
    }

    std::weak_ptr<Actor> ModelLoader::CreateNode(aiNode* aiNode, const std::vector<std::shared_ptr<Mesh>>& meshes, const std::vector<std::shared_ptr<Material>>& materials, const aiScene* scene, Actor* parent)
    {
        std::shared_ptr<Actor> actor = parent->SpawnChildActor().lock();
        actor->SetActorName(aiNode->mName.C_Str());

        aiMatrix4x4 m = aiNode->mTransformation;
        glm::mat4 rootTransform(
            m[0][0], m[0][1], m[0][2], m[0][3],
            m[1][0], m[1][1], m[1][2], m[1][3],
            m[2][0], m[2][1], m[2][2], m[2][3],
            m[3][0], m[3][1], m[3][2], m[3][3]);
        actor->GetTransform().SetLocalTransformMatrix(rootTransform);

        for(unsigned int iMesh = 0; iMesh < aiNode->mNumMeshes; iMesh++)
        {
            std::shared_ptr<Mesh> mesh = meshes[aiNode->mMeshes[iMesh]];
            std::shared_ptr<Material> material = materials[scene->mMeshes[iMesh]->mMaterialIndex];

            MeshComponent* meshComp = actor->AddComponent<MeshComponent>();
            meshComp->SetMesh(mesh);
            meshComp->SetMaterial(material);
        }

        for(unsigned int iChild = 0; iChild < aiNode->mNumChildren; iChild++)
        {
            CreateNode(aiNode->mChildren[iChild], meshes, materials, scene, actor.get());
        }

        return actor;
    }

    bool ModelLoader::LoadModel(std::string modelPath, Actor* inActor, int inFlags)
    {
        // Fix model path
        for (size_t i = 0; i < modelPath.size(); i++)
        {
            if (modelPath[i] == '\\')
                modelPath[i] = '/';
        }

        // Import scene
        Assimp::Importer importer;
        const aiScene * scene = importer.ReadFile(modelPath, aiProcess_Triangulate | aiProcess_OptimizeMeshes | aiProcess_OptimizeGraph | aiProcess_JoinIdenticalVertices | aiProcess_RemoveRedundantMaterials | aiProcess_GenSmoothNormals);
        __Assert(scene != nullptr);

        // Process materials
        std::vector<std::shared_ptr<Material>> materials;
        materials.reserve(scene->mNumMaterials);
        for (unsigned int m = 0; m < scene->mNumMaterials; m++)
        {
            std::shared_ptr<Material> mat = CreateMaterial(scene->mMaterials[m], modelPath, inFlags);
            materials.push_back(mat);
        }

        std::vector<std::shared_ptr<Mesh>> meshes;
        meshes.reserve(scene->mNumMeshes);
        for (unsigned int m = 0; m < scene->mNumMeshes; m++)
        {
            std::shared_ptr<Mesh> mesh = CreateMesh(scene->mMeshes[m]);
            meshes.push_back(mesh);
        }

        CreateNode(scene->mRootNode, meshes, materials, scene, inActor);

        return true;
    }
}
