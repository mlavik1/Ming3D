#ifndef MING3D_SHADERUNIFORMDATA_H
#define MING3D_SHADERUNIFORMDATA_H

#include "shader_info.h"
#include "Debug/st_assert.h"
#include <cstring>

namespace Ming3D
{
    class ShaderUniformData
    {
    private:
        char* mData;
        size_t mSize;

    public:
        Rendering::ShaderDatatypeInfo mTypeInfo;

        ShaderUniformData(Rendering::ShaderDatatypeInfo datatype, size_t size)
        {
            mTypeInfo = datatype;
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

		void CopyData(ShaderUniformData* other)
		{
			__Assert(mSize == other->mSize);
			memcpy(mData, other->mData, mSize);
		}
    };
}

#endif
