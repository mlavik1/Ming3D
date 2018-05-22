#include "shader_writer_hlsl.h"

#include <sstream>
#include <fstream>
#include "Debug/debug.h"

namespace Ming3D { namespace ShaderConverter
{
    std::string ShaderWriterHLSL::GetVariableIdentifierString(const std::string inName)
    {
        if (mReservedKeywords.find(inName) != mReservedKeywords.end())
        {
            return std::string("_") + inName;
        }
        else
        {
            return inName;
        }
    }

    std::string ShaderWriterHLSL::GetConvertedType(const std::string inString)
    {
        if (inString == "vec2")
            return "float2";
        else if (inString == "vec3")
            return "float3";
        else if (inString == "vec4")
            return "float4";
        else if (inString == "mat3")
            return "float3x3";
        else if (inString == "mat4")
            return "float4x4";
        return inString;
    }

    void ShaderWriterHLSL::WriteVariableDeclaration(ShaderStream& inStream, ShaderVariableInfo inVarInfo)
    {
        inStream << GetConvertedType(inVarInfo.mVariableType) << " " << GetVariableIdentifierString(inVarInfo.mVariableName);
    }

    void ShaderWriterHLSL::WriteFunctionDefinition(ShaderStream& inStream, const ShaderFunctionDefinition* inFunctionDef, bool isMainFunction)
    {
        const bool isPSMain = isMainFunction && mCurrentShader == mCurrentProgram->mFragmentShader;
        std::string returnType = GetConvertedType(inFunctionDef->mFunctionInfo.mReturnValueType);
        if (isPSMain)
        {
            returnType = "float4";
        }
        else if (isMainFunction && inFunctionDef->mFunctionInfo.mParameters.size() > 1)
        {
            returnType = GetConvertedType(inFunctionDef->mFunctionInfo.mParameters[1].mVariableType);
        }
        inStream << returnType << " ";
        inStream << inFunctionDef->mFunctionInfo.mFunctionName << "(";

        // Main functions only have one parameter (they return the output parameter)
        const size_t numParams = isMainFunction ? 1 : inFunctionDef->mFunctionInfo.mParameters.size();

        // Write parameter declarations
        for (int iParam = 0; iParam < numParams; iParam++)
        {
            ShaderVariableInfo param = inFunctionDef->mFunctionInfo.mParameters[iParam];
            WriteVariableDeclaration(inStream, param);
            if (iParam != numParams - 1)
                inStream << ", ";
        }

        inStream << ")";

        if (isPSMain)
        {
            inStream << " : SV_TARGET";
        }

        inStream << "\n{\n";
        inStream.AddIndent();

        // Declare output variable
        if (isMainFunction && !isPSMain)
        {
            ShaderVariableInfo param = inFunctionDef->mFunctionInfo.mParameters[1];
            WriteVariableDeclaration(inStream, param);
            inStream << ";\n\n";
        }

        WriteStatementBlock(inStream, inFunctionDef->mStatementBlock);

        inStream << "\n";

        // write output to next shader
        if (isPSMain)
        {
            inStream << "return _PSReturnValue;\n"; // TODO
        }
        else
        {
            inStream << "return " << inFunctionDef->mFunctionInfo.mParameters[1].mVariableName << ";\n";
        }

        inStream.RemoveIndent();
        inStream << "\n}";

    }

    void ShaderWriterHLSL::WriteFunctionCallParameters(ShaderStream& inStream, const std::vector<ShaderExpression*>& inParams)
    {
        for (int iParam = 0; iParam < inParams.size(); iParam++)
        {
            WriteExpression(inStream, inParams[iParam]);
            if (iParam != inParams.size() - 1)
            {
                inStream << ",";
            }
        }
    }

