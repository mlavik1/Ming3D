#ifndef MING3D_SHADERINFO_H
#define MING3D_SHADERINFO_H

#include <string>
#include <vector>

namespace Ming3D
{
    enum class EShaderDatatype
    {
        Struct, Float, Int, Bool, Void, Vec2, Vec3, Vec4, Mat4x4, Texture2D, None
    };

    class ShaderStructMember; // fwd.decl.

    class ShaderDatatypeInfo
    {
    public:
        EShaderDatatype mDatatype;
        std::string mName = "";
        std::string mParentType = "";
        std::vector<ShaderStructMember> mMemberVariables;

        ShaderDatatypeInfo()
        {
            mDatatype = EShaderDatatype::None;
        }

        ShaderDatatypeInfo(EShaderDatatype inType, std::string inName)
        {
            mDatatype = inType;
            mName = inName;
        }

        ShaderDatatypeInfo(EShaderDatatype inType, std::string inName, std::vector<ShaderStructMember> inChildren)
        {
            mDatatype = inType;
            mName = inName;
            mMemberVariables = inChildren;
        }
    };

    class ShaderStructMember
    {
    public:
        ShaderDatatypeInfo mDatatype;
        std::string mName;
        std::string mSemantic;

        ShaderStructMember() {}

        ShaderStructMember(ShaderDatatypeInfo inType, std::string inName)
        {
            mDatatype = inType;
            mName = inName;
        }
    };

    class ShaderVariableInfo
    {
    public:
        std::string mName;
        ShaderDatatypeInfo mDatatypeInfo;
    };

    class ShaderFunctionInfo
    {
    public:
        std::string mFunctionName;
        ShaderDatatypeInfo mReturnValueType;
        std::vector<ShaderVariableInfo> mParameters;
    };

    class ShaderTextureInfo
    {
    public:
        std::string mTextureType; // TODO: Use enum for texture types
        std::string mTextureName;
    };
}

#endif
