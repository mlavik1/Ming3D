#ifndef NATIVEUI_TEXTUTILS_H
#define NATIVEUI_TEXTUTILS_H

#include <string>

namespace NativeUI
{
    class TextUtils
    {
    public:
        static std::wstring UTF8ToWChar(std::string arg_string);
    };
}

#endif
