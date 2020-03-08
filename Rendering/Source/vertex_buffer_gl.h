#ifndef MING3D_VERTEX_BUFFER_GL_H
#define MING3D_VERTEX_BUFFER_GL_H

#include "vertex_buffer.h"

#include <GL/glew.h>

namespace Ming3D::Rendering
{
    class VertexBufferGL : public VertexBuffer
    {
    private:
        GLuint mGLBuffer = -1;

    public:
        virtual ~VertexBufferGL();

        void SetGLBuffer(GLuint inBuffer);
        GLuint GetGLBuffer();
    };
}

#endif