    void ShaderWriterHLSL::WriteExpression(ShaderStream& inStream, const ShaderExpression* inExpression)
    {
        switch (inExpression->GetExpressionType())
        {
        case EExpressionType::Literal:
        {
            LiteralExpression* literalExpression = (LiteralExpression*)inExpression;
            if (literalExpression->mValueType == "float")
            {
                inStream << literalExpression->mToken.mFloatValue;
            }
            else
            {
                inStream << literalExpression->mToken.mTokenString;
            }
            break;
        }
        case EExpressionType::VariableAccess:
        {
            VariableAccessExpression* varAccessExpr = (VariableAccessExpression*)inExpression;
            if (varAccessExpr->mOuterExpression != nullptr)
            {
                WriteExpression(inStream, varAccessExpr->mOuterExpression);
                inStream << ".";
            }
            else
            {
                // Register referenced uniform
                if (mAvailableUniforms.find(varAccessExpr->mIdentifier.mTokenString) != mAvailableUniforms.end())
                {
                    mReferencedUniforms.insert(varAccessExpr->mIdentifier.mTokenString);
                }
            }
            inStream << GetVariableIdentifierString(varAccessExpr->mIdentifier.mTokenString);
            break;
        }
        case EExpressionType::FunctionCall:
        {
            FunctionCallExpression* funcCallExpr = (FunctionCallExpression*)inExpression;

            // TODO: Create a map for variable assignments to replace function calls
            if (funcCallExpr->mIdentifier.mTokenString == "SetFragmentColour")
            {
                inStream << "float4 _PSReturnValue = "; // TODO: What if it has already been called?
                WriteFunctionCallParameters(inStream, funcCallExpr->mParameterExpressions);
            }
            else if (funcCallExpr->mIdentifier.mTokenString == "ReadTexture")
            {
                WriteExpression(inStream, funcCallExpr->mParameterExpressions[0]);
                inStream << ".Sample(";
                WriteExpression(inStream, funcCallExpr->mParameterExpressions[0]);
                inStream << "_sampler, ";
                WriteExpression(inStream, funcCallExpr->mParameterExpressions[1]);
                inStream << ")";
            }
            else
            {
                std::string functionName = funcCallExpr->mIdentifier.mTokenString;
                inStream << functionName;
                inStream << "(";
                WriteFunctionCallParameters(inStream, funcCallExpr->mParameterExpressions);
                inStream << ")";
            }
            break;
        }
        case EExpressionType::BinaryOperation:
        {
            BinaryOperationExpression* binaryExpr = (BinaryOperationExpression*)inExpression;
            const bool wrapInParenthesis = binaryExpr->mOperator != "=";
            if(wrapInParenthesis)
                inStream << "(";

            if (binaryExpr->mOperator == "*")
            {
                inStream << "mul(";
                WriteExpression(inStream, binaryExpr->mRightOperand);
                inStream << ", ";
                WriteExpression(inStream, binaryExpr->mLeftOperand);
                inStream << ")";
            }
            else
            {
                WriteExpression(inStream, binaryExpr->mLeftOperand);
                inStream << binaryExpr->mOperator;
                WriteExpression(inStream, binaryExpr->mRightOperand);
            }

            if(wrapInParenthesis)
                inStream << ")";
            break;
        }
        case EExpressionType::UnaryOperation:
        {
            UnaryOperationExpression* unaryExpr = (UnaryOperationExpression*)inExpression;
            
            // TODO

            break;
        }
        default:
            LOG_ERROR() << "Unhandled expression type";
            break;
        }
    }

    void ShaderWriterHLSL::WriteStatement(ShaderStream& inStream, const ShaderStatement* inStatement)
    {
        switch (inStatement->GetStatementType())
        {
        case EStatementType::VariableDefinition:
        {
            break;
        }
        case EStatementType::Expression:
        {
            ExpressionStatement* expressionStatement = (ExpressionStatement*)inStatement;
            WriteExpression(inStream, expressionStatement->mExpression);
            inStream << ";";
            break;
        }
        case EStatementType::ReturnStatement:
        {
            ReturnStatement* returnStatement = (ReturnStatement*)inStatement;
            inStream << "return";
            if (returnStatement->mReturnValueExpression != nullptr)
            {
                inStream << " ";
                WriteExpression(inStream, returnStatement->mReturnValueExpression);
            }
            inStream << ";";
            break;
        }
        default:
            LOG_ERROR() << "Unhandled statement type";
            break;
        }
    }

    void ShaderWriterHLSL::WriteStatementBlock(ShaderStream& inStream, const ShaderStatementBlock* inStatementBlock)
    {
        for (ShaderStatement* currStatement : inStatementBlock->mStatements)
        {
            WriteStatement(inStream, currStatement);
            inStream << "\n";
        }
    }

