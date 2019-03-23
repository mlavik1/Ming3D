#include "shader_tokeniser.h"

#include <fstream>
#include <string>
#include <numeric>
#include "Debug/debug.h"
#include "Debug/st_assert.h"

namespace Ming3D
{
    Tokeniser::Tokeniser(const char* inSourceText)
    {
        mSourceText = inSourceText;
        mSourceStringPos = mSourceText.c_str();
    }

    bool Tokeniser::isOperator(const char* inToken)
    {
        return (mPunctuators.find(*inToken) != mPunctuators.end() || mDoublePunctuators.find(inToken) != mDoublePunctuators.end());
    }

    Token Tokeniser::ParseToken()
    {
        Token outToken;

        // Remove whitespaces and tabs from beginning, and count linebreaks
        while (true)
        {
            if (*mSourceStringPos == '\n')
            {
                mLineNumber++;
            }
            else if (*mSourceStringPos != ' ' && *mSourceStringPos != '\t')
            {
                break;
            }
            mSourceStringPos++;
        }

        // End of file
        if (*mSourceStringPos == 0)
        {
            outToken.mTokenType = ETokenType::EndOfFile;
            return outToken;
        }
        
        const char* tokenStart = mSourceStringPos;

        bool isNumericLiteral = false;
        bool isFloatLiteral = false;
        bool isPunctuator = false;
        bool isComment = false;
        const char* commentStartPos;

        // Evaluate the first character
        if (isdigit(*mSourceStringPos))
        {
            isNumericLiteral = true;
        }
        else if (*mSourceStringPos == '/' && *(mSourceStringPos + 1) == '/')
        {
            isComment = true;
            commentStartPos = mSourceStringPos;
            mSourceStringPos++;
        }
        else if (mPunctuators.find(*mSourceStringPos) != mPunctuators.end())
        {
            isPunctuator = true;
        }
        mSourceStringPos++;

        // Evaluate the rest of the characters
        while (*mSourceStringPos != 0)
        {
            const char& currChar = *mSourceStringPos;
            if (isComment)
            {
                if (currChar == '\n')
                {
                    mSourceStringPos++;
                    mLineNumber++;
                    break;
                }
            }
            else if (isPunctuator)
            {
                // Double punctuator? (>=, ==, !=, etc..)
                if (mDoublePunctuators.find(std::string { *(mSourceStringPos -1), *mSourceStringPos }) != mDoublePunctuators.end())
                {
                    mSourceStringPos++;
                }
                break;
            }
            else if (currChar == ';' || currChar == ' ' || currChar == '\n' || currChar == '\t' || ((!isNumericLiteral || currChar != '.') && mPunctuators.find(currChar) != mPunctuators.end()))
            {
                break;
            }
            else if (isNumericLiteral)
            {
                if (currChar == '.')
                {
                    isFloatLiteral = true;
                }
                else if (currChar == 'f')
                {
                    if (isFloatLiteral)
                    {
                        mSourceStringPos++;
                        break;
                    }
                    else
                    {
                        LOG_ERROR() << "Invalid character 'f' in numeric literal: " << tokenStart;
                    }
                }
                else if (!isdigit(currChar))
                {
                    LOG_ERROR() << "Invalid character in numerical literal: " << tokenStart;
                }
            }
            mSourceStringPos++;
        }

        const char* tokenEnd = isComment ? commentStartPos : mSourceStringPos;

        outToken.mTokenString = std::string(tokenStart, tokenEnd - tokenStart);

        if (isNumericLiteral && isFloatLiteral)
        {
            outToken.mTokenType = ETokenType::FloatLiteral;
            outToken.mFloatValue = strtof(outToken.mTokenString.c_str(), nullptr);
        }
        else if (isNumericLiteral)
        {
            outToken.mTokenType = ETokenType::IntegerLiteral;
            outToken.mIntValue = strtol(outToken.mTokenString.c_str(), nullptr, 10);
        }
        else if (outToken.mTokenString == "true")
        {
            outToken.mTokenType = ETokenType::BooleanLiteral;
            outToken.mIntValue = 1;
        }
        else if (outToken.mTokenString == "false")
        {
            outToken.mTokenType = ETokenType::BooleanLiteral;
            outToken.mIntValue = 0;
        }
        else if (isPunctuator)
        {
            outToken.mTokenType = ETokenType::Operator;
        }
        else
        {
            outToken.mTokenType = ETokenType::Identifier;
        }

        outToken.mLineNumber = mLineNumber;

        return outToken;
    }

    TokenParser::TokenParser(const char* inShaderCode)
        : mTokeniser(inShaderCode)
    {
        const char* shaderStringPos = inShaderCode;
        while (*shaderStringPos != 0)
        {
            Token token = mTokeniser.ParseToken();
            if (token.mTokenString != "")
            {
                mTokens.push_back(token);
            }
            else if (token.mTokenType == ETokenType::EndOfFile)
            {
                break;
            }
        }
    }

    void TokenParser::Advance()
    {
        mCurrentTokenIndex++;
    }

    const Token& TokenParser::GetCurrentToken()
    {
        return mTokens[mCurrentTokenIndex];
    }

    const Token& TokenParser::GetTokenFromOffset(const int inOffset)
    {
        return mTokens[mCurrentTokenIndex + inOffset];
    }

    bool TokenParser::HasMoreTokens()
    {
        return mCurrentTokenIndex < mTokens.size();
    }

}
