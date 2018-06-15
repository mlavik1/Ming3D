#include "render_device_gl.h"

#include "vertex_buffer_gl.h"
#include "index_buffer_gl.h"
#include "render_target_gl.h"
#include "shader_program_gl.h"
#include "texture_buffer_gl.h"

#include "Debug/debug.h"
#include "Debug/st_assert.h"
#include "shader_writer_glsl.h"

namespace Ming3D
{
    RenderDeviceGL::RenderDeviceGL()
    {
        if (glewInit())
        {
            LOG_ERROR() << "Failed to initialise GLEW";
        }

        mDefaultRasteriserState = (RasteriserStateGL*)CreateRasteriserState(RasteriserStateCullMode::Front, true);
        mDefaultDepthStencilState = (DepthStencilStateGL*)CreateDepthStencilState(DepthStencilDepthFunc::Less, true);

        SetRasteriserState(mDefaultRasteriserState);
        SetDepthStencilState(mDefaultDepthStencilState);
    }

    RenderDeviceGL::~RenderDeviceGL()
    {

    }

    RenderTarget* RenderDeviceGL::CreateRenderTarget(RenderWindow* inWindow)
    {
        TextureInfo textureInfo;
        textureInfo.mWidth = inWindow->GetWindow()->GetWidth();
        textureInfo.mHeight = inWindow->GetWindow()->GetHeight();

        RenderTargetGL* renderTarget = (RenderTargetGL*)CreateRenderTarget(textureInfo, 1);
        renderTarget->mWindowTarget = true;
        return renderTarget;
    }

