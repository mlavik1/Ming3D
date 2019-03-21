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
        inStream << GetConvertedType(inVarInfo.mDatatypeInfo.mName) << " " << GetVariableIdentifierString(inVarInfo.mName);
    }

    void ShaderWriterGLSL::WriteFunctionDefinition(ShaderStream& inStream, const ShaderFunctionDefinition* inFunctionDef, bool isMainFunction)
    {
        inStream << GetConvertedType(inFunctionDef->mFunctionInfo.mReturnValueType.mName) << " ";
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
                    inStream << GetVariableIdentifierString(param.mName) << "." << member.mName << "=" << "input_" << member.mName << ";\n";
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
                inStream << "output_" << member.mName << "=" << GetVariableIdentifierString(inFunctionDef->mFunctionInfo.mParameters[1].mName) << "." << member.mName << ";\n";
                if (member.mSemantic == "SV_POSITION")
                {
                    vertexPosOutputIndex = iMember;
                }
            }
            if (mCurrentShader == mCurrentShaderProgram->mVertexShader)
            {
                if (vertexPosOutputIndex != -1)
                {
                    inStream << "gl_Position = " << GetVariableIdentifierString(inFunctionDef->mFunctionInfo.mParameters[1].mName) << "." << mCurrentShader->mOutput.mMemberVariables[vertexPosOutputIndex].mName << ";\n";
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

    void ShaderWriterGLSL::WriteStatement(ShaderStream& inStream, const ShaderStatement* inStatement, const char* inTerminator)
    {
        switch (inStatement->GetStatementType())
        {
        case EStatementType::ControlStatement:
        {
            ControlStatement* controlStatement = (ControlStatement*)inStatement;
            inStream << controlStatement->mIdentifier << "(";
            for (size_t i = 0; i < controlStatement->mExpressionStatements->mStatements.size(); i++)
            {
                bool isLast = i == controlStatement->mExpressionStatements->mStatements.size() - 1;
                WriteStatement(inStream, controlStatement->mExpressionStatements->mStatements[i], isLast ? "" : ";");
            }
            inStream << ")\n" << "{\n";
            inStream.AddIndent();
            WriteStatementBlock(inStream, controlStatement->mStatementBlock);
            inStream.RemoveIndent();
            inStream << "}";
            inTerminator = "";
            break;
        }
        case EStatementType::VariableDefinition:
        {
            VariableDefinitionStatement* varDefStatement = (VariableDefinitionStatement*)inStatement;
            inStream << GetConvertedType(varDefStatement->mVariableType);
            inStream << " " << varDefStatement->mVariableName;
            inStream << " = ";
            WriteExpression(inStream, varDefStatement->mAssignmentExpression);
            break;
        }
        case EStatementType::Expression:
        {
            ExpressionStatement* expressionStatement = (ExpressionStatement*)inStatement;
            WriteExpression(inStream, expressionStatement->mExpression);
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
            break;
        }
        default:
            LOG_ERROR() << "Unhandled statement type";
            break;
        }
        inStream << inTerminator;
    }

    void ShaderWriterGLSL::WriteStatementBlock(ShaderStream& inStream, const ShaderStatementBlock* inStatementBlock)
    {
        for (ShaderStatement* currStatement : inStatementBlock->mStatements)
        {
            WriteStatement(inStream, currStatement);
            inStream << "\n";
        }
    }

    bool ShaderWriterGLSL::WriteShader(const ParsedShaderProgram* inParsedShaderProgram, ShaderProgramDataGLSL& outData)
    {
        mCurrentShaderProgram = inParsedShaderProgram;

        std::vector<ParsedShader*> shaders;
        if (inParsedShaderProgram->mVertexShader)
            shaders.push_back(inParsedShaderProgram->mVertexShader);
        if (inParsedShaderProgram->mFragmentShader)
            shaders.push_back(inParsedShaderProgram->mFragmentShader);

        for (ShaderVariableInfo uniformInfo : inParsedShaderProgram->mShaderUniforms)
        {
            mAvailableUniforms.emplace(uniformInfo.mName);
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

            std::vector<ShaderDatatypeInfo> structDefinitions = currShader->mStructDefinitions;
            structDefinitions.insert(structDefinitions.end(), inParsedShaderProgram->mStructDefinitions.begin(), inParsedShaderProgram->mStructDefinitions.end());

            // Write structs (from shader program and shader)
            for (ShaderDatatypeInfo structInfo : structDefinitions)
            {
                shaderHeaderStream << "struct " << structInfo.mName << "\n" << "{" << "\n";

                shaderHeaderStream.AddIndent();
                for (const ShaderStructMember member : structInfo.mMemberVariables)
                {
                    shaderHeaderStream << GetConvertedType(member.mDatatype.mName) << " " << member.mName << ";\n";
                }
                shaderHeaderStream.RemoveIndent();

                shaderHeaderStream << "\n};\n";
            }
            shaderHeaderStream << "\n";

            // Write uniforms
            for (const ShaderVariableInfo uniformInfo : inParsedShaderProgram->mShaderUniforms)
            {
                if (mReferencedUniforms.find(uniformInfo.mName) != mReferencedUniforms.end())
                {
                    shaderHeaderStream << "uniform " << GetConvertedType(uniformInfo.mDatatypeInfo.mName) << " " << uniformInfo.mName << ";\n";
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
                shaderHeaderStream << "in " << GetConvertedType(inputMember.mDatatype.mName) << " " << "input_" << inputMember.mName << ";\n";
            }

            shaderHeaderStream << "\n";

            // Write shader output
            for (size_t i = 0; i < currShader->mOutput.mMemberVariables.size(); i++)
            {
                const ShaderStructMember outputMember = currShader->mOutput.mMemberVariables[i];
                shaderHeaderStream << "layout (location=" << i << ") ";
                shaderHeaderStream << "out " << outputMember.mDatatype.mName << " output_" << outputMember.mName << ";\n";
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
