#ifndef MING3D_SHADER_PROGRAM_D3D11_H
#define MING3D_SHADER_PROGRAM_D3D11_H

#include "shader_program.h"

#include <d3d11.h>
#include <unordered_map>
#include <vector>
#include "shader_constant_d3d11.h"
#include "constant_buffer_d3d11.h"

namespace Ming3D::Rendering
{
    class ShaderProgramD3D11 : public ShaderProgram
    {
    public: // TODO: should be private
        ID3D11VertexShader* mVS;
        ID3D11PixelShader* mPS;
        ID3D11InputLayout* mInputLayout; // TODO: Do not create one per shader program
        
        // Constant buffer for all uniforms (non-cbuffer uniforms)
        ConstantBufferD3D11* mUniformCBuffer;
        std::unordered_map<std::string, ShaderConstantD3D11> mUniforms;
        size_t mUniformsSize;
        std::unordered_map<std::string, int> mConstantBufferLocations;


        std::vector<ID3D11Buffer*> mBoundConstantBuffers;
    public:
        virtual ~ShaderProgramD3D11();
    };
}

#endif