    RenderTarget* RenderDeviceGL::CreateRenderTarget(TextureInfo inTextureInfo, int numTextures)
    {
        RenderTargetGL* renderTarget = new RenderTargetGL();

        GLuint FramebufferName = 0;
        glGenFramebuffers(1, &FramebufferName);
        glBindFramebuffer(GL_FRAMEBUFFER, FramebufferName);

        TextureBufferGL* colourBuffer = new TextureBufferGL();

        for (int i = 0; i < numTextures; i++)
        {
            GLuint renderTexture;
            glGenTextures(1, &renderTexture);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, renderTexture);

            glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA8, inTextureInfo.mWidth, inTextureInfo.mHeight);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, renderTexture, 0);

            colourBuffer->SetGLTexture(renderTexture);
            renderTarget->mColourBuffers.push_back(colourBuffer);
        }

        GLuint depthrenderbuffer;
        glGenRenderbuffers(1, &depthrenderbuffer);
        glBindRenderbuffer(GL_RENDERBUFFER, depthrenderbuffer);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, inTextureInfo.mWidth, inTextureInfo.mHeight);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthrenderbuffer);

        //glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, renderTexture, 0);

        renderTarget->mFrameBufferID = FramebufferName;
        TextureBufferGL* depthBuffer = new TextureBufferGL();
        depthBuffer->SetGLTexture(depthrenderbuffer);
        renderTarget->mAttachments.push_back(GL_COLOR_ATTACHMENT0);
        renderTarget->mDepthRenderBuffer = depthBuffer;

        return renderTarget;
    }

    VertexBuffer* RenderDeviceGL::CreateVertexBuffer(VertexData* inVertexData)
    {
        VertexBufferGL* vertexBuffer = new VertexBufferGL();
        GLuint vbo;
        glGenBuffers(1, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, inVertexData->GetNumVertices() * inVertexData->GetVertexSize(), inVertexData->GetDataPtr(), GL_STATIC_DRAW);
        vertexBuffer->SetGLBuffer(vbo);
        vertexBuffer->SetVertexLayout(inVertexData->GetVertexLayout());
        return vertexBuffer;
    }

    IndexBuffer* RenderDeviceGL::CreateIndexBuffer(IndexData* inIndexData)
    {
        IndexBufferGL* indexBuffer = new IndexBufferGL();
        GLuint ibo;
        glGenBuffers(1, &ibo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, inIndexData->GetNumIndices() * sizeof(unsigned int), inIndexData->GetData(), GL_STATIC_DRAW);
        indexBuffer->SetGLBuffer(ibo);
        indexBuffer->SetNumIndices(inIndexData->GetNumIndices());
        return indexBuffer;
    }

    ShaderProgram* RenderDeviceGL::CreateShaderProgram(const std::string& inShaderProgramPath)
    {
        ShaderConverter::ShaderParser shaderParser;
        ShaderConverter::ParsedShaderProgram* parsedProgram = shaderParser.ParseShaderProgram(inShaderProgramPath.c_str());
        ShaderConverter::ShaderWriterGLSL shaderWriter;
        ShaderConverter::ShaderProgramDataGLSL convertedShaderData;
        shaderWriter.WriteShader(parsedProgram, convertedShaderData);
        delete parsedProgram;

        GLuint program = glCreateProgram();
        GLuint vs = glCreateShader(GL_VERTEX_SHADER);
        GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);

        const char* str_v = convertedShaderData.mVertexShader.mSource.c_str();
        const char* str_f = convertedShaderData.mFragmentShader.mSource.c_str();

        glShaderSource(vs, 1, &str_v, 0);
        glShaderSource(fs, 1, &str_f, 0);

        glCompileShader(vs);
        glCompileShader(fs);
        glAttachShader(program, vs);
        glAttachShader(program, fs);
        glLinkProgram(program);

        int vstatus, fstatus, lstatus;
        glGetShaderiv(vs, GL_COMPILE_STATUS, &vstatus);
        printf("Vertex shader compile status: %s\n", (vstatus == GL_TRUE) ? "true" : "false");
        glGetShaderiv(fs, GL_COMPILE_STATUS, &fstatus);
        printf("Fragment shader compile status: %s\n", (fstatus == GL_TRUE) ? "true" : "false");
        glGetProgramiv(program, GL_LINK_STATUS, &lstatus);
        printf("Program link status: %s\n", (lstatus == GL_TRUE) ? "true" : "false");
            
        if (vstatus == GL_FALSE)
        {
            GLchar infoLog[1024];
            glGetShaderInfoLog(vs, sizeof(infoLog), NULL, infoLog);
            LOG_ERROR() << "Error compling vertex shader: " << std::string(infoLog);
        }
        else if (fstatus == GL_FALSE)
        {
            GLchar infoLog[1024];
            glGetShaderInfoLog(fs, sizeof(infoLog), NULL, infoLog);
            LOG_ERROR() << "Error compling fragment shader: " << std::string(infoLog);
        }
        else if (lstatus == GL_FALSE)
        {
            GLchar infoLog[1024];
            glGetProgramInfoLog(program, sizeof(infoLog), NULL, infoLog);
            LOG_ERROR() << "Error linking shader program: " << std::string(infoLog);
        }

        ShaderProgramGL* shaderProgram = new ShaderProgramGL();
        shaderProgram->SetGLProgram(program);
        shaderProgram->SetGLVertexShader(vs);
        shaderProgram->SetGLFragmentShader(fs);
        return shaderProgram;
    }

    TextureBuffer* RenderDeviceGL::CreateTextureBuffer(TextureInfo inTextureInfo, void* inTextureData)
    {
        TextureBufferGL* textureBuffer = new TextureBufferGL();

        __Assert(inTextureData); // TODO: Clear if null

        
        char* buffer = new char[inTextureInfo.mWidth * inTextureInfo.mHeight * inTextureInfo.mBytesPerPixel];
        // Flip the texture
        for (size_t i = 0; i < inTextureInfo.mHeight; i++)
        {
            const size_t rowSize = inTextureInfo.mWidth * inTextureInfo.mBytesPerPixel;
            memcpy(buffer + (rowSize * (inTextureInfo.mHeight - 1)) - (rowSize * i), (char*)inTextureData + rowSize * i, rowSize);
        }

        GLuint glTexture;
        glGenTextures(1, &glTexture);
        glBindTexture(GL_TEXTURE_2D, glTexture);

        GLint pixelFormat;
        if (inTextureInfo.mPixelFormat == PixelFormat::RGB)
            pixelFormat = GL_RGB;
        else if (inTextureInfo.mPixelFormat == PixelFormat::BGRA)
            pixelFormat = GL_BGRA;
        else
            pixelFormat = GL_RGBA;
        GLint internalFormat = (inTextureInfo.mPixelFormat == PixelFormat::RGB) ? GL_RGB : GL_RGBA;

        glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, inTextureInfo.mWidth, inTextureInfo.mHeight, 0, pixelFormat, GL_UNSIGNED_BYTE, buffer);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        textureBuffer->SetGLTexture(glTexture);

        delete[] buffer;

        return textureBuffer;
    }

    RenderWindow* RenderDeviceGL::CreateRenderWindow(WindowBase* inWindow)
    {
        RenderWindowGL* renderWindow = new RenderWindowGL(inWindow);

        return renderWindow;
    }

    RasteriserState* RenderDeviceGL::CreateRasteriserState(RasteriserStateCullMode inCullMode, bool inDepthClipEnabled)
    {
        RasteriserStateGL* rasteriserState = new RasteriserStateGL();
        rasteriserState->mCullMode = inCullMode;
        rasteriserState->mDepthClipEnabled = inDepthClipEnabled;
        return rasteriserState;
    }

    DepthStencilState* RenderDeviceGL::CreateDepthStencilState(DepthStencilDepthFunc inDepthFunc, bool inDepthEnabled)
    {
        DepthStencilStateGL* depthStencilState = new DepthStencilStateGL();
        return depthStencilState;
    }

    void RenderDeviceGL::SetTexture(TextureBuffer* inTexture, int inSlot)
    {
        glEnable(GL_TEXTURE_2D); // TODO
        TextureBufferGL* glTexture = (TextureBufferGL*)inTexture;
        glActiveTexture(GL_TEXTURE0 + inSlot);
        glBindTexture(GL_TEXTURE_2D, glTexture->GetGLTexture());
    }

    void RenderDeviceGL::SetActiveShaderProgram(ShaderProgram* inProgram)
    {
        mActiveShaderProgram = (ShaderProgramGL*)inProgram;
        if (mActiveShaderProgram != nullptr)
        {
            glUseProgram(mActiveShaderProgram->GetGLProgram());
        }
    }

    void RenderDeviceGL::BeginRenderWindow(RenderWindow* inWindow)
    {
        mRenderWindow = (RenderWindowGL*)inWindow;

        mRenderWindow->GetWindow()->BeginRender();
    }

    void RenderDeviceGL::EndRenderWindow(RenderWindow* inWindow)
    {
        __Assert(mRenderWindow == inWindow);

        mRenderWindow->GetWindow()->EndRender();
        mRenderWindow = nullptr;
    }

    void RenderDeviceGL::BeginRenderTarget(RenderTarget* inTarget)
    {
        mRenderTarget = (RenderTargetGL*)inTarget;

        glBindFramebuffer(GL_FRAMEBUFFER, mRenderTarget->mFrameBufferID);
        glDrawBuffers(1, mRenderTarget->mAttachments.data());

        glClearColor(0.0f, 0.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        //glEnable(GL_DEPTH_TEST);
        //glDepthFunc(GL_LEQUAL);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);
        glFrontFace(GL_CCW);
    }

    void RenderDeviceGL::EndRenderTarget(RenderTarget* inTarget)
    {
        __Assert(mRenderTarget == inTarget);

        RenderTargetGL* glTarget = (RenderTargetGL*)inTarget;

        // If rendering to window render target, blit framebuffers (from render target FBO to Window's default FBO)
        if (glTarget->mWindowTarget)
        {
            BlitRenderTarget(glTarget, mRenderWindow);
        }

        mRenderTarget = nullptr;
    }

    void RenderDeviceGL::BlitRenderTarget(RenderTargetGL* inSourceTarget, RenderWindow* inTargetWindow)
    {
        const int w = inTargetWindow->GetWindow()->GetWidth();
        const int h = inTargetWindow->GetWindow()->GetHeight();
        glBindFramebuffer(GL_READ_FRAMEBUFFER, inSourceTarget->mFrameBufferID);
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
        glReadBuffer(GL_COLOR_ATTACHMENT0);
        glDrawBuffer(GL_COLOR_ATTACHMENT0);
        glViewport(0, 0, w, h);
        glBlitFramebuffer(0, 0, w, h, 0, 0, w, h, GL_COLOR_BUFFER_BIT, GL_NEAREST);
    }

    void RenderDeviceGL::RenderPrimitive(VertexBuffer* inVertexBuffer, IndexBuffer* inIndexBuffer)
    {
        VertexBufferGL* vertexBufferGL = (VertexBufferGL*)inVertexBuffer;
        IndexBufferGL* indexBufferGL = (IndexBufferGL*)inIndexBuffer;
        
        size_t vertexComponentIndex = 0;
        size_t vertexComponentOffset = 0;
        for (EVertexComponent vertexComponent : inVertexBuffer->GetVertexLayout().VertexComponents)
        {
            const size_t vertexComponentSize = VertexData::GetVertexComponentSize(vertexComponent);
            glEnableVertexAttribArray(vertexComponentIndex);
            glBindBuffer(GL_ARRAY_BUFFER, vertexBufferGL->GetGLBuffer());
            glVertexAttribPointer(vertexComponentIndex, 3, GL_FLOAT, GL_FALSE, vertexBufferGL->GetVertexSize(), (void*)vertexComponentOffset);
            vertexComponentIndex++;
            vertexComponentOffset += vertexComponentSize;
        }

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferGL->GetGLBuffer());
        glDrawElements(GL_TRIANGLES, indexBufferGL->GetNumIndices(), GL_UNSIGNED_INT, 0);
    }

    void RenderDeviceGL::SetRasteriserState(RasteriserState* inState)
    {
        if (inState == nullptr)
            inState = mDefaultRasteriserState;

        RasteriserStateGL* glRasterState = (RasteriserStateGL*)inState;
        
        if (!glRasterState->mDepthClipEnabled || glRasterState->mCullMode == RasteriserStateCullMode::None)
        {
            glDisable(GL_CULL_FACE);
        }
        else
        {
            glEnable(GL_CULL_FACE);
            glCullFace(glRasterState->mCullMode == RasteriserStateCullMode::Front ? GL_FRONT : GL_BACK);
            glFrontFace(GL_CW);
        }
    }

    void RenderDeviceGL::SetDepthStencilState(DepthStencilState* inState)
    {
        DepthStencilStateGL* glStencilState = (DepthStencilStateGL*)inState;
        mDefaultDepthStencilState = glStencilState;
    }

    void RenderDeviceGL::SetShaderUniformMat4x4(const char* inName, const glm::mat4 inMat)
    {
        GLuint MatrixID = glGetUniformLocation(mActiveShaderProgram->GetGLProgram(), inName);

        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &inMat[0][0]);
    }

    void RenderDeviceGL::SetShaderUniformVec4(const char* inName, const glm::vec4 inVec)
    {

    }
}
