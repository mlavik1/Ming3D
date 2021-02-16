#ifdef MING3D_OPENGL
#include "vertex_buffer_gl.h"

namespace Ming3D::Rendering
{
    VertexBufferGL::VertexBufferGL(const VertexLayout& inLayout, EVertexBufferUsage usage)
    : VertexBuffer(inLayout, usage)
    {

    }

    VertexBufferGL::~VertexBufferGL()
    {
        if (mGLBuffer != -1)
        {
            glDeleteBuffers(1, &mGLBuffer);
        }
    }

    void VertexBufferGL::SetGLBuffer(GLuint inBuffer)
    {
        mGLBuffer = inBuffer;
    }

    GLuint VertexBufferGL::GetGLBuffer()
    {
        return mGLBuffer;
    }
}
#endif
