#include "texture_gl.h"

#include "Debug/st_assert.h"

namespace Ming3D
{
    void TextureGL::BufferTexture()
    {
        __Assert(mTextureData.data());

        glGenTextures(1, &mGLTexture);
        glBindTexture(GL_TEXTURE_2D, mGLTexture);

        GLint pixelFormat;
        if (mPixelFormat == PixelFormat::RGB)
            pixelFormat = GL_RGB;
        else if (mPixelFormat == PixelFormat::BGRA)
            pixelFormat = GL_BGRA;
        else
            pixelFormat = GL_RGBA;
        GLint internalFormat = (mPixelFormat == PixelFormat::RGB) ? GL_RGB : GL_RGBA;

        glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, mWidth, mHeight , 0, pixelFormat, GL_UNSIGNED_BYTE, mTextureData.data());

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }

    GLuint TextureGL::GetGLTexture()
    {
        return mGLTexture;
    }
}
