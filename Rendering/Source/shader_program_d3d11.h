#ifndef MING3D_SHADER_PROGRAM_D3D11_H
#define MING3D_SHADER_PROGRAM_D3D11_H

#include "shader_program.h"

#include <d3d11.h>
#include <unordered_map>
#include <vector>
#include "shader_constant_d3d11.h"

namespace Ming3D
{
    struct ShaderConstantRef
    {
    public:
        size_t mCBufferIndex;
        size_t mConstantIndex;
    };

    class ConstantBufferD3D11 // TODO: move to separate file
    {
    public:
        ~ConstantBufferD3D11();

        ID3D11Buffer* mConstantBuffer;
        void* mConstantData = nullptr;
        std::vector<ShaderConstantD3D11> mShaderConstants;
        size_t mShaderConstantsSize;
    };

    class ShaderProgramD3D11 : public ShaderProgram
    {
    public: // TODO: should be private
        ID3D11VertexShader* mVS;
        ID3D11PixelShader* mPS;
        ID3D11InputLayout* mInputLayout; // TODO: Do not create one per shader program
        std::vector<ConstantBufferD3D11*> mConstantBuffers;
        std::unordered_map<std::string, ShaderConstantRef> mConstantNameMap;

    public:
        virtual ~ShaderProgramD3D11();
    };
}

#endif
