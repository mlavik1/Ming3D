#ifndef MING3D_TEXTURE_GL_H
#define MING3D_TEXTURE_GL_H

#include "texture.h"
#include <gl/glew.h>

namespace Ming3D
{
    class TextureGL : public Texture
    {
    private:
        GLuint mGLTexture;

    public:
        virtual void BufferTexture() override;
        GLuint GetGLTexture();
    };
}

#endif
