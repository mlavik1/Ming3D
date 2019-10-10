#ifdef MING3D_OPENGL
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

    GLuint ShaderProgramGL::GetUniformLocation(const std::string& inName)
    {
        auto locIter = mCachedUniformLocations.find(inName);
        if (locIter != mCachedUniformLocations.end())
            return locIter->second;
        else if(mGLProgram)
        {
            GLuint loc = glGetUniformLocation(mGLProgram, inName.c_str());
            if (loc != -1)
                mCachedUniformLocations.emplace(inName, loc);
            return loc;
        }
    }
}
#endif
