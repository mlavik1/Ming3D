#include "shader_program_gl.h"

namespace Ming3D
{
	void ShaderProgramGL::SetGLProgram(GLuint inProgram)
	{
		mGLProgram = inProgram;
	}

	void ShaderProgramGL::SetGLVertexShader(GLuint inVS)
	{
		mGLVertexShader = inVS;
	}

	void ShaderProgramGL::SetGLFragmentShader(GLuint inFS)
	{
		mGLFragmentShader = inFS;
	}
}
