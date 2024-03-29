#ifdef MING3D_OPENGL
#include "shader_program_gl.h"

namespace Ming3D::Rendering
{
    ShaderProgramGL::~ShaderProgramGL()
    {
        if (mGLProgram != 0)
        {
            glDeleteProgram(mGLProgram);
        }
        if (mGLVertexShader != 0)
        {
            glDeleteShader(mGLVertexShader);
        }
        if (mGLFragmentShader != 0)
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

    GLint ShaderProgramGL::GetUniformLocation(const std::string& inName)
    {
        auto locIter = mCachedUniformLocations.find(inName);
        if (locIter != mCachedUniformLocations.end())
            return locIter->second;
        else if(mGLProgram)
        {
            GLint loc = glGetUniformLocation(mGLProgram, inName.c_str());
            if (loc != -1)
                mCachedUniformLocations.emplace(inName, loc);
            return loc;
        }
        return -1;
    }
}
#endif
