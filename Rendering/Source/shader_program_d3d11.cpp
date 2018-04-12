#include "shader_program_d3d11.h"

namespace Ming3D
{
	ShaderProgramD3D11::~ShaderProgramD3D11()
	{
		mVS->Release();
		mPS->Release();

		if (mConstantData != nullptr)
		{
			delete mConstantData;
		}
	}
}
