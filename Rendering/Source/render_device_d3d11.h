#ifndef MING3D_RENDER_DEVICE_D3D11_H
#define MING3D_RENDER_DEVICE_D3D11_H

#include "render_device.h"
#include "render_target_d3d11.h"
#include "shader_program_d3d11.h"
#include "render_window_d3d11.h"
#include "rasteriser_state_d3d11.h"
#include "depth_stencil_state_d3d11.h"
#include "shader_info.h"

#include <Windows.h>
#include <windowsx.h>
#include <d3d11_1.h>

namespace Ming3D::Rendering
{
    class RenderDeviceD3D11 : public RenderDevice
    {
    private:
        ID3D11Device1* mDevice;
        ID3D11DeviceContext1* mDeviceContext;
        IDXGIFactory* mDXGIFactory;
        RenderTargetD3D11* mRenderTarget = nullptr;
        RenderWindowD3D11* mRenderWindow = nullptr;
        ShaderProgramD3D11* mActiveShaderProgram = nullptr;

        ID3D11SamplerState* mDefaultSamplerState;
        RasteriserStateD3D11* mDefaultRasteriserState;
        DepthStencilStateD3D11* mDefaultDepthStencilState;

        void ApplyConstantPacking(const ShaderUniformInfo& inShaderUniform, size_t& outSize);
        void SetUniformCBufferData(const std::string& inName, const void* inData, size_t inSize);

        DepthStencilViewD3D11* CreateDepthStencilView(int inWidth, int inHeight);

    public:
        RenderDeviceD3D11();
        virtual ~RenderDeviceD3D11();

        virtual std::unique_ptr<RenderTarget> CreateRenderTarget(RenderWindow* inWindow) override;
        virtual std::unique_ptr<RenderTarget> CreateRenderTarget(TextureInfo inTextureInfo, int numTextures) override;
        virtual VertexBuffer* CreateVertexBuffer(VertexData* inVertexData, EBufferUsage usage) override;
        virtual void UpdateVertexBuffer(VertexBuffer* inVertexBuffer, VertexData* inVertexData) override;
        virtual IndexBuffer* CreateIndexBuffer(IndexData* inIndexData, EBufferUsage usage) override;
        virtual void UpdateIndexBuffer(IndexBuffer* inIndexBuffer, IndexData* inIndexData) override;
        virtual ShaderProgram* CreateShaderProgram(ParsedShaderProgram* parsedProgram) override;
        virtual TextureBuffer* CreateTextureBuffer(TextureInfo inTextureInfo, void* inTextureData) override;
        virtual RenderWindow* CreateRenderWindow(WindowBase* inWindow) override;
        virtual RasteriserState* CreateRasteriserState(RasteriserStateCullMode inCullMode, bool inDepthClipEnabled) override;
        virtual DepthStencilState* CreateDepthStencilState(DepthStencilStateDesc inDesc) override;
        virtual BlendState* CreateBlendState(bool enableBlend, EBlendMode blendMode) override;
        virtual ConstantBuffer* CreateConstantBuffer(size_t inSize) override;

        virtual void SetTexture(const TextureBuffer* inTexture, int inSlot) override;
        virtual void SetActiveShaderProgram(ShaderProgram* inProgram) override;
        virtual void BeginRenderWindow(RenderWindow* inWindow) override;
        virtual void EndRenderWindow(RenderWindow* inWindow) override;
        virtual void SetRenderTarget(RenderTarget* inTarget) override;
        virtual void BlitRenderTargetToWindow(RenderTarget* target, RenderWindow* window) override;
        virtual void BeginViewport(unsigned int x, unsigned int y, unsigned int width, unsigned int height) override;
        virtual void RenderPrimitive(VertexBuffer* inVertexBuffer, IndexBuffer* inIndexBuffer, unsigned int startIndex, unsigned int indexCount) override;
        virtual void SetRasteriserState(RasteriserState* inState) override;
        virtual void SetDepthStencilState(DepthStencilState* inState) override;
        virtual void SetBlendState(BlendState* inState) override;
        virtual void SetConstantBufferData(ConstantBuffer* inConstantBuffer, void* inData, size_t inSize) override;
        virtual void BindConstantBuffer(ConstantBuffer* inConstantBuffer, const char* inName, ShaderProgram* inProgram) override;

        virtual void SetShaderUniformFloat(const std::string& inName, float inVal) override;
        virtual void SetShaderUniformInt(const std::string& inName, int inVal) override;
        virtual void SetShaderUniformMat4x4(const std::string& inName, const glm::mat4 inMat) override;
        virtual void SetShaderUniformVec2(const std::string& inName, const glm::vec2 inVec) override;
        virtual void SetShaderUniformVec3(const std::string& inName, const glm::vec3 inVec) override;
        virtual void SetShaderUniformVec4(const std::string& inName, const glm::vec4 inVec) override;

        ID3D11Device* GetDevice() { return mDevice; }
        ID3D11DeviceContext* GetDeviceContext() { return mDeviceContext; }
        IDXGIFactory* GetDXGIFactory() { return mDXGIFactory; }
    };
}

namespace Ming3D::Rendering
{
    extern RenderDeviceD3D11* GRenderDeviceD3D11;
}

#endif
