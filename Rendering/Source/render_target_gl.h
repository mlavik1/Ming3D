#ifndef MING3D_RENDER_TARGET_GL_H
#define MING3D_RENDER_TARGET_GL_H

#include "render_target.h"

#include "GL/glew.h"
#include <vector>
#include "texture_buffer_gl.h"

namespace Ming3D::Rendering
{
    class RenderTargetGL : public RenderTarget
    {
        friend class RenderDeviceGL; // TODO

    private:
        GLuint mFrameBufferID = 0;
        std::vector<TextureBufferGL*> mColourBuffers;
        std::vector<GLenum> mAttachments;
        TextureBufferGL* mDepthRenderBuffer;
        RenderWindow* mRenderWindow = nullptr;

    public:
        RenderTargetGL();
        ~RenderTargetGL();

        virtual void BeginRendering() override;
        virtual void EndRendering() override;

        virtual TextureBuffer* GetColourTextureBuffer(int inSlot) override;
        virtual TextureBuffer* GetDepthTextureBuffer() override;

        virtual RenderWindow* GetRenderWindow() override;
    };
}

#endif
