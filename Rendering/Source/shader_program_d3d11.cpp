#include "shader_program_d3d11.h"

namespace Ming3D
{
    ShaderD3D11::~ShaderD3D11()
    {
        if (mConstantData != nullptr)
        {
            delete mConstantData;
        }
    }

    ShaderProgramD3D11::~ShaderProgramD3D11()
    {
        if (mVS != nullptr)
        {
            mVS->Release();
            delete mVS;
        }
        if (mPS != nullptr)
        {
            mPS->Release();
            delete mPS;
        }

        if (mInputLayout != nullptr)
        {
            mInputLayout->Release();
            delete mInputLayout;
        }

        for (ShaderD3D11* shader : mShaders)
        {
            delete shader;
        }

        mShaders.clear();
    }
}
