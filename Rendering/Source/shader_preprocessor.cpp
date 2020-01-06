#include "shader_preprocessor.h"
#include <ctype.h>
#include <fstream>

namespace Ming3D
{
    ShaderPreprocessor::ShaderPreprocessor(TokenParser& inTokenParser)
        : mTokenParser(inTokenParser)
    {

    }

    bool ShaderPreprocessor::IsCurrentScopeIgnored()
    {
        return !mScopeStack.empty() && mScopeStack.top().mIgnoreContent;
    }

    PreprocessorDirective ShaderPreprocessor::GetPreprocessorDirective(const std::string& inToken)
    {
        if (inToken == "#define")
        {
            return PreprocessorDirective::Define;
        }
        else if (inToken == "#ifdef")
        {
            return PreprocessorDirective::Ifdef;
        }
        else  if (inToken == "#ifndef")
        {
            return PreprocessorDirective::Ifndef;
        }
        else if (inToken == "#else")
        {
            return PreprocessorDirective::Else;
        }
        else if (inToken == "#endif")
        {
            return PreprocessorDirective::Endif;
        }
        else if (inToken == "#include")
        {
            return PreprocessorDirective::Include;
        }
        else
            return PreprocessorDirective::Invalid;
    }

    void ShaderPreprocessor::ProcessToken(Token inToken)
    {
        // handle preprocessor directives
        if (inToken.mTokenType == ETokenType::PreprocessorDirective)
        {
            PreprocessorDirective directive = GetPreprocessorDirective(inToken.mTokenString);

            switch (directive)
            {
            case PreprocessorDirective::Define:
            {
                if (!IsCurrentScopeIgnored())
                {
                    mTokenParser.Advance();
                    const Token defNameToken = mTokenParser.GetCurrentToken();
                    mTokenParser.Advance();
                    const Token defValToken = mTokenParser.GetCurrentToken();
                    std::string defName = defNameToken.mTokenString;
                    std::string defVal = defValToken.mTokenType == ETokenType::NewLine ? "" : defValToken.mTokenString;
                    AddDefinition(defName, defVal);
                }
                break;
            }
            case PreprocessorDirective::Ifdef:
            case PreprocessorDirective::Ifndef:
            {
                mTokenParser.Advance();
                std::string def = mTokenParser.GetCurrentToken().mTokenString;
                ShaderPreprocessorScope scope;
                scope.mScopeType = ShaderPreprocessorScopeType::IfBody;
                scope.mIgnoreContent = IsCurrentScopeIgnored() || (mDefinitions.find(def) == mDefinitions.end()) == (directive == PreprocessorDirective::Ifdef);
                mScopeStack.push(scope);
                break;
            }
            case PreprocessorDirective::Else:
            {
                ShaderPreprocessorScope scope = mScopeStack.top();
                mScopeStack.pop();
                scope.mScopeType = ShaderPreprocessorScopeType::ElseBody;
                scope.mIgnoreContent = IsCurrentScopeIgnored() || !scope.mIgnoreContent;
                mScopeStack.push(scope);
                break;
            }
            case PreprocessorDirective::Endif:
            {
                mScopeStack.pop();
                break;
            }
            case PreprocessorDirective::Include:
            {
                mTokenParser.Advance();
                // Read included file
                std::string includePath = mTokenParser.GetCurrentToken().mTokenString;
                includePath = includePath.substr(1, includePath.size() - 2);
                std::ifstream shaderFile(includePath);
                std::string shaderString((std::istreambuf_iterator<char>(shaderFile)), std::istreambuf_iterator<char>());

                // Parse included file
                std::vector<Token> newTokens;
                Tokeniser tokeniser(shaderString.c_str());
                while (true)
                {
                    Token token = tokeniser.ParseToken();
                    if (token.mTokenString != "")
                    {
                        newTokens.push_back(token);
                    }
                    else if (token.mTokenType == ETokenType::EndOfFile)
                    {
                        break;
                    }
                }

                // Copy new tokens
                std::vector<Token>& tokens = mTokenParser.GetTokens();
                size_t numTokens = tokens.size();
                size_t nextTokenIndex = mTokenParser.GetCurrentTokenIndex() + 1;
                tokens.insert(tokens.begin() + nextTokenIndex, newTokens.begin(), newTokens.end());
                break;
            }
            }
        }
        else if(!IsCurrentScopeIgnored())
        {
            // replace preprocessor definition
            if (inToken.mTokenType == ETokenType::Identifier)
            {
                auto defIter = mDefinitions.find(inToken.mTokenString);
                if (defIter != mDefinitions.end())
                {
                    inToken.mTokenString = defIter->second;
                }
            }
            // push preprocessed token
            if(inToken.mTokenType != ETokenType::NewLine)
                mPreprocessedTokens.push_back(inToken);
        }
    }

    void ShaderPreprocessor::AddDefinition(const std::string name, const std::string value)
    {
        mDefinitions.emplace(name, value);
    }

    void ShaderPreprocessor::PreprocessShader()
    {
        while (mTokenParser.HasMoreTokens())
        {
            Token token = mTokenParser.GetCurrentToken();

            ProcessToken(token);

            mTokenParser.Advance();
        }
        mTokenParser.SetTokens(mPreprocessedTokens);
        mTokenParser.ResetPosition();
    }
}
