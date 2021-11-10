#include "gui_resource_manager.h"
#include "Model/material_factory.h"
#include "GameEngine/game_engine.h"
#include "Texture/texture_loader.h"
#include "Texture/texture_loader.h"

namespace Ming3D
{
    Material* GUIResourceManager::mDefaultGUIMaterial = nullptr;

    Material* GUIResourceManager::GetDefaultGUIMaterial()
    {
        if (mDefaultGUIMaterial == nullptr)
        {
            mDefaultGUIMaterial = MaterialFactory::CreateMaterial(GGameEngine->GetResourceDirectory() + std::string("/Shaders/gui.cgp"));
            std::shared_ptr<Texture> whiteTex = std::shared_ptr<Texture>(TextureLoader::LoadTextureData(GGameEngine->GetResourceDirectory() + std::string("/default-white.png")));
            mDefaultGUIMaterial->SetTexture(0, whiteTex);
        }
        return mDefaultGUIMaterial;
    }

    Material* GUIResourceManager::GetMaterialForTexture(std::shared_ptr<Texture> texture)
    {
        auto it = mMaterialsByTexture.find(texture->mGUID);
        if (it == mMaterialsByTexture.end())
        {
            Material* mat = MaterialFactory::CreateMaterial(GGameEngine->GetResourceDirectory() + std::string("/Shaders/gui.cgp"));
            mat->SetTexture(0, texture);
            mMaterialsByTexture[texture->mGUID] = mat;
            return mat;
        }
        else
            return it->second;
    }

    std::shared_ptr<Texture> GUIResourceManager::GetTexture(std::string texturePath)
    {
        auto it = mTextures.find(texturePath);
        if (it != mTextures.end())
            return it->second;
        else
        {
            std::shared_ptr<Texture> tex(TextureLoader::LoadTextureData(texturePath));
            mTextures[texturePath] = tex;
            return tex;
        }
    }
}
