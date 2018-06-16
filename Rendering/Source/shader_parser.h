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

namespace Ming3D { namespace ShaderConverter
{
    enum class TerminatorType
    {
        LineBreak, Parenthesis, CurlyBrackets
    };

    class ShaderStructMember
    {
    public:
        std::string mVariableType;
        std::string mVariableName;
        std::string mSemantic;
    };

    class ShaderVariableInfo
    {
    public:
        std::string mVariableType;
        std::string mVariableName;
    };

    class ShaderStructInfo
    {
    public:
        std::string mStructName;
        std::vector<ShaderStructMember> mMemberVariables;

        ShaderStructInfo() {}

        ShaderStructInfo(const std::string& inName, const std::vector<ShaderStructMember>& inMembers)
        {
            mStructName = inName;
            mMemberVariables = inMembers;
        }
    };

    class ShaderFunctionInfo
    {
    public:
        std::string mFunctionName;
        std::string mReturnValueType;
        std::vector<ShaderVariableInfo> mParameters;
    };

    class ShaderUniformInfo
    {
    public:
        std::string mUniformType;
        std::string mUniformName;
    };

    class ShaderTextureInfo
    {
    public:
        std::string mTextureType; // TODO: Use enum for texture types
        std::string mTextureName;
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

    enum class EExpressionType
    {
        BinaryOperation, UnaryOperation, Literal, VariableAccess, FunctionCall
    };

    class ShaderExpression
    {
    public:
        std::string mValueType; // TODO
        virtual EExpressionType GetExpressionType() const = 0;
    };

    class BinaryOperationExpression : public ShaderExpression
    {
    public:
        std::string mOperator;
        ShaderExpression* mLeftOperand;
        ShaderExpression* mRightOperand;
        virtual EExpressionType GetExpressionType() const override { return EExpressionType::BinaryOperation; }
    };

    class UnaryOperationExpression : public ShaderExpression
    {
    public:
        std::string mOperator;
        ShaderExpression* mOperand;
        virtual EExpressionType GetExpressionType() const override { return EExpressionType::UnaryOperation; }
    };

    class LiteralExpression : public ShaderExpression
    {
    public:
        Token mToken;
        virtual EExpressionType GetExpressionType() const override { return EExpressionType::Literal; }
    };

    class VariableAccessExpression : public ShaderExpression
    {
    public:
        ShaderExpression* mOuterExpression = nullptr;
        Token mIdentifier;
        virtual EExpressionType GetExpressionType() const override { return EExpressionType::VariableAccess; }
    };

    class FunctionCallExpression : public ShaderExpression
    {
    public:
        Token mIdentifier;
        std::vector<ShaderExpression*> mParameterExpressions;
        virtual EExpressionType GetExpressionType() const override { return EExpressionType::FunctionCall; }
    };

    enum class EStatementType
    {
        VariableDefinition, Expression, ReturnStatement, ControlStatement
    };

    class ShaderStatement
    {
    public:
        virtual EStatementType GetStatementType() const = 0;
    };

    class VariableDefinitionStatement : public ShaderStatement
    {
    public:
        std::string mVariableType;
        std::string mVariableName;
        ShaderExpression* mAssignmentExpression = nullptr;
        virtual EStatementType GetStatementType() const override { return EStatementType::VariableDefinition; };
    };

    class ExpressionStatement : public ShaderStatement
    {
    public:
        ShaderExpression* mExpression = nullptr;
        virtual EStatementType GetStatementType() const override { return EStatementType::Expression; };
    };

    class ReturnStatement : public ShaderStatement
    {
    public:
        ShaderExpression* mReturnValueExpression = nullptr;
        virtual EStatementType GetStatementType() const override { return EStatementType::ReturnStatement; };
    };

    class ShaderStatementBlock
    {
    public:
        std::vector<ShaderStatement*> mStatements;
    };

    class ControlStatement : public ShaderStatement
    {
    public:
        std::string mIdentifier;
        ShaderStatementBlock* mExpressionStatements = nullptr;
        ShaderStatementBlock* mStatementBlock = nullptr;
        virtual EStatementType GetStatementType() const override { return EStatementType::ControlStatement; };
    };

    class ShaderFunctionDefinition
    {
    public:
        ShaderFunctionInfo mFunctionInfo;
        ShaderStatementBlock* mStatementBlock;
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
        std::vector<ShaderStructInfo> mStructs;
        std::vector<ShaderFunctionInfo> mFunctions;
    };

    class ParsedShader
    {
    public:
        std::vector<ShaderStructInfo> mStructDefinitions;
        std::vector<ShaderFunctionDefinition*> mFunctionDefinitions;
        ShaderFunctionDefinition* mMainFunction = nullptr;
        ShaderStructInfo mInput;
        ShaderStructInfo mOutput;
    };

    class ParsedShaderProgram
    {
    public:
        ParsedShader* mVertexShader = nullptr;
        ParsedShader* mFragmentShader = nullptr;

        std::vector<ShaderStructInfo> mStructDefinitions;
        std::vector<ShaderFunctionDefinition*> mFunctionDefinitions;
        std::vector<ShaderUniformInfo> mShaderUniforms;
        std::vector<ShaderTextureInfo> mShaderTextures;
    };

    /**
    * Responsible for parsing shader code, and extracting a syntactic tree.
    * The ShaderWriter will use that to convert shaders to API-specific shading languages (GLSL/HLSL)
    */
    class ShaderParser
    {
    private:
        std::set<std::string> mBuiltinDataTypes = { "float", "int", "bool", "void", "Texture2D" };
        std::set<std::string> mControlStatementIdentifiers = { "for", "while", "if", "else" };
        std::unordered_map<std::string, ShaderStructInfo> mBuiltinStructs;

        std::unordered_map<std::string, OperatorInfo> mUnaryOperatorsMap;
        std::unordered_map<std::string, OperatorInfo> mBinaryOperatorsMap;

        std::unordered_map<std::string, ShaderVariableInfo> mVariablesInScope;
        std::unordered_map<std::string, ShaderStructInfo> mStructsInScope;
        std::unordered_map<std::string, ShaderFunctionInfo> mFunctionsInScope;

        std::stack<ShaderParserScope> mScopeStack;

        OperatorInfo mDefaultOuterOperatorInfo = { "", 999, EOperatorAssociativity::LeftToRight };

        void OnParseError(TokenParser inTokenParser, const char* inErrorString);

        void PushScopeStack();
        void PopScopeStack();
        void AddVariableToScope(const ShaderVariableInfo& inVariable);
        void AddStructToScope(const ShaderStructInfo& inStruct);
        void AddFunctionToScope(const ShaderFunctionInfo& inFunc);

        bool GetStructInfo(const char* inStructName, ShaderStructInfo& outStructInfo, ParsedShaderProgram* inShaderProgram, ParsedShader* inShader);

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
        EParseResult ParseStructBody(TokenParser& inTokenParser, ShaderStructInfo* outStructInfo);

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
        ParsedShaderProgram* ParseShaderProgram(const char* inShaderProgramPath);
    };
}}

#endif