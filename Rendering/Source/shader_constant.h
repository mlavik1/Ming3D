#ifndef MING3D_SHADERCONSTANT_H
#define MING3D_SHADERCONSTANT_H

namespace Ming3D
{
    class ShaderUniformInfo
    {
    public:
        ShaderDatatypeInfo mType;
        std::string mName;

        ShaderUniformInfo(const ShaderDatatypeInfo& inType, const std::string& inName)
        {
            mType = inType;
            mName = inName;
        }
    };

    class ShaderConstant
    {
    public:
        ShaderUniformInfo mUniformInfo;

        ShaderConstant(ShaderUniformInfo inInfo)
            : mUniformInfo(inInfo)
        {
        }
    };
}

#endif
