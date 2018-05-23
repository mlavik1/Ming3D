#ifndef MING3D_RENDER_DEVICE_GL_H
#define MING3D_RENDER_DEVICE_GL_H

#include "render_device.h"
#include <gl/glew.h>
#include "glm/glm.hpp"
#include "render_target_gl.h"
#include "shader_program_gl.h"

namespace Ming3D
{
    class RenderDeviceGL : public RenderDevice
    {
    private:
        RenderTargetGL* mRenderTarget = nullptr;

        ShaderProgramGL* mActiveShaderProgram = nullptr;

    public:
        RenderDeviceGL();
        ~RenderDeviceGL();

        virtual RenderTarget* CreateRenderTarget(WindowBase* inWindow) override;
        virtual VertexBuffer* CreateVertexBuffer(VertexData* inVertexData) override;
        virtual IndexBuffer* CreateIndexBuffer(IndexData* inIndexData) override;
        virtual ShaderProgram* CreateShaderProgram(const std::string& inShaderProgramPath) override;
        virtual Texture* CreateTexture() override;
        virtual void SetTexture(Texture* inTexture) override;
        virtual void SetRenderTarget(RenderTarget* inTarget);
        virtual void SetActiveShaderProgram(ShaderProgram* inProgram) override;
        virtual void BeginRendering() override;
        virtual void EndRendering() override;
        virtual void RenderPrimitive(VertexBuffer* inVertexBuffer, IndexBuffer* inIndexBuffer) override;

        virtual void SetShaderUniformMat4x4(const char* inName, const glm::mat4 inMat) override;
        virtual void SetShaderUniformVec4(const char* inName, const glm::vec4 inVec) override;
    };
}

#endif
