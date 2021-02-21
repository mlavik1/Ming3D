#ifndef MING3D_VERTEX_BUFFER_GL_H
#define MING3D_VERTEX_BUFFER_GL_H

#include "vertex_buffer.h"

#include <GL/glew.h>

namespace Ming3D::Rendering
{
    class VertexBufferGL : public VertexBuffer
    {
    private:
        GLuint mGLBuffer;
        bool mAssigned = false;

    public:
        VertexBufferGL(const VertexLayout& inLayout, EBufferUsage usage);
        virtual ~VertexBufferGL();

        void SetGLBuffer(GLuint inBuffer);
        GLuint GetGLBuffer();

        size_t mDataSize = 0;
    };
}

#endif
