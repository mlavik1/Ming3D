#include "texture_loader.h"

#ifdef _WIN32
#include <SDL_image.h>
#else
#include <SDL2/SDL_image.h>
#endif

namespace Ming3D
{
    Texture* TextureLoader::LoadTextureData(const std::string inFilePath)
    {
        SDL_Surface * surface = IMG_Load(inFilePath.c_str());

        if (surface == nullptr)
            return nullptr;

        Texture* texture = new Texture();

        unsigned int bytesPerPixel = surface->format->BytesPerPixel;
        PixelFormat pixelFormat;
        if (surface->format->BitsPerPixel == 32 && surface->format->Bshift < surface->format->Gshift)
            pixelFormat = PixelFormat::RGBA;
        else if (surface->format->BitsPerPixel == 32)
            pixelFormat = PixelFormat::RGBA;
        else
            pixelFormat = PixelFormat::RGB;

        const int texWidth = surface->w;
        const int texHeight = surface->h;
        const size_t textureSize = 4 * texWidth * texHeight; // Force 32 bit
        uint8_t* textureData = new uint8_t[textureSize];

        // correct pixel format - TODO: also handle 16 bit
        if (pixelFormat == PixelFormat::RGB && bytesPerPixel == 3)
        {
            pixelFormat = PixelFormat::RGBA;
            bytesPerPixel = 4;

            size_t sourceTextureIndex = 0;
            for (size_t i = 0; i < static_cast<size_t>(texWidth * texHeight * 4); i++)
            {
                if (i % 4 == 3)
                {
                    textureData[i] = 255;
                }
                else
                {
                    textureData[i] = ((uint8_t*)surface->pixels)[sourceTextureIndex];
                    sourceTextureIndex++;
                }
            }
        }
        else
        {
            memcpy(&textureData[0], surface->pixels, textureSize);
        }

        texture->SetTextureData(textureData, bytesPerPixel, pixelFormat, surface->w, surface->h);
        /*
        #ifndef MING3D_OPENGL // TODO - TEMP HACK: what to do about this? OpenGL and D3D use a different origin (Upper-Left vs BottomLeft). If we try to solve it by flipping the Y-coordinate in the shader, we also have to do that when rendering to texture, which is ugly..
        char* buffer = new char[surface->w * surface->h * bytesPerPixel];
        memcpy(buffer, outTexture->mTextureData.data(), surface->w * surface->h * bytesPerPixel);
        for (size_t i = 0; i < surface->h; i++)
        {
        const size_t rowSize = surface->w * bytesPerPixel;
        memcpy(outTexture->mTextureData.data() + (rowSize * (surface->h - 1)) - (rowSize * i), buffer + rowSize * i, rowSize);
        }
        delete[] buffer;
        #endif
        */
        delete[] textureData;
        SDL_FreeSurface(surface);
        return texture;
    }

    void TextureLoader::CreateEmptyTexture(Texture* outTexture)
    {
        const int bytesPerPixel = 4;
        const int textureWidth = 64;
        const int textureHeight = 64;
        uint8_t textureData[textureWidth * textureHeight];
        for (size_t iPixel = 0; iPixel < textureWidth * textureHeight; iPixel++)
            textureData[iPixel] = 255;

        outTexture->SetTextureData(textureData, bytesPerPixel, Ming3D::PixelFormat::RGBA, textureWidth, textureHeight);
    }
}
