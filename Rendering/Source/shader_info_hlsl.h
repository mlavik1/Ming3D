#ifndef MING3D_SHADERINFOHLSL_H
#define MING3D_SHADERINFOHLSL_H

#include "shader_info.h"

namespace Ming3D
{
    class ConvertedShaderProgramHLSL : public ConvertedShaderProgram
    {
    public:
        ID3D10Blob * vsBlob = nullptr;
        ID3D10Blob* psBlob = nullptr;

        virtual ~ConvertedShaderProgramHLSL()
        {
            if (vsBlob != nullptr)
                vsBlob->Release();
            if (psBlob != nullptr)
                psBlob->Release();
        }
    };
}

#endif
