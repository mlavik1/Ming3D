#pragma once

#include "Texture/texture.h"
#include "Model/material.h"
#include "glm/vec2.hpp"
#include <unordered_map>
#include <string>

namespace Ming3D
{
    class FontGlyph
    {
    public:
        glm::vec2 mTexCoord;
        glm::vec2 mTexSize;
        glm::vec2 mRelSize; // RENAME
        int mAdvance;
        int mBearingX;
        int mBearingY;
        int mWidth;
        int mHeight;
    };

    class FontFace
    {
    public:
        Material* mMaterial; // TODO: Maybe not store this here?
        std::shared_ptr<Texture> mTexture;
        std::unordered_map<wchar_t, FontGlyph> mGlyphs;
        int mFontSize;
        int mMaxBearingOffset; // Max difference between glyph height and bearing.
        int mMeanBearing; // Mean vertical bearing.

        /* Gets the glyph info of a UTF16 character code. */
        bool GetGlyph(wchar_t charCode, FontGlyph& outGlyph);
    };
}
