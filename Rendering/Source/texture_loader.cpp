#include "texture_loader.h"

#include "SDL_image.h"

namespace Ming3D
{
    void TextureLoader::LoadTextureData(const char* inFilePath, Texture* outTexture)
    {
        SDL_Surface * surface = IMG_Load(inFilePath);

        void* textureDataPtr = surface->pixels;

        unsigned int bytesPerPixel = surface->format->BytesPerPixel;
        PixelFormat pixelFormat;
        if (surface->format->BitsPerPixel == 32 && surface->format->Bshift < surface->format->Gshift)
            pixelFormat = PixelFormat::RGBA;
        else if (surface->format->BitsPerPixel == 32)
            pixelFormat = PixelFormat::RGBA;
        else
            pixelFormat = PixelFormat::RGB;

        // correct pixel format - TODO: also handle 16 bit
        if (pixelFormat == PixelFormat::RGB && bytesPerPixel == 3)
        {
            pixelFormat = PixelFormat::RGBA;
            bytesPerPixel = 4;

            uint8_t* textureData = new uint8_t[surface->w * surface->h * 4];

            size_t sourceTextureIndex = 0;
            for (size_t i = 0; i < surface->w * surface->h * 4; i++)
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
            textureDataPtr = textureData;
        }

        outTexture->SetTextureData(textureDataPtr, bytesPerPixel, pixelFormat, surface->w, surface->h);
        /*
#ifndef MING3D_USE_OPENGL // TODO - TEMP HACK: what to do about this? OpenGL and D3D use a different origin (Upper-Left vs BottomLeft). If we try to solve it by flipping the Y-coordinate in the shader, we also have to do that when rendering to texture, which is ugly..
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
        SDL_FreeSurface(surface);
    }
}
