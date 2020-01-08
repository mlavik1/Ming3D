#ifndef MING3D_SHADERTOKENISER_H
#define MING3D_SHADERTOKENISER_H

#include <vector>
#include <set>
#include <unordered_map>
#include <stack>

namespace Ming3D
{
    enum class ETokenType
    {
        EndOfFile,
        FloatLiteral,
        IntegerLiteral,
        BooleanLiteral,
        StringLiteral,
        Operator,
        NewLine,
        PreprocessorDirective,
        Identifier
    };

    class Token
    {
    public:
        ETokenType mTokenType;
        std::string mTokenString;
        float mFloatValue;
        int mIntValue;
        int mLineNumber;
    };

    /**
    * Shader tokeniser.
    * Divides shader code into a set of tokens.
    * Used by the TokenParser.
    */
    class Tokeniser
    {
    private:
        const std::set<char> mPunctuators = { '[', ']', '(' , ')' , '{' , '}' , ',' , '.' , ';' , ':' , '<', '>', '=', '!', '+', '-', '*', '/', '&', '|', '?' };
        const std::set<std::string> mDoublePunctuators = { "==", ">=", "<=", "!=", "&&", "||", "+=", "*=", "/=", "&=", "|=" };

        std::string mSourceText;
        const char* mSourceStringPos;
        int mLineNumber = 1;

        bool isOperator(const char* inToken);

    public:
        Tokeniser(const char* inSourceText);

        Token ParseToken();
    };
    
    /**
    * Shader token parser.
    * Used by ShaderParser to parse tokens.
    */
    class TokenParser
    {
    private:
        Tokeniser mTokeniser;
        std::vector<Token> mTokens;
        size_t mCurrentTokenIndex = 0;

    public:
        TokenParser(const char* inShaderCode);
        void ResetPosition();
        void Advance();
        const Token& GetCurrentToken();
        const Token& GetTokenFromOffset(const int inOffset);
        bool HasMoreTokens();
        void SetTokens(std::vector<Token>& inTokens);

        size_t GetCurrentTokenIndex() { return mCurrentTokenIndex; };
        std::vector<Token>& GetTokens() { return mTokens; };
    };
}

#endif
