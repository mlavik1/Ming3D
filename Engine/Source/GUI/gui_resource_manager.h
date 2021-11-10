#pragma once
#include <memory>
#include <string>
#include <unordered_map>
#include <cstdint>

namespace Ming3D
{
    class Texture;
    class Material;

    class GUIResourceManager
    {
    private:
        static Material* mDefaultGUIMaterial;
        std::unordered_map<uint64_t, Material*> mMaterialsByTexture;
        std::unordered_map<std::string, std::shared_ptr<Texture>> mTextures;

    public:
        Material* GetDefaultGUIMaterial();
        Material* GetMaterialForTexture(std::shared_ptr<Texture> texture);
        std::shared_ptr<Texture> GetTexture(std::string texturePath);
    };
}
