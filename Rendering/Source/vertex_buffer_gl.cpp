#ifdef MING3D_OPENGL
#include "vertex_buffer_gl.h"

namespace Ming3D::Rendering
{
    VertexBufferGL::VertexBufferGL(const VertexLayout& inLayout, EBufferUsage usage)
    : VertexBuffer(inLayout, usage)
    {

    }

    VertexBufferGL::~VertexBufferGL()
    {
        if (mAssigned)
        {
            glDeleteBuffers(1, &mGLBuffer);
        }
    }

    void VertexBufferGL::SetGLBuffer(GLuint inBuffer)
    {
        mGLBuffer = inBuffer;
        mAssigned = true;
    }

    GLuint VertexBufferGL::GetGLBuffer()
    {
        return mGLBuffer;
    }
}
#endif
