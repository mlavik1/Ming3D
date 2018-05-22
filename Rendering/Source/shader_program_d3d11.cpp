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
		mVS->Release();
		mPS->Release();

        for (ShaderD3D11* shader : mShaders)
        {
            delete shader;
        }

        mShaders.clear();
	}
}
