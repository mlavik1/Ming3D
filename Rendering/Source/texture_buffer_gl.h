#ifndef MING3D_TEXTURE_GL_H
#define MING3D_TEXTURE_GL_H

#include "texture_buffer.h"
#include <GL/glew.h>

namespace Ming3D
{
    class TextureBufferGL : public TextureBuffer
    {
    private:
        GLuint mGLTexture = -1;

    public:
        virtual ~TextureBufferGL();
        void SetGLTexture(GLuint inTextureID);
        GLuint GetGLTexture();
    };
}

#endif
