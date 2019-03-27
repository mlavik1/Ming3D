#ifndef MING3D_SHADERUNIFORMDATA_H
#define MING3D_SHADERUNIFORMDATA_H

#include "shader_info.h"

namespace Ming3D
{
    class ShaderUniformData
    {
    private:
        char* mData;
        size_t mSize;

    public:
        ShaderVariableInfo mUniformInfo;

        ShaderUniformData(ShaderVariableInfo info, size_t size)
        {
            mUniformInfo = info;
            mData = new char[size];
            mSize = size;
        }

        ~ShaderUniformData()
        {
            delete[] mData;
        }

        void SetData(const void* inValue)
        {
            memcpy(mData, inValue, mSize);
        }

        void GetData(void* outValue) const
        {
            memcpy(outValue, mData, mSize);
        }
    };
}

#endif
