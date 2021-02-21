#ifndef MING3D_INDEX_BUFFER_GL_H
#define MING3D_INDEX_BUFFER_GL_H

#include "index_buffer.h"

#include <GL/glew.h>

namespace Ming3D::Rendering
{
    class IndexBufferGL : public IndexBuffer
    {
    private:
        GLuint mGLBuffer;
        bool mAssigned = false;

    public:
        IndexBufferGL(EBufferUsage usage);
        ~IndexBufferGL();
        void SetGLBuffer(GLuint inBuffer);
        GLuint GetGLBuffer();
    };
}

#endif
