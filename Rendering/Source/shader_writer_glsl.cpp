#include "shader_writer_glsl.h"

#include <sstream>
#include <fstream>
#include "Debug/debug.h"

namespace Ming3D { namespace ShaderConverter
{
    std::string ShaderWriterGLSL::GetVariableIdentifierString(const std::string inName)
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

    std::string ShaderWriterGLSL::GetConvertedType(const std::string inString)
    {
        if (inString == "Texture2D")
            return "sampler2D";

        return inString;
    }

    void ShaderWriterGLSL::WriteVariableDeclaration(ShaderStream& inStream, ShaderVariableInfo inVarInfo)
    {
        inStream << GetConvertedType(inVarInfo.mVariableType) << " " << GetVariableIdentifierString(inVarInfo.mVariableName);
    }

    void ShaderWriterGLSL::WriteFunctionDefinition(ShaderStream& inStream, const ShaderFunctionDefinition* inFunctionDef, bool isMainFunction)
    {
        inStream << GetConvertedType(inFunctionDef->mFunctionInfo.mReturnValueType) << " ";
        inStream << inFunctionDef->mFunctionInfo.mFunctionName << "(";

        if (!isMainFunction)
        {
            for (int iParam = 0; iParam < inFunctionDef->mFunctionInfo.mParameters.size(); iParam++)
            {
                ShaderVariableInfo param = inFunctionDef->mFunctionInfo.mParameters[iParam];
                WriteVariableDeclaration(inStream, param);
                if (iParam != inFunctionDef->mFunctionInfo.mParameters.size() - 1)
                    inStream << ", ";
            }
        }

        inStream << ")\n";
        inStream << "{\n";
        inStream.AddIndent();

        if (isMainFunction)
        {
            if (inFunctionDef->mFunctionInfo.mParameters.size() > 0)
            {
                ShaderVariableInfo param = inFunctionDef->mFunctionInfo.mParameters[0];
                WriteVariableDeclaration(inStream, param);
                inStream << ";\n";
                for (const ShaderStructMember& member : mCurrentShader->mInput.mMemberVariables)
                {
                    inStream << GetVariableIdentifierString(param.mVariableName) << "." << member.mVariableName << "=" << "input_" << member.mVariableName << ";\n";
                }
            }
            if (inFunctionDef->mFunctionInfo.mParameters.size() > 1)
            {
                ShaderVariableInfo param = inFunctionDef->mFunctionInfo.mParameters[1];
                WriteVariableDeclaration(inStream, param);
                inStream << ";\n";
            }
            inStream << "\n";
        }

        WriteStatementBlock(inStream, inFunctionDef->mStatementBlock);

        inStream << "\n";

        // write output to next shader
        if (isMainFunction &&  inFunctionDef->mFunctionInfo.mParameters.size() > 1)
        {
            int vertexPosOutputIndex = -1;
            for (size_t iMember = 0; iMember < mCurrentShader->mOutput.mMemberVariables.size(); iMember++)
            {
                const ShaderStructMember& member = mCurrentShader->mOutput.mMemberVariables[iMember];
                inStream << "output_" << member.mVariableName << "=" << GetVariableIdentifierString(inFunctionDef->mFunctionInfo.mParameters[1].mVariableName) << "." << member.mVariableName << ";\n";
                if (member.mSemantic == "SV_POSITION")
                {
                    vertexPosOutputIndex = iMember;
                }
            }
            if (mCurrentShader == mCurrentShaderProgram->mVertexShader)
            {
                if (vertexPosOutputIndex != -1)
                {
                    inStream << "gl_Position = " << GetVariableIdentifierString(inFunctionDef->mFunctionInfo.mParameters[1].mVariableName) << "." << mCurrentShader->mOutput.mMemberVariables[vertexPosOutputIndex].mVariableName << ";\n";
                }
                else
                {
                    LOG_ERROR() << "Vertex shader output has no member of semantic type SV_POSITION";
                }
            }
        }

        inStream.RemoveIndent();
        inStream << "\n}";

    }

    void ShaderWriterGLSL::WriteFunctionCallParameters(ShaderStream& inStream, const std::vector<ShaderExpression*>& inParams)
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

