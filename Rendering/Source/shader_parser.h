#ifndef MING3D_SHADERPARSER_H
#define MING3D_SHADERPARSER_H

/*
* *** TODO ***
* Move all classes (except ShaderParser) to a separate header file.
*/

#include <vector>
#include <set>
#include <unordered_map>
#include <stack>
#include "shader_tokeniser.h"
#include "shader_info.h"

namespace Ming3D
{
    enum class TerminatorType
    {
        LineBreak, Parenthesis, CurlyBrackets
    };

    enum class EOperatorAssociativity
    {
        LeftToRight,
        RightToLeft
    };

    class OperatorInfo
    {
    public:
        std::string mOperator;
        int mPrecedence;
        EOperatorAssociativity mAssociativity;
    };

    enum EParseResult
    {
        Parsed,
        NotParsed,
        Error
    };

    class ShaderParserScope
    {
    public:
        std::vector<ShaderVariableInfo> mVariables;
        std::vector<ShaderDatatypeInfo> mStructs;
        std::vector<ShaderFunctionInfo> mFunctions;
    };

    /**
    * Responsible for parsing shader code, and extracting a syntactic tree.
    * The ShaderWriter will use that to convert shaders to API-specific shading languages (GLSL/HLSL)
    */
    class ShaderParser
    {
    private:
        ShaderParserParams mParams;

        std::set<std::string> mControlStatementIdentifiers = { "for", "while", "if", "else" };

        std::unordered_map<std::string, ShaderDatatypeInfo> mBuiltinDatatypes;

        std::unordered_map<std::string, OperatorInfo> mUnaryOperatorsMap;
        std::unordered_map<std::string, OperatorInfo> mBinaryOperatorsMap;

        std::unordered_map<std::string, ShaderVariableInfo> mVariablesInScope;
        std::unordered_map<std::string, ShaderFunctionInfo> mFunctionsInScope;

        std::unordered_map<std::string, ShaderDatatypeInfo> mCurrentProgramStructDefs;
        std::unordered_map<std::string, ShaderDatatypeInfo> mCurrentShaderStructDefs;

        std::stack<ShaderParserScope> mScopeStack;

        OperatorInfo mDefaultOuterOperatorInfo = { "", 999, EOperatorAssociativity::LeftToRight };

        void OnParseError(TokenParser inTokenParser, const std::string& inErrorString);

        void PushScopeStack();
        void PopScopeStack();
        void AddVariableToScope(const ShaderVariableInfo& inVariable);
        void AddFunctionToScope(const ShaderFunctionInfo& inFunc);

        bool GetTypeInfo(const std::string& inName, ShaderDatatypeInfo& outTypeInfo);

        bool IsTypeIdentifier(const char* inTokenString);
        bool IsControlStatementIdentifier(const char* inTokenString);

        bool TryGetUnaryOperator(const char* inOperator, OperatorInfo& outOperatorInfo);
        bool TryGetBinaryOperator(const char* inOperator, OperatorInfo& outOperatorInfo);

        EParseResult ParseIdentifierWithSuffixOperator(TokenParser& inTokenParser, ShaderExpression** outExpression);

        /**
        * Parses an atom (either literals or parenthesised expressions).
        * @param inTokenParser  Token parser, containing all tokens and the index of the current one.
        * @param outExpression  Expression returned by this function (or nullptr if it fails).
        */
        EParseResult ParseAtom(TokenParser& inTokenParser, ShaderExpression** outExpression);
        
        /*
        * Parse an expression recursively.
        * Uses precedence climbing.
        * @param inTokenParser  Token parser, containing all tokens and the index of the current one.
        * @param inOperator     Operator of outer expression, if this is a sub-expression. Use mDefaultOuterOperatorInfo if not.
        * @param outExpression  Expression returned by this function (or nullptr if it fails).
        */
        EParseResult ParseExpression(TokenParser& inTokenParser, const OperatorInfo& inOperator, ShaderExpression** outExpression);
        
        /*
        * Parse the body of a struct.
        * @param inTokenParser  Token parser, containing all tokens and the index of the current one.
        * @param outExpression  Expression returned by this function (or nullptr if it fails).
        */
        EParseResult ParseStructBody(TokenParser& inTokenParser, ShaderDatatypeInfo* outStructInfo);

        /*
        * Parse a function definition.
        * @param inTokenParser  Token parser, containing all tokens and the index of the current one.
        * @param outExpression  Expression returned by this function (or nullptr if it fails).
        */
        EParseResult ParseFunctionDefinition(TokenParser& inTokenParser, ShaderFunctionDefinition** outDefinition);

        /*
        * Parse one single statement (recursively)
        * @param inTokenParser  Token parser, containing all tokens and the index of the current one.
        * @param outExpression  Expression returned by this function (or nullptr if it fails).
        */
        EParseResult ParseStatement(TokenParser& inTokenParser, ShaderStatement** outStatement);
        
        /*
        * Parse a block of statements.
        * @param inTokenParser Token parser, containing all tokens and the index of the current one.
        * @param inTerminator  Statement block terminator (will stop parsing when it finds one).
        * @param outExpression  Expression returned by this function (or nullptr if it fails).
        */
        EParseResult ParseStatementBlock(TokenParser& inTokenParser, TerminatorType inTerminator, ShaderStatementBlock** outStatementBlock);

    public:
        ShaderParser();

        /*
        * Parse the source code of a shader program.
        * Send the parsed program to a ShaderWriter to convert it.
        */
        ParsedShaderProgram* ParseShaderProgram(const ShaderParserParams& inParams);
    };
}

#endif
