#ifndef MING3D_SHADERCACHE_H
#define MING3D_SHADERCACHE_H

#include <unordered_map>
#include <string>
#include "shader_info.h"

namespace Ming3D::Rendering
{
    class ShaderCache
    {
    private:
        static std::unordered_map<std::string, ParsedShaderProgram*> mCachedProgramInfos;

        static std::string ParamsToString(const ShaderParserParams& inParams);

    public:
        static void CacheProgramInfo(const ShaderParserParams& inParams, ParsedShaderProgram* inProgram); // !!! TODO: Cache a copy of the program !!!

        static bool GetCachedProgramInfo(const ShaderParserParams& inParams, ParsedShaderProgram*& outProgram);
    };
}

#endif