    bool ShaderWriterHLSL::WriteShader(ParsedShaderProgram* inParsedShaderProgram, ShaderProgramDataHLSL& outData)
    {
        mCurrentProgram = inParsedShaderProgram;

        std::vector<ParsedShader*> shaders;
        if (inParsedShaderProgram->mVertexShader)
            shaders.push_back(inParsedShaderProgram->mVertexShader);
        if (inParsedShaderProgram->mFragmentShader)
            shaders.push_back(inParsedShaderProgram->mFragmentShader);

        for (ShaderUniformInfo uniformInfo : inParsedShaderProgram->mShaderUniforms)
        {
            mAvailableUniforms.emplace(uniformInfo.mUniformName);
        }
        for (ShaderTextureInfo textureInfo : inParsedShaderProgram->mShaderTextures)
        {
            mAvailableUniforms.emplace(textureInfo.mTextureName);
        }

        for (ParsedShader* currShader : shaders)
        {
            mCurrentShader = currShader;
            mReferencedUniforms.clear();

            std::vector<ShaderUniformInfo> currShaderUniforms;

            ShaderStream shaderBodyStream;
            ShaderStream shaderHeaderStream;

            // *** Shader body ***

            for (const ShaderFunctionDefinition* functionDef : currShader->mFunctionDefinitions)
            {
                WriteFunctionDefinition(shaderBodyStream, functionDef, false);
            }

            WriteFunctionDefinition(shaderBodyStream, currShader->mMainFunction, true);


            // *** Shader header ***

            std::vector<ShaderStructInfo> structDefinitions = currShader->mStructDefinitions;
            structDefinitions.insert(structDefinitions.end(), inParsedShaderProgram->mStructDefinitions.begin(), inParsedShaderProgram->mStructDefinitions.end());

            // Write structs (from shader program and shader)
            for (ShaderStructInfo structInfo : structDefinitions)
            {
                shaderHeaderStream << "struct " << structInfo.mStructName << "\n" << "{" << "\n";

                shaderHeaderStream.AddIndent();
                for (const ShaderStructMember member : structInfo.mMemberVariables)
                {
                    shaderHeaderStream << GetConvertedType(member.mVariableType) << " " << member.mVariableName;
                    if (member.mSemantic != "")
                    {
                        shaderHeaderStream << " : " << member.mSemantic;
                    }
                    shaderHeaderStream << ";\n";
                }
                shaderHeaderStream.RemoveIndent();

                shaderHeaderStream << "\n};\n";
            }
            shaderHeaderStream << "\n";

            // Write constant buffers
            shaderHeaderStream << "cbuffer SHADER_CONSTANT_BUFFER : register(b0)\n";
            shaderHeaderStream << "{\n";
            shaderHeaderStream.AddIndent();
            for (const ShaderUniformInfo uniformInfo : inParsedShaderProgram->mShaderUniforms)
            {
                if (mReferencedUniforms.find(uniformInfo.mUniformName) != mReferencedUniforms.end())
                {
                    shaderHeaderStream << GetConvertedType(uniformInfo.mUniformType) << " " << uniformInfo.mUniformName << ";\n";
                    currShaderUniforms.push_back(uniformInfo);
                }
            }
            shaderHeaderStream.RemoveIndent();
            shaderHeaderStream << "}";
            shaderHeaderStream << "\n";

            // Write textures
            for (const ShaderTextureInfo textureInfo : inParsedShaderProgram->mShaderTextures)
            {
                if (mReferencedUniforms.find(textureInfo.mTextureName) != mReferencedUniforms.end())
                {
                    shaderHeaderStream << GetConvertedType(textureInfo.mTextureType) << " " << textureInfo.mTextureName << ";\n";
                    shaderHeaderStream << "SamplerState " << textureInfo.mTextureName << "_sampler" << ";\n";
                }
            }

            std::stringstream outStream;
            outStream << shaderHeaderStream.GetStream().str() << "\n" << shaderBodyStream.GetStream().str();

            if (currShader == inParsedShaderProgram->mVertexShader)
            {
                outData.mVertexShader.mSource = outStream.str();
                outData.mVertexShader.mUniforms = currShaderUniforms;
            }
            else
            {
                outData.mFragmentShader.mSource = outStream.str();
                outData.mFragmentShader.mUniforms = currShaderUniforms;
            }

            std::ofstream oFile;
            oFile.open(std::string("converted_shader_." + (currShader == inParsedShaderProgram->mVertexShader ? std::string("vs") : std::string("fs"))));
            oFile << outStream.str().c_str();
            oFile.close();
        }

        return true;
    }
}}
