#pragma once

#include "Texture/texture.h"
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
    };

    class FontFace
    {
    public:
        Texture* mTexture;
        std::unordered_map<wchar_t, FontGlyph> mGlyphs;

        /* Gets the glyph info of a UTF16 character code. */
        bool GetGlyph(wchar_t charCode, FontGlyph& outGlyph);
    };
}
