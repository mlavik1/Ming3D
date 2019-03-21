#ifndef MING3D_RENDER_DEVICE_D3D11_H
#define MING3D_RENDER_DEVICE_D3D11_H

#include "render_device.h"
#include "render_target_d3d11.h"
#include "shader_program_d3d11.h"
#include "render_window_d3d11.h"
#include "rasteriser_state_d3d11.h"
#include "depth_stencil_state_d3d11.h"

#include <Windows.h>
#include <windowsx.h>
#include <d3d11.h>


namespace Ming3D
{
    class RenderDeviceD3D11 : public RenderDevice
    {
    private:
        ID3D11Device* mDevice;
        ID3D11DeviceContext* mDeviceContext;
        IDXGIFactory* mDXGIFactory;
        RenderTargetD3D11* mRenderTarget = nullptr;
        RenderWindowD3D11* mRenderWindow = nullptr;
        ShaderProgramD3D11* mActiveShaderProgram = nullptr;

        ID3D11SamplerState* mDefaultSamplerState;
        RasteriserStateD3D11* mDefaultRasteriserState;
        DepthStencilStateD3D11* mDefaultDepthStencilState;

        size_t GetShaderUniformSize(const ShaderUniformInfo& inShaderUniform);

    public:
        RenderDeviceD3D11();
        ~RenderDeviceD3D11();

        virtual RenderTarget* CreateRenderTarget(RenderWindow* inWindow) override;
        virtual RenderTarget* CreateRenderTarget(TextureInfo inTextureInfo, int numTextures) override;
        virtual VertexBuffer* CreateVertexBuffer(VertexData* inVertexData) override;
        virtual IndexBuffer* CreateIndexBuffer(IndexData* inIndexData) override;
        virtual ShaderProgram* CreateShaderProgram(const ShaderConverter::ParsedShaderProgram* parsedProgram) override;
        virtual TextureBuffer* CreateTextureBuffer(TextureInfo inTextureInfo, void* inTextureData) override;
        virtual RenderWindow* CreateRenderWindow(WindowBase* inWindow) override;
        virtual RasteriserState* CreateRasteriserState(RasteriserStateCullMode inCullMode, bool inDepthClipEnabled) override;
        virtual DepthStencilState* CreateDepthStencilState(DepthStencilDepthFunc inDepthFunc, bool inDepthEnabled) override;

        virtual void SetTexture(TextureBuffer* inTexture, int inSlot) override;
        virtual void SetActiveShaderProgram(ShaderProgram* inProgram) override;
        virtual void BeginRenderWindow(RenderWindow* inWindow) override;
        virtual void EndRenderWindow(RenderWindow* inWindow) override;
        virtual void BeginRenderTarget(RenderTarget* inTarget) override;
        virtual void EndRenderTarget(RenderTarget* inTarget) override;
        virtual void RenderPrimitive(VertexBuffer* inVertexBuffer, IndexBuffer* inIndexBuffer) override;
        virtual void SetRasteriserState(RasteriserState* inState) override;
        virtual void SetDepthStencilState(DepthStencilState* inState) override;

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
