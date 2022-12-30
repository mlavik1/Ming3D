#include "font_manager.h"
#include "freetype2/freetype/freetype.h"
#include <iostream>
#include <cmath>
#include <vector>
#include <filesystem>
#include "Model/material_factory.h"
#include "GameEngine/game_engine.h"
#include "gui_resource_manager.h"

namespace Ming3D
{
    const int NearestPOT(const int x)
    {
        return std::pow(2, std::ceil(std::log(x) / std::log(2)));
    }

    FontFace* FontManager::GetFontFace(const std::string fontPath, int fontSize)
    {
        std::string fontName = std::filesystem::absolute(fontPath).filename().string();
        std::string fontHash = fontPath + std::to_string(fontSize);

        // Check if font already has been loaded
        auto it = mFontFaces.find(fontHash);
        if (it != mFontFaces.end())
            return it->second;

        // Load FreeType
        FT_Library ftlib;
        FT_Error error = FT_Init_FreeType(&ftlib);
        if (error != FT_Err_Ok)
        {
            std::cout << "FT_Init_FreeType failed with error code: " << error << std::endl;
            return nullptr;
        }

        // Load font face
        FT_Face face;
        error = FT_New_Face(ftlib, fontPath.c_str(), 0, &face);
        if (error)
        {
            std::cout << "Failed to open file " << fontPath << ", error code: " << error << std::endl;
            return nullptr;
        }

        // Set font size
        error = FT_Set_Pixel_Sizes(face, 0, fontSize);
        if (error)
        {
            std::cout << "BitmapFontGenerator > failed to set font size, error code: " << error << std::endl;
            return nullptr;
        }


        //FT_Select_Charmap(face, ft_encoding_unicode);


        std::vector<FT_UInt> chars;
        FT_UInt index;
        for (FT_ULong c = FT_Get_First_Char(face, &index); index != 0; c = FT_Get_Next_Char(face, c, &index))
            chars.push_back(index);
        
        int charsPerDim = (int)std::ceil(sqrtf((float)chars.size()));
        int fntBmpWidth = (fontSize + 2) * charsPerDim;
        int fntBmpHeight = fntBmpWidth;

        fntBmpWidth = NearestPOT(fntBmpWidth);
        fntBmpHeight = NearestPOT(fntBmpHeight);

        unsigned char* buffer = new unsigned char[fntBmpWidth * fntBmpHeight * 4];
        memset(buffer, 0, fntBmpWidth * fntBmpHeight * 4);

        FontFace* fontFace = new FontFace();

        // Draw all characters to combined bitmap
        for(size_t i = 0; i < chars.size(); ++i)
        {
            FT_UInt glyphIndex = FT_Get_Char_Index(face, chars[i]);

            FT_Load_Glyph(face, glyphIndex, FT_LOAD_DEFAULT);

            FT_Render_Glyph(face->glyph, FT_RENDER_MODE_NORMAL);

            // find the tile position where we have to draw the character
            int x = (i % charsPerDim) * (fontSize + 2) + 1; // +1 is padding
            int y = (i / charsPerDim) * (fontSize + 2) + 1; // +1 is padding

            // Copy character pixels to combined bitmap
            const FT_Bitmap& bitmap = face->glyph->bitmap;
            for (unsigned int xx = 0; xx < bitmap.width; ++xx)
            {
                for (unsigned int yy = 0; yy < bitmap.rows; ++yy)
                {
                    unsigned char r = bitmap.buffer[(yy * (bitmap.width) + xx)];
                    buffer[(y + yy) * fntBmpWidth * 4 + (x + xx) * 4 + 0] = r;
                    buffer[(y + yy) * fntBmpWidth * 4 + (x + xx) * 4 + 1] = r;
                    buffer[(y + yy) * fntBmpWidth * 4 + (x + xx) * 4 + 2] = r;
                    buffer[(y + yy) * fntBmpWidth * 4 + (x + xx) * 4 + 3] = r;
                }
            }

            const FT_Glyph_Metrics metrics = face->glyph->metrics;

            FontGlyph glyphInfo;
            glyphInfo.mTexCoord = glm::vec2(x / static_cast<float>(fntBmpWidth), y / static_cast<float>(fntBmpHeight));
            
            glyphInfo.mAdvance = metrics.horiAdvance / 64;
            glyphInfo.mBearingX = metrics.horiBearingX / 64;
            glyphInfo.mBearingY = metrics.horiBearingY / 64;
            glyphInfo.mWidth = metrics.width / 64;
            glyphInfo.mHeight = metrics.height / 64;
            
            glyphInfo.mTexSize = glm::vec2(bitmap.width / static_cast<float>(fntBmpWidth), bitmap.rows / static_cast<float>(fntBmpHeight));
            glyphInfo.mRelSize = glm::vec2(bitmap.width / static_cast<float>(fontSize), bitmap.rows / static_cast<float>(fontSize));
           
            fontFace->mMeanBearing += glyphInfo.mBearingY;
            fontFace->mMaxBearingOffset = std::max(fontFace->mMaxBearingOffset, glyphInfo.mHeight - glyphInfo.mBearingY);

            fontFace->mGlyphs.emplace(static_cast<wchar_t>(chars[i]), glyphInfo);
        }

        fontFace->mMeanBearing /= fontFace->mGlyphs.size();

        fontFace->mTexture = std::make_shared<Texture>();
        fontFace->mTexture->SetTextureData(buffer, 4, PixelFormat::RGBA, fntBmpWidth, fntBmpHeight);

        // Create material (TODO: Maybe not here?)
        fontFace->mMaterial = new Material(GGameEngine->GetGUIResourceManager()->GetDefaultGUIMaterial());
        fontFace->mMaterial->SetTexture(0, fontFace->mTexture);

        fontFace->mFontSize = fontSize;

        mFontFaces.emplace(fontHash, fontFace);

        return fontFace;
    }
}
