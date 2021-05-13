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

namespace Ming3D
{
    ModelData* ModelDataImporter::ImportModelData(std::string modelPath)
    {
        ModelData* modelData = new ModelData();

        for (size_t i = 0; i < modelPath.size(); i++)
        {
            if (modelPath[i] == '\\')
                modelPath[i] = '/';
        }

        Assimp::Importer importer;
        const aiScene * scene = importer.ReadFile(modelPath, aiProcess_Triangulate | aiProcess_OptimizeMeshes | aiProcess_OptimizeGraph | aiProcess_JoinIdenticalVertices | aiProcess_RemoveRedundantMaterials | aiProcess_GenSmoothNormals);

        __Assert(scene != nullptr);

        for (unsigned int m = 0; m < scene->mNumMaterials; m++)
        {
            MaterialData* matData = new MaterialData();
            modelData->mMaterials.push_back(matData);
            aiString path;  // filename
            if (scene->mMaterials[m]->GetTexture(aiTextureType::aiTextureType_DIFFUSE, 0, &path) == AI_SUCCESS)
            {
                std::string texturePath = modelPath;
                size_t iLastSlash = texturePath.find_last_of('/');
                if (iLastSlash != std::string::npos)
                    texturePath = texturePath.substr(0, iLastSlash + 1) + std::string(path.C_Str());
                else
                    texturePath = std::string(path.C_Str());
                matData->mTexture = TextureLoader::LoadTextureData(texturePath.c_str());
            }
            else
            {
                matData->mTexture = nullptr;
            }
            aiColor4D diffCol;
            if (aiGetMaterialColor(scene->mMaterials[m], AI_MATKEY_COLOR_DIFFUSE, &diffCol) == AI_SUCCESS)
                matData->mDiffuseColour = glm::vec4(diffCol.r, diffCol.g, diffCol.b, diffCol.a);

            aiColor4D specCol;
            if (aiGetMaterialColor(scene->mMaterials[m], AI_MATKEY_COLOR_DIFFUSE, &specCol) == AI_SUCCESS)
                matData->mSpecularColour = glm::vec4(specCol.r, specCol.g, specCol.b, specCol.a);

            aiColor4D ambCol;
            if (aiGetMaterialColor(scene->mMaterials[m], AI_MATKEY_COLOR_AMBIENT, &ambCol) == AI_SUCCESS)
                matData->mAmbientColour = glm::vec4(ambCol.r, ambCol.g, ambCol.b, ambCol.a);

            float shininess;
            if (aiGetMaterialFloat(scene->mMaterials[m], AI_MATKEY_SHININESS, &shininess) == AI_SUCCESS)
                matData->mShininess = shininess;
        }

        for (unsigned int m = 0; m < scene->mNumMeshes; m++)
        {
            MeshData* meshData = new MeshData();

            modelData->mMeshes.push_back(meshData);

            Rendering::VertexLayout vertLayout;
            vertLayout.VertexComponents.push_back(Rendering::EVertexComponent::Position);
            if (scene->mMeshes[m]->HasNormals())
                vertLayout.VertexComponents.push_back(Rendering::EVertexComponent::Normal);
            if (scene->mMeshes[m]->HasTextureCoords(0))
                vertLayout.VertexComponents.push_back(Rendering::EVertexComponent::TexCoord);
            
            Rendering::VertexData* vertData = new Rendering::VertexData(vertLayout, scene->mMeshes[m]->mNumVertices);
            size_t vertSize = vertData->GetVertexSize();
            char* currVert = (char*)vertData->GetDataPtr();

            // Get Vertices
            for (unsigned int i = 0; i < scene->mMeshes[m]->mNumVertices; ++i)
            {
                const aiVector3D aiv = scene->mMeshes[m]->mVertices[i];
                glm::vec3 v(aiv.x, aiv.y, aiv.z);
                memcpy(currVert, &v, sizeof(v));
                currVert += sizeof(v);

                if (scene->mMeshes[m]->HasNormals())
                {
                    const aiVector3D aivn = scene->mMeshes[m]->mNormals[i];
                    glm::vec3 vn(aivn.x, aivn.y, aivn.z);
                    memcpy(currVert, &vn, sizeof(vn));
                    currVert += sizeof(vn);
                }
                if (scene->mMeshes[m]->HasTextureCoords(0))
                {
                    const aiVector3D aivt = scene->mMeshes[m]->mTextureCoords[0][i];
                    glm::vec2 vt(aivt.x, aivt.y);
                    memcpy(currVert, &vt, sizeof(vt));
                    currVert += sizeof(vt);
                }
            }

            meshData->mVertexData = vertData;

            for (unsigned int f = 0; f < scene->mMeshes[m]->mNumFaces; f++)
            {
                const aiFace& face = scene->mMeshes[m]->mFaces[f];

                for (int i = 0; i < 3; i++)
                {
                    meshData->mIndices.push_back(face.mIndices[i]);
                }
            }

            int matIndex = scene->mMeshes[m]->mMaterialIndex;
            meshData->mMaterialIndex = matIndex;
        }
        return modelData;
    }

    bool ModelLoader::LoadModel(std::string modelPath, Actor* inActor, int inFlags)
    {
        ModelData* modelData = ModelDataImporter::ImportModelData(modelPath);

        if (modelData == nullptr)
            return false;

        std::vector<Material*> materials;
        materials.reserve(modelData->mMaterials.size());
        for (MaterialData* matData : modelData->mMaterials)
        {
            MaterialParams matParams;
            matParams.mShaderProgramPath = GGameEngine->GetResourceDirectory() + std::string("/Shaders/defaultshader.cgp");
            if (matData->mTexture == nullptr)
                matParams.mPreprocessorDefinitions.emplace("use_mat_colour", "");
            
            if(inFlags & MODELLOADERFLAGS_UNLIT)
                matParams.mPreprocessorDefinitions.emplace("unlit_mode", "");

            Material* material = MaterialFactory::CreateMaterial(matParams); // TODO: Generate shader based on vertex layout
            
            if (matData->mTexture != nullptr)
                material->SetTexture("mainTexture", matData->mTexture);
            else
                material->SetColour(matData->mDiffuseColour);
            material->SetShaderUniformVec4("_colourSpecular", matData->mSpecularColour);
            material->SetShaderUniformFloat("_shininess", matData->mShininess);
            
			if (matData->mTexture != nullptr || matData->mDiffuseColour.a < 1.0f)
				material->SetRenderType(ERenderType::Transparent);
			else
				material->SetRenderType(ERenderType::Opaque);

			materials.push_back(material);
        }

        for (MeshData* meshData : modelData->mMeshes)
        {
            Actor* childActor = new Actor();
            childActor->GetTransform().SetParent(&inActor->GetTransform());

            Mesh* mesh = new Mesh();
            mesh->mVertexData = meshData->mVertexData;
            mesh->mIndexData = new Rendering::IndexData(meshData->mIndices.size());
            if (meshData->mIndices.size() > 0)
                memcpy(mesh->mIndexData->GetData(), &meshData->mIndices[0], meshData->mIndices.size() * sizeof(meshData->mIndices[0]));

            MeshComponent* meshComp = childActor->AddComponent<MeshComponent>();
            meshComp->SetMesh(mesh);
            Material* material = meshData->mMaterialIndex >= 0 ? materials[meshData->mMaterialIndex] : nullptr;
            meshComp->SetMaterial(material);
        }

        return true;
    }
}
