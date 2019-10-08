#include "shader_cache.h"
#include <sstream>

namespace Ming3D
{
    std::unordered_map<std::string, ParsedShaderProgram*> ShaderCache::mCachedProgramInfos;

    std::string ShaderCache::ParamsToString(const ShaderParserParams& inParams)
    {
        std::stringstream ss;
        ss << inParams.mShaderProgramPath;
        for (auto def : inParams.mPreprocessorDefinitions)
        {
            ss << "|" << def.first << "=" << def.second;
        }
        return ss.str();
    }

    void ShaderCache::CacheProgramInfo(const ShaderParserParams& inParams, ParsedShaderProgram* inProgram)
    {
        std::string strParams = ParamsToString(inParams);
        mCachedProgramInfos.emplace(strParams, inProgram); // TODO: copy???
    }

    bool ShaderCache::GetCachedProgramInfo(const ShaderParserParams& inParams, ParsedShaderProgram*& outProgram)
    {
        std::string strParams = ParamsToString(inParams);
        auto itPrg = mCachedProgramInfos.find(strParams);
        if (itPrg != mCachedProgramInfos.end())
        {
            outProgram = itPrg->second;
            return true;
        }
        else
            return false;
    }
}
