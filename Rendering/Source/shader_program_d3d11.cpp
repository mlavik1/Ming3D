#ifdef MING3D_D3D11
#include "shader_program_d3d11.h"

namespace Ming3D::Rendering
{
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
        
        delete mUniformCBuffer;
    }
}
#endif

