#ifdef MING3D_OPENGL
#include "render_device_gl.h"
#include "vertex_buffer_gl.h"
#include "index_buffer_gl.h"
#include "render_target_gl.h"
#include "shader_program_gl.h"
#include "texture_buffer_gl.h"
#include "constant_buffer_gl.h"
#include "Debug/debug.h"
#include "Debug/st_assert.h"
#include "shader_writer_glsl.h"
#include "Debug/debug_stats.h"
#include <GL/glew.h>
#include "blend_state_gl.h"

namespace Ming3D::Rendering
{
    RenderDeviceGL::RenderDeviceGL()
    {
        const GLubyte* vendor = glGetString(GL_VENDOR);
        const GLubyte* renderer = glGetString(GL_RENDERER);
        LOG_INFO() << "Graphics Vendor: " << vendor;
        LOG_INFO() << "Graphics Renderer: " << renderer;

        if (glewInit())
        {
            LOG_ERROR() << "Failed to initialise GLEW";
        }

        mDefaultRasteriserState = (RasteriserStateGL*)CreateRasteriserState(RasteriserStateCullMode::Back, true);
        
        DepthStencilStateDesc dssDesc;
        dssDesc.mDepthFunc = DepthStencilDepthFunc::Less;
        dssDesc.mDepthEnabled = true;
        dssDesc.mDepthWrite = true;
        mDefaultDepthStencilState = (DepthStencilStateGL*)CreateDepthStencilState(dssDesc);

        SetRasteriserState(mDefaultRasteriserState);
        SetDepthStencilState(mDefaultDepthStencilState);
    }

    std::unique_ptr<RenderTarget> RenderDeviceGL::CreateRenderTarget(RenderWindow* inWindow)
    {
        TextureInfo textureInfo;
        textureInfo.mWidth = inWindow->GetWindow()->GetWidth();
        textureInfo.mHeight = inWindow->GetWindow()->GetHeight();

        std::unique_ptr<RenderTarget> renderTarget = CreateRenderTarget(textureInfo, 1);
        RenderTargetGL* renderTargetGl = static_cast<RenderTargetGL*>(renderTarget.get());
        renderTargetGl->mRenderWindow = inWindow;
        return renderTarget;
    }

    std::unique_ptr<RenderTarget> RenderDeviceGL::CreateRenderTarget(TextureInfo inTextureInfo, int numTextures)
    {
        std::unique_ptr<RenderTargetGL> renderTarget = std::make_unique<RenderTargetGL>();
        renderTarget->mTextureInfo = inTextureInfo;

        GLuint frameBuffer = 0;
        glGenFramebuffers(1, &frameBuffer);
        glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);

        TextureBufferGL* colourBuffer = new TextureBufferGL();

        for (int i = 0; i < numTextures; i++)
        {
            GLuint renderTexture;
            glGenTextures(1, &renderTexture);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, renderTexture);

            glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA8, inTextureInfo.mWidth, inTextureInfo.mHeight);
            //glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, inTextureInfo.mWidth, inTextureInfo.mHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, renderTexture, 0);

            renderTarget->mAttachments.push_back(GL_COLOR_ATTACHMENT0 + i);

            colourBuffer->SetGLTexture(renderTexture);
            renderTarget->mColourBuffers.push_back(colourBuffer);
        }

        renderTarget->mFrameBufferID = frameBuffer;

        GLuint depthTexture;
        glGenTextures(1, &depthTexture);
        glBindTexture(GL_TEXTURE_2D, depthTexture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, inTextureInfo.mWidth, inTextureInfo.mHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
        float borderColour[] = { 1.0f, 1.0f, 1.0f, 1.0f }; // depth is 1.0 outside border (neccessary for shadows)
        glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColour);
        glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthTexture, 0);

        CheckGLErrors("CreateRenderTarget");

        TextureBufferGL* depthBuffer = new TextureBufferGL();
        depthBuffer->SetGLTexture(depthTexture);
        renderTarget->mDepthRenderBuffer = depthBuffer;

        return renderTarget;
    }

    VertexBuffer* RenderDeviceGL::CreateVertexBuffer(VertexData* inVertexData, EBufferUsage usage)
    {
        VertexBufferGL* vertexBuffer = new VertexBufferGL(inVertexData->GetVertexLayout(), usage);
        vertexBuffer->mDataSize = inVertexData->GetNumVertices() * inVertexData->GetVertexSize();
        GLenum usageGL = usage == EBufferUsage::StaticDraw ? GL_STATIC_DRAW : GL_DYNAMIC_DRAW;
        GLuint vbo;
        glGenBuffers(1, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, vertexBuffer->mDataSize, inVertexData->GetDataPtr(), usageGL);
        vertexBuffer->SetGLBuffer(vbo);

        CheckGLErrors("CreateVertexBuffer");
        return vertexBuffer;
    }

    void RenderDeviceGL::UpdateVertexBuffer(VertexBuffer* inVertexBuffer, VertexData* inVertexData)
    {
        VertexBufferGL* vertexBuffer = static_cast<VertexBufferGL*>(inVertexBuffer);
        size_t newSize = inVertexData->GetNumVertices() * inVertexData->GetVertexSize();
        // Same size + dynamic buffer => update buffer data
        if(vertexBuffer->GetUsage() == EBufferUsage::DynamicDraw && newSize == vertexBuffer->mDataSize)
        {
            glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer->GetGLBuffer());
            void* dataPtr = glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
            memcpy(dataPtr, inVertexData->GetDataPtr(), vertexBuffer->mDataSize);
            glUnmapBuffer(GL_ARRAY_BUFFER);
        }
        // Else => recreate buffer (expensive!)
        else
        {
            LOG_WARNING() << "Recreating vertex buffer. This can be expesnive."
                << "Please consider setting it to dynamic, and call UpdateVertexBuffer with vertex data of same size";
            GLenum usageGL = vertexBuffer->GetUsage() == EBufferUsage::StaticDraw ? GL_STATIC_DRAW : GL_DYNAMIC_DRAW;
            vertexBuffer->mDataSize = newSize;
            glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer->GetGLBuffer());
            glBufferData(GL_ARRAY_BUFFER, vertexBuffer->mDataSize, inVertexData->GetDataPtr(), usageGL);
        }

        CheckGLErrors("UpdateVertexBuffer");
    }

    IndexBuffer* RenderDeviceGL::CreateIndexBuffer(IndexData* inIndexData, EBufferUsage usage)
    {
        IndexBufferGL* indexBuffer = new IndexBufferGL(usage);
        GLenum usageGL = usage == EBufferUsage::StaticDraw ? GL_STATIC_DRAW : GL_DYNAMIC_DRAW;
        GLuint ibo;
        glGenBuffers(1, &ibo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, inIndexData->GetNumIndices() * sizeof(unsigned int), inIndexData->GetData(), usageGL);
        indexBuffer->SetGLBuffer(ibo);
        indexBuffer->SetNumIndices(inIndexData->GetNumIndices());

        CheckGLErrors("CreateIndexBuffer");
        return indexBuffer;
    }

    void RenderDeviceGL::UpdateIndexBuffer(IndexBuffer* inIndexBuffer, IndexData* inIndexData)
    {
        IndexBufferGL* indexBuffer = static_cast<IndexBufferGL*>(inIndexBuffer);
        size_t oldSize = indexBuffer->GetNumIndices() * sizeof(unsigned int);
        size_t newSize = inIndexData->GetNumIndices() * sizeof(unsigned int);
        // Same size + dynamic buffer => update buffer data
        if (inIndexBuffer->GetUsage() == EBufferUsage::DynamicDraw && newSize == oldSize)
        {
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer->GetGLBuffer());
            void* dataPtr = glMapBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_WRITE_ONLY);
            memcpy(dataPtr, inIndexData->GetData(), newSize);
            glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);
        }
        // Else => recreate buffer (expensive!)
        else
        {
            if(inIndexBuffer->GetUsage() != EBufferUsage::DynamicDraw)
            {
                LOG_WARNING() << "Recreating index buffer. This can be expesnive."
                    << "Please consider setting it to dynamic, and call UpdateVertexBuffer with vertex data of same size";
            }

            GLenum usageGL = inIndexBuffer->GetUsage() == EBufferUsage::StaticDraw ? GL_STATIC_DRAW : GL_DYNAMIC_DRAW;
            indexBuffer->SetNumIndices(inIndexData->GetNumIndices());
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer->GetGLBuffer());
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, newSize, inIndexData->GetData(), usageGL);
        }

        CheckGLErrors("UpdateVertexBuffer");
    }

    ShaderProgram* RenderDeviceGL::CreateShaderProgram(ParsedShaderProgram* parsedProgram)
    {
        ShaderWriterGLSL shaderWriter;

        ConvertedShaderProgramGLSL* convertedProgram = static_cast<ConvertedShaderProgramGLSL*>(parsedProgram->mConvertedProgram);
        if (convertedProgram == nullptr)
        {
            ShaderProgramDataGLSL convertedShaderData;
            if (!shaderWriter.WriteShader(parsedProgram, convertedShaderData))
                return nullptr;
            convertedProgram = new ConvertedShaderProgramGLSL();
            convertedProgram->mShaderProgramData = convertedShaderData;
            parsedProgram->mConvertedProgram = convertedProgram;
        }

        ShaderProgramDataGLSL convertedShaderData = convertedProgram->mShaderProgramData;

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

        CheckGLErrors("CreateShaderProgram");

        ShaderProgramGL* shaderProgram = new ShaderProgramGL();
        shaderProgram->SetGLProgram(program);
        shaderProgram->SetGLVertexShader(vs);
        shaderProgram->SetGLFragmentShader(fs);

        return shaderProgram;
    }

    TextureBuffer* RenderDeviceGL::CreateTextureBuffer(TextureInfo inTextureInfo, void* inTextureData)
    {
        ADD_DEBUG_STAT_INT("CreateTextureBuffer", 1);

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

        GLint wrapMode = inTextureInfo.mTextureWrapMode == TextureWrapMode::Clamp ? GL_CLAMP_TO_EDGE : GL_REPEAT;

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapMode);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapMode);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        CheckGLErrors("CreateTextureBuffer");

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

    DepthStencilState* RenderDeviceGL::CreateDepthStencilState(DepthStencilStateDesc inDesc)
    {
        DepthStencilStateGL* depthStencilState = new DepthStencilStateGL();
        depthStencilState->mDepthEnabled = inDesc.mDepthEnabled;
        depthStencilState->mDepthWrite = inDesc.mDepthWrite;
        switch (inDesc.mDepthFunc)
        {
        case DepthStencilDepthFunc::Less:
            depthStencilState->mDepthFunc = GL_LESS;
            break;
        case DepthStencilDepthFunc::LEqual:
            depthStencilState->mDepthFunc = GL_LEQUAL;
            break;
        case DepthStencilDepthFunc::Equal:
            depthStencilState->mDepthFunc = GL_EQUAL;
            break;
        case DepthStencilDepthFunc::GEqual:
            depthStencilState->mDepthFunc = GL_LEQUAL;
            break;
        case DepthStencilDepthFunc::Greater:
            depthStencilState->mDepthFunc = GL_GREATER;
            break;
        }
        // TODO: enable/disable depth?
        return depthStencilState;
    }

    BlendState* RenderDeviceGL::CreateBlendState(bool enableBlend, EBlendMode blendMode)
    {
        BlendStateGL* blendState = new BlendStateGL(enableBlend, blendMode);
        return blendState;
    }

    ConstantBuffer* RenderDeviceGL::CreateConstantBuffer(size_t inSize)
    {
        ConstantBufferGL* cb = new ConstantBufferGL();

        GLuint ubo = 0;
        char* initialData = new char[inSize];
        glGenBuffers(1, &ubo);
        glBindBuffer(GL_UNIFORM_BUFFER, ubo);
        glBufferData(GL_UNIFORM_BUFFER, inSize, initialData, GL_DYNAMIC_DRAW);
        glBindBuffer(GL_UNIFORM_BUFFER, 0);
        delete[] initialData;
        cb->mGLBuffer = ubo;

        CheckGLErrors("CreateConstantBuffer");

        return cb;
    }

    void RenderDeviceGL::SetTexture(const TextureBuffer* inTexture, int inSlot)
    {
        ADD_FRAME_STAT_INT("SetTexture", 1);

        glEnable(GL_TEXTURE_2D); // TODO
        TextureBufferGL* glTexture = (TextureBufferGL*)inTexture;
        glActiveTexture(GL_TEXTURE0 + inSlot);
        glBindTexture(GL_TEXTURE_2D, glTexture->GetGLTexture());

        CheckGLErrors("SetTexture");
    }

    void RenderDeviceGL::SetActiveShaderProgram(ShaderProgram* inProgram)
    {
        ADD_FRAME_STAT_INT("SetActiveShaderProgram", 1);

        if (mActiveShaderProgram == inProgram)
            return;

        mActiveShaderProgram = static_cast<ShaderProgramGL*>(inProgram);
        if (mActiveShaderProgram != nullptr)
        {
            glUseProgram(mActiveShaderProgram->GetGLProgram());
        }

        CheckGLErrors("SetActiveShaderProgram");
    }

    void RenderDeviceGL::BeginRenderWindow(RenderWindow* inWindow)
    {
        mRenderWindow = static_cast<RenderWindowGL*>(inWindow);

        mRenderWindow->GetWindow()->BeginRender();
    }

    void RenderDeviceGL::EndRenderWindow(RenderWindow* inWindow)
    {
        __Assert(mRenderWindow == inWindow);

        mRenderWindow->GetWindow()->EndRender();
        mRenderWindow = nullptr;
    }

    void RenderDeviceGL::SetRenderTarget(RenderTarget* inTarget)
    {
        mRenderTarget = (RenderTargetGL*)inTarget;

        glBindFramebuffer(GL_FRAMEBUFFER, mRenderTarget->mFrameBufferID);
        glDrawBuffers(1, mRenderTarget->mAttachments.data());

        CheckGLErrors("BeginRenderTarget");

        SetDepthStencilState(mDefaultDepthStencilState); // TODO
    }

    void RenderDeviceGL::BlitRenderTargetToWindow(RenderTarget* inTarget, RenderWindow* inTargetWindow)
    {
        SetRenderTarget(inTarget);

        RenderTargetGL* glTarget = (RenderTargetGL*)inTarget;

        BlitRenderTarget(glTarget, mRenderWindow);

        mRenderTarget = nullptr;
    }

    void RenderDeviceGL::BlitRenderTarget(RenderTargetGL* inSourceTarget, RenderWindow* inTargetWindow)
    {
        const int w = inTargetWindow->GetWindow()->GetWidth();
        const int h = inTargetWindow->GetWindow()->GetHeight();
        glBindFramebuffer(GL_READ_FRAMEBUFFER, inSourceTarget->mFrameBufferID);
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
        glViewport(0, 0, w, h);
        glBlitFramebuffer(0, 0, w, h, 0, 0, w, h, GL_COLOR_BUFFER_BIT, GL_NEAREST);

        CheckGLErrors("BlitRenderTarget");
    }

    void RenderDeviceGL::BeginViewport(unsigned int x, unsigned int y, unsigned int width, unsigned int height)
    {
        glEnable(GL_SCISSOR_TEST);
        glScissor(x, y, width, height);
        glDepthMask(GL_TRUE);
        glClearColor(0.0f, 0.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glClear(GL_DEPTH_BUFFER_BIT);
        glDisable(GL_SCISSOR_TEST);
        glViewport(x, y, width, height);
    }


    void RenderDeviceGL::CheckGLErrors(const char* callerName)
    {
        GLenum errCode;
        while((errCode = glGetError()) != GL_NO_ERROR)
        {
            std::string errString = (char*)glewGetErrorString(errCode);
            LOG_ERROR() << errString << ", error code: " << errCode << ", in: " << callerName;
        }
    }

    void RenderDeviceGL::RenderPrimitive(VertexBuffer* inVertexBuffer, IndexBuffer* inIndexBuffer, unsigned int startIndex, unsigned int indexCount)
    {
        ADD_FRAME_STAT_INT("RenderPrimitive", 1);

        VertexBufferGL* vertexBufferGL = static_cast<VertexBufferGL*>(inVertexBuffer);
        IndexBufferGL* indexBufferGL = static_cast<IndexBufferGL*>(inIndexBuffer);
        
        size_t vertexComponentIndex = 0;
        size_t vertexComponentOffset = 0;
        for (EVertexComponent vertexComponent : inVertexBuffer->GetVertexLayout().VertexComponents)
        {
            const size_t vertexComponentSize = VertexData::GetVertexComponentSize(vertexComponent);
            glEnableVertexAttribArray(vertexComponentIndex);
            glBindBuffer(GL_ARRAY_BUFFER, vertexBufferGL->GetGLBuffer());
            glVertexAttribPointer(vertexComponentIndex, VertexData::GetVertexComponentSize(vertexComponent) / sizeof(float), GL_FLOAT, GL_FALSE, vertexBufferGL->GetVertexSize(), (void*)vertexComponentOffset);
            vertexComponentIndex++;
            vertexComponentOffset += vertexComponentSize;
        }

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferGL->GetGLBuffer());
        glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(indexCount), GL_UNSIGNED_INT, reinterpret_cast<void*>(startIndex * sizeof(unsigned int)) /* TODO: Handle 16 bit index buffer */);

        CheckGLErrors("RenderPrimitive");
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
            glFrontFace(GL_CCW);
        }

        CheckGLErrors("SetRasteriserState");
    }

    void RenderDeviceGL::SetDepthStencilState(DepthStencilState* inState)
    {
        DepthStencilStateGL* glStencilState = (DepthStencilStateGL*)inState;
        mDefaultDepthStencilState = glStencilState;

        if (glStencilState->mDepthEnabled)
            glEnable(GL_DEPTH_TEST);
        else
            glDisable(GL_DEPTH_TEST);
        glDepthFunc(glStencilState->mDepthFunc);
        glDepthRange(0.0f, 1.0f);

        if (glStencilState->mDepthWrite)
            glDepthMask(GL_TRUE);
        else
            glDepthMask(GL_FALSE);

        CheckGLErrors("SetDepthStencilState");
    }

    void RenderDeviceGL::SetBlendState(BlendState* inState)
    {
        BlendStateGL* blendState = (BlendStateGL*)inState;
        
        if(blendState->mBlendEnabled)
            glEnable(GL_BLEND);
        else
            glDisable(GL_BLEND);
        glBlendFunc(blendState->mSrcFactor, blendState->mDstFactor);

        CheckGLErrors("SetDepthStencilState");
    }

    void RenderDeviceGL::SetConstantBufferData(ConstantBuffer* inConstantBuffer, void* inData, size_t inSize)
    {
        ConstantBufferGL* cb = static_cast<ConstantBufferGL*>(inConstantBuffer);

        glBindBuffer(GL_UNIFORM_BUFFER, cb->mGLBuffer);
        GLvoid* ptr = glMapBuffer(GL_UNIFORM_BUFFER, GL_WRITE_ONLY);
        memcpy(ptr, inData, inSize);
        glUnmapBuffer(GL_UNIFORM_BUFFER);
        
        CheckGLErrors("SetConstantBufferData");
    }

    void RenderDeviceGL::BindConstantBuffer(ConstantBuffer* inConstantBuffer, const char* inName, ShaderProgram* inProgram)
    {
        ConstantBufferGL* cb = static_cast<ConstantBufferGL*>(inConstantBuffer);
        ShaderProgramGL* prog = static_cast<ShaderProgramGL*>(inProgram);

        // get index
        unsigned int block_index = glGetUniformBlockIndex(prog->GetGLProgram(), inName);
        // bind UBO to shader program
        glBindBufferBase(GL_UNIFORM_BUFFER, block_index, cb->mGLBuffer);

        CheckGLErrors("BindConstantBuffer");
    }

    void RenderDeviceGL::SetShaderUniformFloat(const std::string& inName, float inVal)
    {
        ADD_FRAME_STAT_INT("SetConstantBufferData", 1);

        GLint loc = mActiveShaderProgram->GetUniformLocation(inName);
        if (loc != -1)
            glUniform1f(loc, inVal);
        else
            LOG_WARNING() << "Shader program has no uniform with name: " << inName;

        CheckGLErrors("SetShaderUniformFloat");
    }

    void RenderDeviceGL::SetShaderUniformInt(const std::string& inName, int inVal)
    {
        ADD_FRAME_STAT_INT("SetConstantBufferData", 1);

        GLint loc = mActiveShaderProgram->GetUniformLocation(inName);
        if (loc != -1)
            glUniform1i(loc, inVal);
        else
            LOG_WARNING() << "Shader program has no uniform with name: " << inName;

        CheckGLErrors("SetShaderUniformInt");
    }

    void RenderDeviceGL::SetShaderUniformMat4x4(const std::string& inName, const glm::mat4 inMat)
    {
        ADD_FRAME_STAT_INT("SetConstantBufferData", 1);

        GLint loc = mActiveShaderProgram->GetUniformLocation(inName);
        if (loc != -1)
            glUniformMatrix4fv(loc, 1, GL_FALSE, &inMat[0][0]);
        else
            LOG_WARNING() << "Shader program has no uniform with name: " << inName;

        CheckGLErrors("SetShaderUniformMat4x4");
    }

    void RenderDeviceGL::SetShaderUniformVec2(const std::string& inName, const glm::vec2 inVec)
    {
        ADD_FRAME_STAT_INT("SetConstantBufferData", 1);

        GLint loc = mActiveShaderProgram->GetUniformLocation(inName);
        if (loc != -1)
            glUniform2fv(loc, 1, (float*)&inVec[0]);
        else
            LOG_WARNING() << "Shader program has no uniform with name: " << inName;

        CheckGLErrors("SetShaderUniformVec2");
    }

    void RenderDeviceGL::SetShaderUniformVec3(const std::string& inName, const glm::vec3 inVec)
    {
        ADD_FRAME_STAT_INT("SetConstantBufferData", 1);

        GLint loc = mActiveShaderProgram->GetUniformLocation(inName);
        if (loc != -1)
            glUniform3fv(loc, 1, (float*)&inVec[0]);
        else
            LOG_WARNING() << "Shader program has no uniform with name: " << inName;
        CheckGLErrors("SetShaderUniformVec3");
    }

    void RenderDeviceGL::SetShaderUniformVec4(const std::string& inName, const glm::vec4 inVec)
    {
        ADD_FRAME_STAT_INT("SetConstantBufferData", 1);

        GLint loc = mActiveShaderProgram->GetUniformLocation(inName);
        if (loc != -1)
            glUniform4fv(loc, 1, (float*)&inVec[0]);
        else
            LOG_WARNING() << "Shader program has no uniform with name: " << inName;

        CheckGLErrors("SetShaderUniformVec4");
    }
}
#endif
