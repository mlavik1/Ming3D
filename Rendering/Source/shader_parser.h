#ifndef MING3D_SHADERPARSER_H
#define MING3D_SHADERPARSER_H

#include <vector>
#include <set>
#include <unordered_map>
#include <stack>
#include "shader_tokeniser.h"

namespace Ming3D { namespace ShaderConverter
{
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
        VariableDefinition, Expression, ReturnStatement
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

    class ShaderParser
    {
    private:
        std::set<std::string> mBuiltinDataTypes = { "float", "int", "bool", "void", "Texture2D" };
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

        bool TryGetUnaryOperator(const char* inOperator, OperatorInfo& outOperatorInfo);
        bool TryGetBinaryOperator(const char* inOperator, OperatorInfo& outOperatorInfo);

        EParseResult ParseIdentifierWithSuffixOperator(TokenParser& inTokenParser, ShaderExpression** outExpression);

        /**
        * Parses an atom (either literals or parenthesised expressions).
        **/
        EParseResult ParseAtom(TokenParser& inTokenParser, ShaderExpression** outExpression);
        /**
        * Parses an expression.
        **/
        
        EParseResult ParseExpression(TokenParser& inTokenParser, const OperatorInfo& inOperator, ShaderExpression** outExpression);
        
        EParseResult ParseStructBody(TokenParser& inTokenParser, ShaderStructInfo* outStructInfo);

        EParseResult ParseFunctionDefinition(TokenParser& inTokenParser, ShaderFunctionDefinition** outDefinition);

        EParseResult ParseStatement(TokenParser& inTokenParser, ShaderStatement** outStatement);
        
        EParseResult ParseStatementBlock(TokenParser& inTokenParser, char inTerminator, ShaderStatementBlock** outStatementBlock);

    public:
        ShaderParser();

        ParsedShaderProgram* ParseShaderProgram(const char* inShaderProgramPath);
    };
}}

#endif
