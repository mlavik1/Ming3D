#ifndef MING3D_SHADERINFO_H
#define MING3D_SHADERINFO_H

#include <string>
#include <vector>
#include "shader_tokeniser.h"
#include "glm/glm.hpp"
#include <map>

namespace Ming3D
{
    enum class EShaderDatatype
    {
        None = 0, Struct = 1, Float = 2, Int = 3, Bool = 4, Void = 5, Vec2 = 6, Vec3 = 7, Vec4 = 8, Mat4x4 = 9, Texture2D = 10
    };

    class ShaderStructMember; // fwd.decl.

    class ShaderDatatypeInfo
    {
    public:
        EShaderDatatype mDatatype;
        std::string mName = "";
        std::string mParentType = "";
        std::vector<ShaderStructMember> mMemberVariables;

        ShaderDatatypeInfo();
        ShaderDatatypeInfo(EShaderDatatype inType, std::string inName);
        ShaderDatatypeInfo(EShaderDatatype inType, std::string inName, std::vector<ShaderStructMember> inChildren);

        size_t GetDataSize() const;
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

    class ConstantBufferInfo
    {
    public:
        std::string mName;
        std::vector<ShaderVariableInfo> mShaderUniforms;
    };

    /**
    * Base class for converted (and possibly compiled) shader data.
    */
    class ConvertedShaderProgram
    {
    public:
        virtual ~ConvertedShaderProgram() {}
    };

    class ParsedShader
    {
    public:
        std::vector<ShaderDatatypeInfo> mStructDefinitions;
        std::vector<ShaderFunctionDefinition*> mFunctionDefinitions;
        ShaderFunctionDefinition* mMainFunction = nullptr;
        ShaderDatatypeInfo mInput;
        ShaderDatatypeInfo mOutput;

        ~ParsedShader()
        {
            if(mMainFunction != nullptr)
                delete mMainFunction;
        }
    };

    class ParsedShaderProgram
    {
    public:
        std::string mProgramPath;

        ParsedShader* mVertexShader = nullptr;
        ParsedShader* mFragmentShader = nullptr;

        std::vector<ShaderDatatypeInfo> mStructDefinitions;
        std::vector<ShaderFunctionDefinition*> mFunctionDefinitions;
        std::vector<ConstantBufferInfo> mConstantBufferInfos;
        std::vector<ShaderVariableInfo> mUniforms;
        std::vector<ShaderTextureInfo> mShaderTextures;
        ConvertedShaderProgram* mConvertedProgram = nullptr;

        ~ParsedShaderProgram()
        {
            for (ShaderFunctionDefinition* def : mFunctionDefinitions)
                delete def; 
            if (mVertexShader != nullptr)
                delete mVertexShader;
            if (mFragmentShader != nullptr)
                delete mFragmentShader;
            if (mConvertedProgram != nullptr)
                delete mConvertedProgram;
        }
    };

    struct ShaderParserParams
    {
        std::string mShaderProgramPath;
        std::unordered_map<std::string, std::string> mPreprocessorDefinitions;
    };
}

#endif
