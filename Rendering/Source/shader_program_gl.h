#ifndef MING3D_SHADER_PROGRAM_GL_H
#define MING3D_SHADER_PROGRAM_GL_H

#include "shader_program.h"
#include <gl/glew.h>

namespace Ming3D
{
	class ShaderProgramGL : public ShaderProgram
	{
	private:
		GLuint mGLProgram;
		GLuint mGLVertexShader;;
		GLuint mGLFragmentShader;

	public:
		void SetGLProgram(GLuint inProgram);
		void SetGLVertexShader(GLuint inVS);
		void SetGLFragmentShader(GLuint inFS);

		GLuint GetGLProgram() { return mGLProgram; }
		GLuint GetGLVertexShader() { return mGLVertexShader; }
		GLuint GetGLFragmentShader() { return mGLFragmentShader; }

	};
}

#endif
