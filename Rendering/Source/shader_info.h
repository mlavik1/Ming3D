#ifndef MING3D_SHADERINFO_H
#define MING3D_SHADERINFO_H

#include <string>
#include <vector>
#include "shader_tokeniser.h"

namespace Ming3D
{
    enum class EShaderDatatype
    {
        Struct, Float, Int, Bool, Void, Vec2, Vec3, Vec4, Mat4x4, Texture2D, None
    };

    class ShaderStructMember; // fwd.decl.

    class ShaderDatatypeInfo
    {
    public:
        EShaderDatatype mDatatype;
        std::string mName = "";
        std::string mParentType = "";
        std::vector<ShaderStructMember> mMemberVariables;

        ShaderDatatypeInfo()
        {
            mDatatype = EShaderDatatype::None;
        }

        ShaderDatatypeInfo(EShaderDatatype inType, std::string inName)
        {
            mDatatype = inType;
            mName = inName;
        }

        ShaderDatatypeInfo(EShaderDatatype inType, std::string inName, std::vector<ShaderStructMember> inChildren)
        {
            mDatatype = inType;
            mName = inName;
            mMemberVariables = inChildren;
        }
    };

    class ShaderStructMember
    {
    public:
        ShaderDatatypeInfo mDatatype;
        std::string mName;
        std::string mSemantic;

        ShaderStructMember() {}

        ShaderStructMember(ShaderDatatypeInfo inType, std::string inName)
        {
            mDatatype = inType;
            mName = inName;
        }
    };

    class ShaderVariableInfo
    {
    public:
        std::string mName;
        ShaderDatatypeInfo mDatatypeInfo;
    };

    class ShaderFunctionInfo
    {
    public:
        std::string mFunctionName;
        ShaderDatatypeInfo mReturnValueType;
        std::vector<ShaderVariableInfo> mParameters;
    };

    class ShaderTextureInfo
    {
    public:
        std::string mTextureType; // TODO: Use enum for texture types
        std::string mTextureName;
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

    class ParsedShader
    {
    public:
        std::vector<ShaderDatatypeInfo> mStructDefinitions;
        std::vector<ShaderFunctionDefinition*> mFunctionDefinitions;
        ShaderFunctionDefinition* mMainFunction = nullptr;
        ShaderDatatypeInfo mInput;
        ShaderDatatypeInfo mOutput;
    };

    class ParsedShaderProgram
    {
    public:
        ParsedShader* mVertexShader = nullptr;
        ParsedShader* mFragmentShader = nullptr;

        std::vector<ShaderDatatypeInfo> mStructDefinitions;
        std::vector<ShaderFunctionDefinition*> mFunctionDefinitions;
        std::vector<ShaderVariableInfo> mShaderUniforms;
        std::vector<ShaderTextureInfo> mShaderTextures;
    };
}

#endif
