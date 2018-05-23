#ifndef MING3D_SHADER_PROGRAM_D3D11_H
#define MING3D_SHADER_PROGRAM_D3D11_H

#include "shader_program.h"

#include <d3d11.h>
#include <unordered_map>

namespace Ming3D
{
    struct ShaderConstantD3D11
    {
    public:
        size_t mOffset;
        size_t mSize;
        ShaderConstantD3D11(const size_t inOffset, const size_t inSize)
        {
            mOffset = inOffset;
            mSize = inSize;
        }
    };

    class ShaderD3D11
    {
    public:
        ID3D11Buffer* mConstantBuffer;
        void* mConstantData = nullptr;
        std::unordered_map<std::string, ShaderConstantD3D11> mShaderConstantMap;
        size_t mShaderConstantsSize;

        ~ShaderD3D11();
    };

    class ShaderProgramD3D11 : public ShaderProgram
    {
    public: // TODO: should be private
        ID3D11VertexShader* mVS;
        ID3D11PixelShader* mPS;
        ID3D11InputLayout* mInputLayout; // TODO: THIS SHOULD NOT BE STORED HERE
        std::vector<ShaderD3D11*> mShaders;

    public:
        ~ShaderProgramD3D11();
    };
}

#endif
