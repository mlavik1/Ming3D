#include "vertex_buffer_gl.h"

namespace Ming3D
{
	void VertexBufferGL::SetGLBuffer(GLuint inBuffer)
	{
		mGLBuffer = inBuffer;
	}

	GLuint VertexBufferGL::GetGLBuffer()
	{
		return mGLBuffer;
	}
}
