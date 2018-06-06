#include "vertex_buffer_gl.h"

namespace Ming3D
{
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
