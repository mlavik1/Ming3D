#pragma once
#include "font_face.h"
#include <string>
#include <unordered_map>

namespace Ming3D
{
    class FontManager
    {
    private:
        std::unordered_map<std::string, FontFace*> mFontFaces;

    public:
        FontFace* GetFontFace(const std::string fontPath, int fontSize);
    };
}
