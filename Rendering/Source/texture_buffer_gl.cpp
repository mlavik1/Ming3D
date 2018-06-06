#include "texture_buffer_gl.h"

#include "Debug/st_assert.h"

namespace Ming3D
{
    TextureBufferGL::~TextureBufferGL()
    {
        if (mGLTexture != -1)
        {
            glDeleteTextures(1, &mGLTexture);
        }
    }

    void TextureBufferGL::SetGLTexture(GLuint inTextureID)
    {
        mGLTexture = inTextureID;
    }

    GLuint TextureBufferGL::GetGLTexture()
    {
        return mGLTexture;
    }
}
