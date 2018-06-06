#ifndef MING3D_SHADER_PROGRAM_GL_H
#define MING3D_SHADER_PROGRAM_GL_H

#include "shader_program.h"
#include <gl/glew.h>

namespace Ming3D
{
    class ShaderProgramGL : public ShaderProgram
    {
    private:
        GLuint mGLProgram = -1;
        GLuint mGLVertexShader = -1;
        GLuint mGLFragmentShader = -1;

    public:
        virtual ~ShaderProgramGL();

        void SetGLProgram(GLuint inProgram);
        void SetGLVertexShader(GLuint inVS);
        void SetGLFragmentShader(GLuint inFS);

        GLuint GetGLProgram() { return mGLProgram; }
        GLuint GetGLVertexShader() { return mGLVertexShader; }
        GLuint GetGLFragmentShader() { return mGLFragmentShader; }

    };
}

#endif
