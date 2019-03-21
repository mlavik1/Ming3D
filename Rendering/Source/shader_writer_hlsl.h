#ifndef MING3D_SHADERWRITER_HLSL_H
#define MING3D_SHADERWRITER_HLSL_H

#include "shader_writer.h"
#include "shader_parser.h"

#include <set>

namespace Ming3D { namespace ShaderConverter
{
    struct ShaderDataHLSL
    {
        std::string mSource;
        std::vector<ShaderVariableInfo> mUniforms;
        std::vector<ShaderTextureInfo> mTextures;
    };

    struct ShaderProgramDataHLSL
    {
        ShaderDataHLSL mVertexShader;
        ShaderDataHLSL mFragmentShader;
    };

    class ShaderWriterHLSL : public ShaderWriter
    {
    private:
        const std::set<std::string> mReservedKeywords = {};
        std::set<std::string> mAvailableUniforms;
        std::set<std::string> mReferencedUniforms;

        const ParsedShaderProgram* mCurrentProgram = nullptr;
        ParsedShader* mCurrentShader = nullptr;

        std::string GetVariableIdentifierString(const std::string inName);
        std::string GetConvertedType(const std::string inString);

        void WriteVariableDeclaration(ShaderStream& inStream, ShaderVariableInfo inVarInfo);
        void WriteFunctionDefinition(ShaderStream& inStream, const ShaderFunctionDefinition* inFunctionDef, bool isMainFunction);

        void WriteFunctionCallParameters(ShaderStream& inStream, const std::vector<ShaderExpression*>& inParams);
        void WriteExpression(ShaderStream& inStream, const ShaderExpression* inExpression);
        void WriteStatement(ShaderStream& inStream, const ShaderStatement* inStatement, const char* inTerminator = ";");
        void WriteStatementBlock(ShaderStream& inStream, const ShaderStatementBlock* inStatementBlock);

    public:
        bool WriteShader(const ParsedShaderProgram* inParsedShaderProgram, ShaderProgramDataHLSL& outData);
    };
}}

#endif
