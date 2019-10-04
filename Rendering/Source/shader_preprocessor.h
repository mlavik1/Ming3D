#ifndef MING3D_SHADERPREPROCESSOR_H
#define MING3D_SHADERPREPROCESSOR_H

#include <string>
#include <sstream>
#include <stack>
#include <unordered_map>
#include "shader_tokeniser.h"

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
        TokenParser& mTokenParser;
        std::stack<ShaderPreprocessorScope> mScopeStack;
        std::unordered_map<std::string, std::string> mDefinitions;
        std::vector<Token> mPreprocessedTokens;

        PreprocessorDirective GetPreprocessorDirective(const std::string& inToken);
        void ProcessToken(Token inToken);
        bool IsCurrentScopeIgnored();

    public:
        ShaderPreprocessor(TokenParser& inTokenParser);
        void PreprocessShader();
    };
}

#endif
