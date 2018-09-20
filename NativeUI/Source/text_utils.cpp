#include "text_utils.h"
#include <windows.h>

namespace NativeUI
{
    std::wstring TextUtils::UTF8ToWChar(std::string arg_string)
    {
        int wcharsNum = MultiByteToWideChar(CP_UTF8, 0, arg_string.c_str(), -1, NULL, 0);
        wchar_t* wstr = new wchar_t[wcharsNum];
        MultiByteToWideChar(CP_UTF8, 0, arg_string.c_str(), -1, wstr, wcharsNum); // convert to wide char
        std::wstring returnString(wstr);
        delete[] wstr;
        return returnString;
    }
}