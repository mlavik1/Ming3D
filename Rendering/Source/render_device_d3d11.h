#ifndef MING3D_RENDER_DEVICE_D3D11_H
#define MING3D_RENDER_DEVICE_D3D11_H

#include "render_device.h"
#include "render_target_d3d11.h"
#include "shader_program_d3d11.h"

#include <Windows.h>
#include <windowsx.h>
#include <d3d11.h>
#include <d3dx11.h>
#include <d3dx10.h>


namespace Ming3D
{
    class RenderDeviceD3D11 : public RenderDevice
    {
    private:
        ID3D11Device* mDevice;
        ID3D11DeviceContext* mDeviceContext;
        IDXGIFactory* mDXGIFactory;
        RenderTargetD3D11* mRenderTarget = nullptr;
        ShaderProgramD3D11* mActiveShaderProgram = nullptr;

        ID3D11SamplerState* mDefaultSamplerState;
        ID3D11RasterizerState* mDefaultRasterState;
        ID3D11DepthStencilState* mDefaultDepthStencilState;

        size_t GetShaderUniformSize(const ShaderUniformInfo& inShaderUniform);

    public:
        RenderDeviceD3D11();
        ~RenderDeviceD3D11();

        virtual RenderTarget* CreateRenderTarget(WindowBase* inWindow) override;
        virtual VertexBuffer* CreateVertexBuffer(VertexData* inVertexData) override;
        virtual IndexBuffer* CreateIndexBuffer(IndexData* inIndexData) override;
        virtual ShaderProgram* CreateShaderProgram(const std::string& inShaderProgramPath) override;
        virtual Texture* CreateTexture() override;
        virtual void SetTexture(Texture* inTexture) override;
        virtual void SetRenderTarget(RenderTarget* inTarget) override;
        virtual void SetActiveShaderProgram(ShaderProgram* inProgram) override;
        virtual void BeginRendering() override;
        virtual void EndRendering() override;
        virtual void RenderPrimitive(VertexBuffer* inVertexBuffer, IndexBuffer* inIndexBuffer) override;

        virtual void SetShaderUniformMat4x4(const char* inName, const glm::mat4 inMat) override;
        virtual void SetShaderUniformVec4(const char* inName, const glm::vec4 inVec) override;

        ID3D11Device* GetDevice() { return mDevice; }
        ID3D11DeviceContext* GetDeviceContext() { return mDeviceContext; }
        IDXGIFactory* GetDXGIFactory() { return mDXGIFactory; }
    };
}

namespace Ming3D
{
    extern RenderDeviceD3D11* GRenderDeviceD3D11;
}

#endif
