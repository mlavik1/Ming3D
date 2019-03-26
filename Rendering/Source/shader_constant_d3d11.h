#ifndef MING3D_SHADERCONSTANTD3D11_H
#define MING3D_SHADERCONSTANTD3D11_H

#include "shader_constant.h"

namespace Ming3D
{
    class ShaderConstantD3D11 : public ShaderConstant
    {
    public:
        size_t mOffset;
        size_t mSize;

        ShaderConstantD3D11(ShaderUniformInfo inInfo, const size_t inOffset, const size_t inSize)
            : ShaderConstant(inInfo)
        {
            mOffset = inOffset;
            mSize = inSize;
        }
    };
}

#endif
