#ifndef MING3D_SHADERPREPROCESSOR_H
#define MING3D_SHADERPREPROCESSOR_H

#include <string>
#include <sstream>
#include <stack>
#include <unordered_map>

namespace Ming3D
{
    enum class ShaderPreprocessorScopeType
    {
        IfBody,
        ElseBody
    };

    struct ShaderPreprocessorScope
    {
        ShaderPreprocessorScopeType mScopeType;
        bool mIgnoreContent = false;
    };

    enum class PreprocessorDirective
    {
        Define,
        Ifdef,
        Ifndef,
        Else,
        Endif,
        Invalid
    };

    class ShaderPreprocessor
    {
    private:
        const const std::string& mSourceText;
        std::stack<ShaderPreprocessorScope> mScopeStack;
        std::unordered_map<std::string, std::string> mDefinitions;
        const char* mReadPos;
        std::stringstream mOutputStream;

        bool IsTokenDelimiter(char inChar);
        PreprocessorDirective GetPreprocessorDirective(const std::string& inToken);
        std::string ParseToken();
        void ProcessToken(const std::string& inToken);
        bool IsCurrentScopeIgnored();

    public:
        ShaderPreprocessor(const std::string& inShaderText);
        void PreprocessShader(std::string& outShaderText);
    };
}

#endif