    void ShaderWriterGLSL::WriteExpression(ShaderStream& inStream, const ShaderExpression* inExpression)
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
                inStream << "FragColour = ";
                WriteFunctionCallParameters(inStream, funcCallExpr->mParameterExpressions);
            }
            else
            {
                std::string functionName = funcCallExpr->mIdentifier.mTokenString;
                if (funcCallExpr->mIdentifier.mTokenString == "ReadTexture") // TODO: replace with map lookup?
                {
                    functionName = "texture";
                }
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
            WriteExpression(inStream, binaryExpr->mLeftOperand);
            inStream << binaryExpr->mOperator;
            WriteExpression(inStream, binaryExpr->mRightOperand);
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

    void ShaderWriterGLSL::WriteStatement(ShaderStream& inStream, const ShaderStatement* inStatement)
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

    void ShaderWriterGLSL::WriteStatementBlock(ShaderStream& inStream, const ShaderStatementBlock* inStatementBlock)
    {
        for (ShaderStatement* currStatement : inStatementBlock->mStatements)
        {
            WriteStatement(inStream, currStatement);
            inStream << "\n";
        }
    }

    bool ShaderWriterGLSL::WriteShader(ParsedShaderProgram* inParsedShaderProgram, ShaderProgramDataGLSL& outData)
    {
        mCurrentShaderProgram = inParsedShaderProgram;

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

            ShaderStream shaderBodyStream;
            ShaderStream shaderHeaderStream;

            // *** Shader body ***

            for (const ShaderFunctionDefinition* functionDef : currShader->mFunctionDefinitions)
            {
                WriteFunctionDefinition(shaderBodyStream, functionDef, false);
            }

            WriteFunctionDefinition(shaderBodyStream, currShader->mMainFunction, true);


            // *** Shader header ***

            shaderHeaderStream << "#version 430" << "\n\n";

            std::vector<ShaderStructInfo> structDefinitions = currShader->mStructDefinitions;
            structDefinitions.insert(structDefinitions.end(), inParsedShaderProgram->mStructDefinitions.begin(), inParsedShaderProgram->mStructDefinitions.end());

            // Write structs (from shader program and shader)
            for (ShaderStructInfo structInfo : structDefinitions)
            {
                shaderHeaderStream << "struct " << structInfo.mStructName << "\n" << "{" << "\n";

                shaderHeaderStream.AddIndent();
                for (const ShaderStructMember member : structInfo.mMemberVariables)
                {
                    shaderHeaderStream << GetConvertedType(member.mVariableType) << " " << member.mVariableName << ";\n";
                }
                shaderHeaderStream.RemoveIndent();

                shaderHeaderStream << "\n};\n";
            }
            shaderHeaderStream << "\n";

            // Write uniforms
            for (const ShaderUniformInfo uniformInfo : inParsedShaderProgram->mShaderUniforms)
            {
                if (mReferencedUniforms.find(uniformInfo.mUniformName) != mReferencedUniforms.end())
                {
                    shaderHeaderStream << "uniform " << GetConvertedType(uniformInfo.mUniformType) << " " << uniformInfo.mUniformName << ";\n";
                }
            }
            shaderHeaderStream << "\n";

            // Write textures
            for (const ShaderTextureInfo textureInfo : inParsedShaderProgram->mShaderTextures)
            {
                if (mReferencedUniforms.find(textureInfo.mTextureName) != mReferencedUniforms.end())
                {
                    shaderHeaderStream << "uniform " << GetConvertedType(textureInfo.mTextureType) << " " << textureInfo.mTextureName << ";\n";
                }
            }
            shaderHeaderStream << "\n";

            // Write shader input
            for (size_t i = 0; i < currShader->mInput.mMemberVariables.size(); i++)
            {
                const ShaderStructMember inputMember = currShader->mInput.mMemberVariables[i];
                shaderHeaderStream << "layout (location=" << i << ") ";
                shaderHeaderStream << "in " << GetConvertedType(inputMember.mVariableType) << " " << "input_" << inputMember.mVariableName << ";\n";
            }

            shaderHeaderStream << "\n";

            // Write shader output
            for (size_t i = 0; i < currShader->mOutput.mMemberVariables.size(); i++)
            {
                const ShaderStructMember outputMember = currShader->mOutput.mMemberVariables[i];
                shaderHeaderStream << "layout (location=" << i << ") ";
                shaderHeaderStream << "out " << outputMember.mVariableType << " output_" << outputMember.mVariableName << ";\n";
            }

            shaderHeaderStream << "\n";

            if (currShader == inParsedShaderProgram->mFragmentShader)
            {
                shaderHeaderStream << "out vec4 FragColour;\n";
            }

            std::stringstream outStream;
            outStream << shaderHeaderStream.GetStream().str() << "\n" << shaderBodyStream.GetStream().str();

            if (currShader == inParsedShaderProgram->mVertexShader)
                outData.mVertexShader.mSource = outStream.str();
            else
                outData.mFragmentShader.mSource = outStream.str();

            std::ofstream oFile;
            oFile.open(std::string("converted_shader_." + (currShader == inParsedShaderProgram->mVertexShader ? std::string("vs") : std::string("fs"))));
            oFile << outStream.str().c_str();
            oFile.close();
        }

        return true;
    }
}}
