#ifndef MING3D_RENDER_DEVICE_GL_H
#define MING3D_RENDER_DEVICE_GL_H

#include "render_device.h"
#include <GL/glew.h>
#include "glm/glm.hpp"
#include "render_target_gl.h"
#include "shader_program_gl.h"
#include "render_window_gl.h"
#include "rasteriser_state_gl.h"
#include "depth_stencil_state_gl.h"

namespace Ming3D::Rendering
{
    class RenderDeviceGL : public RenderDevice
    {
    private:
        RenderTargetGL* mRenderTarget = nullptr;
        RenderWindowGL* mRenderWindow = nullptr;

        ShaderProgramGL* mActiveShaderProgram = nullptr;

        void BlitRenderTarget(RenderTargetGL* inSourceTarget, RenderWindow* inTargetWindow);
        void CheckGLErrors(const char* callerName);

        RasteriserStateGL* mDefaultRasteriserState;
        DepthStencilStateGL* mDefaultDepthStencilState;

    public:
        RenderDeviceGL();
        ~RenderDeviceGL();

        virtual RenderTarget* CreateRenderTarget(RenderWindow* inWindow) override;
        virtual RenderTarget* CreateRenderTarget(TextureInfo inTextureInfo, int numTextures) override;
        virtual VertexBuffer* CreateVertexBuffer(VertexData* inVertexData, EVertexBufferUsage usage) override;
        virtual void UpdateVertexBuffer(VertexBuffer* inVertexBuffer, VertexData* inVertexData) override;
        virtual IndexBuffer* CreateIndexBuffer(IndexData* inIndexData) override;
        virtual ShaderProgram* CreateShaderProgram(ParsedShaderProgram* parsedProgram) override;
        virtual TextureBuffer* CreateTextureBuffer(TextureInfo inTextureInfo, void* inTextureData) override;
        virtual RenderWindow* CreateRenderWindow(WindowBase* inWindow) override;
        virtual RasteriserState* CreateRasteriserState(RasteriserStateCullMode inCullMode, bool inDepthClipEnabled) override;
        virtual DepthStencilState* CreateDepthStencilState(DepthStencilStateDesc inDesc) override;
        virtual ConstantBuffer* CreateConstantBuffer(size_t inSize) override;

        virtual void SetTexture(const TextureBuffer* inTexture, int inSlot) override;
        virtual void SetActiveShaderProgram(ShaderProgram* inProgram) override;
        virtual void BeginRenderWindow(RenderWindow* inWindow) override;
        virtual void EndRenderWindow(RenderWindow* inWindow) override;
        virtual void BeginRenderTarget(RenderTarget* inTarget) override;
        virtual void EndRenderTarget(RenderTarget* inTarget) override;
        virtual void RenderPrimitive(VertexBuffer* inVertexBuffer, IndexBuffer* inIndexBuffer) override;
        virtual void SetRasteriserState(RasteriserState* inState) override;
        virtual void SetDepthStencilState(DepthStencilState* inState) override;
        virtual void SetConstantBufferData(ConstantBuffer* inConstantBuffer, void* inData, size_t inSize) override;
        virtual void BindConstantBuffer(ConstantBuffer* inConstantBuffer, const char* inName, ShaderProgram* inProgram) override;

        virtual void SetShaderUniformFloat(const std::string& inName, float inVal) override;
        virtual void SetShaderUniformInt(const std::string& inName, int inVal) override;
        virtual void SetShaderUniformMat4x4(const std::string& inName, const glm::mat4 inMat) override;
        virtual void SetShaderUniformVec2(const std::string& inName, const glm::vec2 inVec) override;
        virtual void SetShaderUniformVec3(const std::string& inName, const glm::vec3 inVec) override;
        virtual void SetShaderUniformVec4(const std::string& inName, const glm::vec4 inVec) override;

    };
}

#endif
