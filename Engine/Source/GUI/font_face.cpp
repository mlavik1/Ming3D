#include "font_face.h"

namespace Ming3D
{
    bool FontFace::GetGlyph(wchar_t charCode, FontGlyph& outGlyph)
    {
        auto it = mGlyphs.find(charCode);
        if (it != mGlyphs.end())
        {
            outGlyph = it->second;
            return true;
        }
        else
            return false;
    }
}
