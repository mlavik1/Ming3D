#pragma once
#include "font_face.h"
#include <string>

namespace Ming3D
{
    class FontHelper
    {
    public:
        static FontFace* LoadFontFace(const std::string fontPath);
    };
}
