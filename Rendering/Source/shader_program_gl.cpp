#include "shader_program_gl.h"

namespace Ming3D
{
    ShaderProgramGL::~ShaderProgramGL()
    {
        if (mGLProgram != -1)
        {
            glDeleteProgram(mGLProgram);
        }
        if (mGLVertexShader != -1)
        {
            glDeleteShader(mGLVertexShader);
        }
        if (mGLFragmentShader != -1)
        {
            glDeleteShader(mGLFragmentShader);
        }
    }

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
