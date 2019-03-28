#ifndef MING3D_RENDER_DEVICE_H
#define MING3D_RENDER_DEVICE_H
#include "render_target.h"

#include "window_base.h"
#include "vertex_buffer.h"
#include "index_buffer.h"
#include "graphics_data.h"
#include "shader_program.h"
#include "graphics_data.h"
#include "texture.h"
#include "texture_buffer.h"
#include "render_window.h"
#include "rasteriser_state.h"
#include "depth_stencil_state.h"
#include "shader_info.h"

#include <string>

namespace Ming3D
{
    /**
    * RenderDevice base class.
    * This is the base class for all rendering APIs.
    * It is responsible for creating render targets, vertex buffers, index buffers, shader programs and textures.
    */
    class RenderDevice
    {
    public:
        virtual ~RenderDevice() {}

        virtual RenderTarget* CreateRenderTarget(RenderWindow* inWindow) = 0;
        virtual RenderTarget* CreateRenderTarget(TextureInfo inTextureInfo, int numTextures) = 0;
        virtual VertexBuffer* CreateVertexBuffer(VertexData* inVertexData) = 0;
        virtual IndexBuffer* CreateIndexBuffer(IndexData* inIndexData) = 0;
        virtual ShaderProgram* CreateShaderProgram(const ParsedShaderProgram* inShaderProgramPath) = 0;
        virtual TextureBuffer* CreateTextureBuffer(TextureInfo inTextureInfo, void* inTextureData) = 0;
        virtual RenderWindow* CreateRenderWindow(WindowBase* inWindow) = 0;
        virtual RasteriserState* CreateRasteriserState(RasteriserStateCullMode inCullMode, bool inDepthClipEnabled) = 0;
        virtual DepthStencilState* CreateDepthStencilState(DepthStencilDepthFunc inDepthFunc, bool inDepthEnabled) = 0;

        virtual void SetTexture(const TextureBuffer* inTexture, int inSlot) = 0;
        virtual void SetActiveShaderProgram(ShaderProgram* inProgram) = 0;
        virtual void BeginRenderWindow(RenderWindow* inWindow) = 0;
        virtual void EndRenderWindow(RenderWindow* inWindow) = 0;
        virtual void BeginRenderTarget(RenderTarget* inTarget) = 0;
        virtual void EndRenderTarget(RenderTarget* inTarget) = 0;
        virtual void RenderPrimitive(VertexBuffer* inVertexBuffer, IndexBuffer* inIndexBuffer) = 0;
        virtual void SetRasteriserState(RasteriserState* inState) = 0;
        virtual void SetDepthStencilState(DepthStencilState* inState) = 0;
        
        virtual void SetShaderUniformFloat(const std::string& inName, float inVal) = 0;
        virtual void SetShaderUniformInt(const std::string& inName, int inVal) = 0;
        virtual void SetShaderUniformMat4x4(const std::string& inName, const glm::mat4 inMat) = 0;
        virtual void SetShaderUniformVec2(const std::string& inName, const glm::vec2 inVec) = 0;
        virtual void SetShaderUniformVec3(const std::string& inName, const glm::vec3 inVec) = 0;
        virtual void SetShaderUniformVec4(const std::string& inName, const glm::vec4 inVec) = 0;
    };
}
#endif
