#include "shader_info.h"

namespace Ming3D::Rendering
{
    ShaderDatatypeInfo::ShaderDatatypeInfo()
    {
        mDatatype = EShaderDatatype::None;
    }

    ShaderDatatypeInfo::ShaderDatatypeInfo(EShaderDatatype inType, std::string inName)
    {
        mDatatype = inType;
        mName = inName;
    }

    ShaderDatatypeInfo::ShaderDatatypeInfo(EShaderDatatype inType, std::string inName, std::vector<ShaderStructMember> inChildren)
    {
        mDatatype = inType;
        mName = inName;
        mMemberVariables = inChildren;
    }

    size_t ShaderDatatypeInfo::GetDataSize() const
    {
        switch (mDatatype)
        {
        case EShaderDatatype::Bool: return sizeof(bool);
        case EShaderDatatype::Float: return sizeof(float);
        case EShaderDatatype::Int: return sizeof(int);
        case EShaderDatatype::Mat4x4: return sizeof(glm::mat4);
        case EShaderDatatype::Vec2: return sizeof(glm::vec2);
        case EShaderDatatype::Vec3: return sizeof(glm::vec3);
        case EShaderDatatype::Vec4: return sizeof(glm::vec4);
        case EShaderDatatype::Struct:
        {
            size_t size = 0;
            for (const ShaderStructMember& child : mMemberVariables)
                size += child.mDatatype.GetDataSize();
            return size;
        }
        default: assert(0); // Unhandled data size
        }
        return 0;
    }
}
