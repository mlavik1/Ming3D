#include "shader_preprocessor.h"
#include <ctype.h>

namespace Ming3D
{
    ShaderPreprocessor::ShaderPreprocessor(const std::string& inShaderText)
        : mSourceText(inShaderText)
    {

    }

    bool ShaderPreprocessor::IsTokenDelimiter(char inChar)
    {
        return (*mReadPos == ' ' || *mReadPos == '(' || *mReadPos == ')' || *mReadPos == ',' || *mReadPos == '+' || *mReadPos == '-' || *mReadPos == '/' || *mReadPos == '*' || *mReadPos == '=');
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
        else
            return PreprocessorDirective::Invalid;
    }

    std::string ShaderPreprocessor::ParseToken()
    {
        while (IsTokenDelimiter(*mReadPos))
        {
            if (!IsCurrentScopeIgnored())
                mOutputStream << *mReadPos;
            mReadPos++;
        }

        const char* startPos = mReadPos;

        if (*mReadPos == '#')
            mReadPos++;

        while (!IsTokenDelimiter(*mReadPos) && *mReadPos != 0 && *mReadPos !='\n')
        {
            mReadPos++;
        }

        if (mReadPos != startPos)
            return std::string(startPos, mReadPos);
        else
            return "";
    }

    void ShaderPreprocessor::ProcessToken(const std::string& inToken)
    {
        // handle preprocessor directives
        if (inToken[0] == '#')
        {
            PreprocessorDirective directive = GetPreprocessorDirective(inToken);

            switch (directive)
            {
            case PreprocessorDirective::Define:
            {
                if (!IsCurrentScopeIgnored())
                {
                    std::string defName = ParseToken();
                    std::string defVal = ParseToken();
                    mDefinitions.emplace(defName, defVal);
                }
                break;
            }
            case PreprocessorDirective::Ifdef:
            case PreprocessorDirective::Ifndef:
            {
                std::string def = ParseToken();
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
            }
        }
        else if(!IsCurrentScopeIgnored())
        {
            // replace preprocessor definition
            auto defIter = mDefinitions.find(inToken);
            if (defIter != mDefinitions.end())
            {
                mOutputStream << defIter->second;
            }
            else
            {
                mOutputStream << inToken;
            }
        }
    }

    void ShaderPreprocessor::PreprocessShader(std::string& outShaderText)
    {
        mReadPos = mSourceText.data();
        while (*mReadPos != 0)
        {
            // handle comments
            if (mReadPos[0] == '/' && mReadPos[1] == '/')
            {
                while (*mReadPos != 0 && *mReadPos != '\n')
                    mReadPos++;
                mOutputStream << "\n";
            }
            
            // parse next token
            std::string token = ParseToken();

            // process token, if current scope should be printed
            ProcessToken(token);
            
            // always print new-lines, to make sure line numbers are preserved
            while (*mReadPos == '\n')
            {
                mOutputStream << *mReadPos;
                mReadPos++;
            }
        }
        outShaderText = mOutputStream.str();
    }
}
