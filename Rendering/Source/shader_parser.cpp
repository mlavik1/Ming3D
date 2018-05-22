#include "shader_parser.h"

#include <fstream>
#include <string>
#include <numeric>
#include "Debug/debug.h"
#include "Debug/st_assert.h"
#include "shader_tokeniser.h"

namespace Ming3D {
    namespace ShaderConverter
    {
        ShaderParser::ShaderParser()
        {
            mBuiltinStructs.emplace("vec2", ShaderStructInfo("vec2", { { "float", "x" },{ "float", "y" },{ "float", "r" },{ "float", "g" } }));
            mBuiltinStructs.emplace("vec3", ShaderStructInfo("vec3", { { "float", "x" },{ "float", "y" },{ "float", "z" },{ "float", "r" },{ "float", "g" },{ "float", "b" } }));
            mBuiltinStructs.emplace("vec4", ShaderStructInfo("vec4", { { "float", "x" },{ "float", "y" },{ "float", "z" },{ "float", "w" },{ "float", "r" },{ "float", "g" },{ "float", "b" },{ "float", "a" } }));
            mBuiltinStructs.emplace("mat4", ShaderStructInfo("mat4", {})); // TODO

            mUnaryOperatorsMap.emplace("++", OperatorInfo{ "++", 3, EOperatorAssociativity::LeftToRight });
            mUnaryOperatorsMap.emplace("--", OperatorInfo{ "--", 3, EOperatorAssociativity::LeftToRight });
            mUnaryOperatorsMap.emplace("!", OperatorInfo{ "!", 3, EOperatorAssociativity::LeftToRight });
            mUnaryOperatorsMap.emplace("+", OperatorInfo{ "+", 3, EOperatorAssociativity::LeftToRight });
            mUnaryOperatorsMap.emplace("-", OperatorInfo{ "-", 3, EOperatorAssociativity::LeftToRight });

            mBinaryOperatorsMap.emplace("*", OperatorInfo{ "*", 4, EOperatorAssociativity::LeftToRight });
            mBinaryOperatorsMap.emplace("/", OperatorInfo{ "/", 4, EOperatorAssociativity::LeftToRight });
            mBinaryOperatorsMap.emplace("+", OperatorInfo{ "+", 5, EOperatorAssociativity::LeftToRight });
            mBinaryOperatorsMap.emplace("-", OperatorInfo{ "-", 5, EOperatorAssociativity::LeftToRight });
            mBinaryOperatorsMap.emplace(">", OperatorInfo{ ">", 7, EOperatorAssociativity::LeftToRight });
            mBinaryOperatorsMap.emplace("<", OperatorInfo{ "<", 7, EOperatorAssociativity::LeftToRight });
            mBinaryOperatorsMap.emplace(">=", OperatorInfo{ ">=", 7, EOperatorAssociativity::LeftToRight });
            mBinaryOperatorsMap.emplace("<=", OperatorInfo{ "<=", 7, EOperatorAssociativity::LeftToRight });
            mBinaryOperatorsMap.emplace("==", OperatorInfo{ "==", 8, EOperatorAssociativity::LeftToRight });
            mBinaryOperatorsMap.emplace("!=", OperatorInfo{ "!=", 8, EOperatorAssociativity::LeftToRight });
            mBinaryOperatorsMap.emplace("&&", OperatorInfo{ "&&", 12, EOperatorAssociativity::LeftToRight });
            mBinaryOperatorsMap.emplace("^^", OperatorInfo{ "^^", 13, EOperatorAssociativity::LeftToRight });
            mBinaryOperatorsMap.emplace("||", OperatorInfo{ "||", 14, EOperatorAssociativity::LeftToRight });
            mBinaryOperatorsMap.emplace("=", OperatorInfo{ "=", 16, EOperatorAssociativity::LeftToRight });
        }

        void ShaderParser::OnParseError(TokenParser inTokenParser, const char* inErrorString)
        {
            // TODO: PRINT THE LINE THAT HAS THE ERROR
            LOG_ERROR() << "Parsing error, in line " << inTokenParser.GetCurrentToken().mLineNumber << ":" << inErrorString;
        }

        void ShaderParser::PushScopeStack()
        {
            mScopeStack.push(ShaderParserScope());
        }

        void ShaderParser::PopScopeStack()
        {
            for (const ShaderVariableInfo& var : mScopeStack.top().mVariables)
            {
                mVariablesInScope.erase(var.mVariableName);
            }
            mScopeStack.pop();
        }

        void ShaderParser::AddVariableToScope(const ShaderVariableInfo& inVariable)
        {
            mScopeStack.top().mVariables.push_back(inVariable);
            mVariablesInScope.emplace(inVariable.mVariableName, inVariable);
        }

        void ShaderParser::AddStructToScope(const ShaderStructInfo& inStruct)
        {
            mScopeStack.top().mStructs.push_back(inStruct);
            mStructsInScope.emplace(inStruct.mStructName, inStruct);
        }

        void ShaderParser::AddFunctionToScope(const ShaderFunctionInfo& inFunc)
        {
            mScopeStack.top().mFunctions.push_back(inFunc);
            mFunctionsInScope.emplace(inFunc.mFunctionName, inFunc);
        }

        bool ShaderParser::GetStructInfo(const char* inStructName, ShaderStructInfo& outStructInfo, ParsedShaderProgram* inShaderProgram, ParsedShader* inShader)
        {
            for (ShaderStructInfo structInfo : inShaderProgram->mStructDefinitions)
            {
                if (structInfo.mStructName == inStructName)
                {
                    outStructInfo = structInfo;
                    return true;
                }
            }
            if (inShader != nullptr)
            {
                for (ShaderStructInfo structInfo : inShader->mStructDefinitions)
                {
                    if (structInfo.mStructName == inStructName)
                    {
                        outStructInfo = structInfo;
                        return true;
                    }
                }
            }
            return false;
        }

        bool ShaderParser::IsTypeIdentifier(const char* inTokenString)
        {
            if (mBuiltinDataTypes.find(inTokenString) != mBuiltinDataTypes.end())
                return true;
            if (mBuiltinStructs.find(inTokenString) != mBuiltinStructs.end())
                return true;
            if (mStructsInScope.find(inTokenString) != mStructsInScope.end())
                return true;
            if (mFunctionsInScope.find(inTokenString) != mFunctionsInScope.end())
                return true;
            return false;
        }

        bool ShaderParser::TryGetUnaryOperator(const char* inOperator, OperatorInfo& outOperatorInfo)
        {
            auto opIter = mUnaryOperatorsMap.find(inOperator);
            if (opIter != mUnaryOperatorsMap.end())
            {
                outOperatorInfo = opIter->second;
                return true;
            }
            return false;
        }

        bool ShaderParser::TryGetBinaryOperator(const char* inOperator, OperatorInfo& outOperatorInfo)
        {
            auto opIter = mBinaryOperatorsMap.find(inOperator);
            if (opIter != mBinaryOperatorsMap.end())
            {
                outOperatorInfo = opIter->second;
                return true;
            }
            return false;
        }

        EParseResult ShaderParser::ParseIdentifierWithSuffixOperator(TokenParser& inTokenParser, ShaderExpression** outExpression)
        {
            Token identifierToken = inTokenParser.GetCurrentToken();

            inTokenParser.Advance();

            std::string strToken = inTokenParser.GetCurrentToken().mTokenString;
            if (strToken == "(")
            {
                // Function call
                FunctionCallExpression* funcExpression = new FunctionCallExpression();
                funcExpression->mIdentifier = identifierToken;

                inTokenParser.Advance();

                while (inTokenParser.GetCurrentToken().mTokenString != ")")
                {
                    ShaderExpression* paramExpr = nullptr;
                    EParseResult paramParseResult = ParseExpression(inTokenParser, mDefaultOuterOperatorInfo, &paramExpr);

                    if (paramParseResult == EParseResult::Error)
                    {
                        OnParseError(inTokenParser, "Invalid subexpression");
                        return EParseResult::Error;
                    }
                    funcExpression->mParameterExpressions.push_back(paramExpr);
                    if (inTokenParser.GetCurrentToken().mTokenString == ",")
                        inTokenParser.Advance();
                }
                if (inTokenParser.GetCurrentToken().mTokenString != ")")
                {
                    OnParseError(inTokenParser, "Missing ) after subexpression");
                    return EParseResult::Error;
                }
                inTokenParser.Advance();

                *outExpression = funcExpression;
                return EParseResult::Parsed;

                // TODO: constructor?
                // WE CAN STORE CONSTRUCTORS AS FUNCTION CALLS
                // THIS WAY WE CAN GENERATE FAKE CONSTRUCTORS (functions that return a type) FOR USER STRUCTS
            }
            else
            {
                ShaderExpression* identifierExpression = new VariableAccessExpression();
                ((VariableAccessExpression*)identifierExpression)->mIdentifier = identifierToken;
                // TODO: Check that the variable is defined
                while (true)
                {
                    if (inTokenParser.GetCurrentToken().mTokenString == ".")
                    {
                        // TODO: Check that the member is defined in the identifier

                        inTokenParser.Advance();

                        Token memberToken = inTokenParser.GetCurrentToken();
                        VariableAccessExpression* memberExpr = new VariableAccessExpression();
                        memberExpr->mIdentifier = memberToken;
                        memberExpr->mOuterExpression = identifierExpression;
                        identifierExpression = memberExpr;
                        inTokenParser.Advance();
                    }
                    else if (inTokenParser.GetCurrentToken().mTokenString == "[")
                    {
                        // TODO: Implement support for arrays
                        OnParseError(inTokenParser, "No support for arrays yet");
                        return EParseResult::Error; // TODO
                    }
                    else
                    {
                        break; // no suffix operator
                    }
                }
                *outExpression = identifierExpression;
                return EParseResult::Parsed;
            }
        }

        EParseResult ShaderParser::ParseAtom(TokenParser& inTokenParser, ShaderExpression** outExpression)
        {
            // 1. (try) Parse unary operator
            // 2. Switch on token:
            //    FloatLiteral, IntegerLiteral, BoolLiteral, Identifier, LeftParenthesis, function call, constructor

            Token currToken = inTokenParser.GetCurrentToken();

            OperatorInfo prefixOperator;
            bool hasPrefixOperator = false;

            // Parse unary prefix operator
            if (currToken.mTokenType == ETokenType::Operator)
            {
                if (TryGetUnaryOperator(currToken.mTokenString.c_str(), prefixOperator))
                {
                    hasPrefixOperator = true;
                    inTokenParser.Advance();
                    currToken = inTokenParser.GetCurrentToken();
                }
            }

            ShaderExpression* atomExpression = nullptr;

            switch (currToken.mTokenType)
            {
            case ETokenType::BooleanLiteral:
            case ETokenType::FloatLiteral:
            case ETokenType::IntegerLiteral:
            {
                atomExpression = new LiteralExpression();
                ((LiteralExpression*)atomExpression)->mToken = currToken;
                inTokenParser.Advance();
                break;
            }
            case ETokenType::Operator:
            {
                if (currToken.mTokenString == "(")
                {
                    inTokenParser.Advance();
                    EParseResult res = ParseExpression(inTokenParser, mDefaultOuterOperatorInfo, &atomExpression);
                    if (res != EParseResult::Parsed)
                    {
                        OnParseError(inTokenParser, "Failed to parse atom. Invalid expression after (");
                        return EParseResult::Error;
                    }
                    inTokenParser.Advance();
                }
                else
                {
                    return EParseResult::NotParsed;
                }
                break;
            }
            case ETokenType::Identifier:
            {
                EParseResult identifierParseResult = ParseIdentifierWithSuffixOperator(inTokenParser, &atomExpression);
                if (identifierParseResult != EParseResult::Parsed)
                {
                    return EParseResult::Error;
                }
                break;
            }
            default:
            {
                OnParseError(inTokenParser, "Unhandled token atom type");
                return EParseResult::Error;
            }
            }

            if (hasPrefixOperator)
            {
                UnaryOperationExpression* unaryExpr = new UnaryOperationExpression();
                unaryExpr->mOperator = prefixOperator.mOperator;
                unaryExpr->mOperand = atomExpression;
                *outExpression = unaryExpr;
            }
            else
            {
                *outExpression = atomExpression;
            }

            return EParseResult::Parsed;
        }

        EParseResult ShaderParser::ParseExpression(TokenParser& inTokenParser, const OperatorInfo& inOperator, ShaderExpression** outExpression)
        {
            EParseResult atomParseResult = ParseAtom(inTokenParser, outExpression);
            if (atomParseResult != EParseResult::Parsed)
            {
                return atomParseResult;
            }
            while (inTokenParser.GetCurrentToken().mTokenString != ";" && inTokenParser.GetCurrentToken().mTokenString != ")" && inTokenParser.GetCurrentToken().mTokenString != ",") // TODO
            {
                // Parse operator
                Token operatorToken = inTokenParser.GetCurrentToken();
                OperatorInfo operatorInfo;
                if (TryGetBinaryOperator(operatorToken.mTokenString.c_str(), operatorInfo))
                {
                    if (operatorInfo.mPrecedence < inOperator.mPrecedence) // TODO: Operator associativity - need that for ternary operator
                    {
                        inTokenParser.Advance();
                        ShaderExpression* rightExpr = nullptr;
                        EParseResult subExprParseResult = ParseExpression(inTokenParser, operatorInfo, &rightExpr);
                        if (subExprParseResult == EParseResult::Parsed)
                        {
                            BinaryOperationExpression* opExpr = new BinaryOperationExpression();
                            opExpr->mOperator = operatorInfo.mOperator;
                            opExpr->mLeftOperand = *outExpression;
                            opExpr->mRightOperand = rightExpr;
                            *outExpression = opExpr;
                        }
                        else
                        {
                            return EParseResult::Error;
                        }
                    }
                    else
                    {
                        break;
                    }
                }
                else
                {
                    OnParseError(inTokenParser, "Expected binary operator");
                    return EParseResult::Error;
                }
            }

            return EParseResult::Parsed;
        }

        EParseResult ShaderParser::ParseStructBody(TokenParser& inTokenParser, ShaderStructInfo* outStructInfo)
        {
            while (inTokenParser.GetCurrentToken().mTokenString != "}")
            {
                const Token typeToken = inTokenParser.GetCurrentToken();
                if (IsTypeIdentifier(typeToken.mTokenString.c_str()))
                {
                    inTokenParser.Advance();
                    const Token identifierToken = inTokenParser.GetCurrentToken();
                    inTokenParser.Advance();
                    
                    std::string semanticName;

                    if (inTokenParser.GetCurrentToken().mTokenString == ":")
                    {
                        inTokenParser.Advance();
                        semanticName = inTokenParser.GetCurrentToken().mTokenString;
                        inTokenParser.Advance();
                    }

                    if (inTokenParser.GetCurrentToken().mTokenString != ";")
                    {
                        OnParseError(inTokenParser, "Missing semicolon after struct member definition");
                        return EParseResult::Error;
                    }
                    ShaderStructMember structMember;
                    structMember.mVariableType = typeToken.mTokenString;
                    structMember.mVariableName = identifierToken.mTokenString;
                    structMember.mSemantic = semanticName;
                    outStructInfo->mMemberVariables.push_back(structMember);
                    inTokenParser.Advance();
                }
                else
                {
                    OnParseError(inTokenParser, "Invalid type identifier in struct body");
                    return EParseResult::Error;
                }
            }

            return EParseResult::Parsed;
        }

        EParseResult ShaderParser::ParseFunctionDefinition(TokenParser& inTokenParser, ShaderFunctionDefinition** outDefinition)
        {
            ShaderFunctionInfo funcInfo;
            funcInfo.mReturnValueType = inTokenParser.GetCurrentToken().mTokenString;
            inTokenParser.Advance();
            funcInfo.mFunctionName = inTokenParser.GetCurrentToken().mTokenString; // TODO: check if valid identifier name
            inTokenParser.Advance();

            if (inTokenParser.GetCurrentToken().mTokenString != "(")
            {
                OnParseError(inTokenParser, "Missing parenthesis after function name");
                return EParseResult::Error;
            }
            inTokenParser.Advance();

            PushScopeStack();

            // Parse function parameters
            while (inTokenParser.GetCurrentToken().mTokenString != ")")
            {
                // TODO: check shader input types as well (if main)
                if (0)//if (!IsTypeIdentifier(inTokenParser.GetCurrentToken().mTokenString.c_str()))
                {
                    OnParseError(inTokenParser, "Invalid function parameter type");
                    return EParseResult::Error;
                }
                ShaderVariableInfo funcParam;
                funcParam.mVariableType = inTokenParser.GetCurrentToken().mTokenString;
                inTokenParser.Advance();
                funcParam.mVariableName = inTokenParser.GetCurrentToken().mTokenString;
                funcInfo.mParameters.push_back(funcParam);
                AddVariableToScope(funcParam);
                inTokenParser.Advance();
                if (inTokenParser.GetCurrentToken().mTokenString == ",")
                    inTokenParser.Advance();
            }
            inTokenParser.Advance();

            if (inTokenParser.GetCurrentToken().mTokenString != "{")
            {
                OnParseError(inTokenParser, "Invalid function definition body. Missing {");
                return EParseResult::Error;
            }
            inTokenParser.Advance();
            ShaderFunctionDefinition* funcDefinition = new ShaderFunctionDefinition();
            funcDefinition->mFunctionInfo = funcInfo;
            EParseResult statementParseRes = ParseStatementBlock(inTokenParser, '}', &funcDefinition->mStatementBlock);

            *outDefinition = funcDefinition;

            PopScopeStack();

            return statementParseRes;
        }

        EParseResult ShaderParser::ParseStatement(TokenParser& inTokenParser, ShaderStatement** outStatement)
        {
            const std::string& tokenString = inTokenParser.GetCurrentToken().mTokenString;
            if (IsTypeIdentifier(tokenString.c_str()))
            {
                VariableDefinitionStatement* varDefStatement = new VariableDefinitionStatement();

                varDefStatement->mVariableType = inTokenParser.GetCurrentToken().mTokenString;

                inTokenParser.Advance();

                varDefStatement->mVariableName = inTokenParser.GetCurrentToken().mTokenString; // TODO: check if valid identifier name

                inTokenParser.Advance();

                if (inTokenParser.GetCurrentToken().mTokenString == "=")
                {
                    ShaderExpression* assignmentExpression = nullptr;
                    EParseResult exprParseResult = ParseExpression(inTokenParser, mDefaultOuterOperatorInfo, &varDefStatement->mAssignmentExpression);
                    if (exprParseResult != EParseResult::Parsed)
                    {
                        OnParseError(inTokenParser, "Invalid assignment expression in variable definition expression");
                        delete varDefStatement;
                        return EParseResult::Error;
                    }
                }

                if (inTokenParser.GetCurrentToken().mTokenString != ";")
                {
                    OnParseError(inTokenParser, "Missing semicolon");
                    delete varDefStatement;
                    return EParseResult::Error;
                }

                *outStatement = varDefStatement;
            }
            else if (tokenString == "return")
            {
                ReturnStatement* retStatement = new ReturnStatement();

                inTokenParser.Advance();

                EParseResult exprParseResult = EParseResult::NotParsed;
                if (inTokenParser.GetCurrentToken().mTokenString != ";")
                {
                    exprParseResult = ParseExpression(inTokenParser, mDefaultOuterOperatorInfo, &retStatement->mReturnValueExpression);
                }

                if (exprParseResult == EParseResult::Error || inTokenParser.GetCurrentToken().mTokenString != ";")
                {
                    OnParseError(inTokenParser, "Invalid return statement");
                    delete retStatement;
                    return EParseResult::Error;
                }
                inTokenParser.Advance();
                *outStatement = retStatement;
            }
            else
            {
                ExpressionStatement* exprStatement = new ExpressionStatement();
                EParseResult exprParseResult = ParseExpression(inTokenParser, mDefaultOuterOperatorInfo, &exprStatement->mExpression);
                if (exprParseResult != EParseResult::Parsed)
                {
                    OnParseError(inTokenParser, "Invalid statement expression");
                    delete exprStatement;
                    return EParseResult::Error;
                }
                *outStatement = exprStatement;
                inTokenParser.Advance();
            }
            return EParseResult::Parsed;
        }

        EParseResult ShaderParser::ParseStatementBlock(TokenParser& inTokenParser, char inTerminator, ShaderStatementBlock** outStatementBlock)
        {
            *outStatementBlock = new ShaderStatementBlock();
            while (inTokenParser.GetCurrentToken().mTokenString[0] != inTerminator)
            {
                ShaderStatement* statement = nullptr;
                EParseResult statementParseResult = ParseStatement(inTokenParser, &statement);
                if (statementParseResult == EParseResult::Parsed)
                {
                    (*outStatementBlock)->mStatements.push_back(statement);
                }
                else
                {
                    return statementParseResult;
                }
            }
            return EParseResult::Parsed;
        }

        ParsedShaderProgram* ShaderParser::ParseShaderProgram(const char* inShaderProgramPath)
        {

            ParsedShaderProgram* parsedShaderProgram = new ParsedShaderProgram();
            std::ifstream shaderFile(inShaderProgramPath);
            std::string shaderString((std::istreambuf_iterator<char>(shaderFile)), std::istreambuf_iterator<char>());

            TokenParser tokenParser(shaderString.c_str());

            std::vector<ParsedShader*> parsedShaders;
            ParsedShader* currentParsingShader = nullptr;

            bool failed = false;

            while (tokenParser.HasMoreTokens())
            {
                const Token currToken = tokenParser.GetCurrentToken();

                if (currToken.mTokenString == "ShaderUniforms")
                {
                    tokenParser.Advance();
                    tokenParser.Advance(); // TODO: check
                    ShaderStructInfo inputStruct;
                    EParseResult inputParseResult = ParseStructBody(tokenParser, &inputStruct);
                    if (inputParseResult == EParseResult::Parsed)
                    {
                        for (ShaderStructMember member : inputStruct.mMemberVariables)
                        {
                            ShaderUniformInfo uniformInfo;
                            uniformInfo.mUniformName = member.mVariableName;
                            uniformInfo.mUniformType = member.mVariableType;
                            parsedShaderProgram->mShaderUniforms.push_back(uniformInfo);
                        }
                        tokenParser.Advance();
                    }
                    else if (inputParseResult == EParseResult::Error)
                    {
                        failed = true;
                        break;
                    }
                }
                else if (currToken.mTokenString == "ShaderTextures")
                {
                    tokenParser.Advance();
                    tokenParser.Advance();
                    ShaderStructInfo inputStruct;
                    EParseResult inputParseResult = ParseStructBody(tokenParser, &inputStruct);
                    if (inputParseResult == EParseResult::Parsed)
                    {
                        for (ShaderStructMember member : inputStruct.mMemberVariables)
                        {
                            ShaderTextureInfo textureInfo;
                            textureInfo.mTextureName = member.mVariableName;
                            textureInfo.mTextureType = member.mVariableType; // TODO: use enum
                            parsedShaderProgram->mShaderTextures.push_back(textureInfo);
                        }
                        tokenParser.Advance();
                    }
                    else if (inputParseResult == EParseResult::Error)
                    {
                        failed = true;
                        break;
                    }
                }
                else if (currToken.mTokenString == "struct")
                {
                    tokenParser.Advance();
                    ShaderStructInfo structInfo;
                    structInfo.mStructName = tokenParser.GetCurrentToken().mTokenString; // TODO: Check valid identifier name
                    tokenParser.Advance(); // TODO: check that current token is "{"
                    tokenParser.Advance();
                    EParseResult inputParseResult = ParseStructBody(tokenParser, &structInfo);
                    if (inputParseResult == EParseResult::Parsed)
                    {
                        if (currentParsingShader != nullptr)
                            currentParsingShader->mStructDefinitions.push_back(structInfo);
                        else
                            parsedShaderProgram->mStructDefinitions.push_back(structInfo);
                    }
                    else if (inputParseResult == EParseResult::Error)
                    {
                        failed = true;
                        break;
                    }
                    tokenParser.Advance();
                }
                else
                {
                    // Parse shader program
                    if (currentParsingShader == nullptr)
                    {
                        if (currToken.mTokenString == "shader")
                        {
                            tokenParser.Advance();
                            currentParsingShader = new ParsedShader();
                            if (tokenParser.GetCurrentToken().mTokenString == "VertexShader")
                            {
                                parsedShaderProgram->mVertexShader = currentParsingShader;
                            }
                            else
                            {
                                parsedShaderProgram->mFragmentShader = currentParsingShader;
                            }
                            tokenParser.Advance();
                            tokenParser.Advance(); // TODO: look for "{"
                            PushScopeStack();
                        }
                    }
                    // Parse shader
                    else
                    {
                        if (currToken.mTokenString == "}")
                        {
                            parsedShaders.push_back(currentParsingShader);
                            currentParsingShader = nullptr; // end of shader
                            PopScopeStack();
                            tokenParser.Advance();
                        }
                        else if (IsTypeIdentifier(currToken.mTokenString.c_str()))
                        {
                            ShaderFunctionDefinition* functionDef = nullptr;
                            EParseResult funcDefparseResult = ParseFunctionDefinition(tokenParser, &functionDef);

                            if (funcDefparseResult == EParseResult::Parsed)
                            {
                                if (functionDef->mFunctionInfo.mFunctionName == "main")
                                {
                                    currentParsingShader->mMainFunction = functionDef;
                                    if (functionDef->mFunctionInfo.mParameters.size() > 0)
                                    {
                                        if (!GetStructInfo(functionDef->mFunctionInfo.mParameters[0].mVariableType.c_str(), currentParsingShader->mInput, parsedShaderProgram, currentParsingShader))
                                        {
                                            OnParseError(tokenParser, "Invalid shader input parameter in main");
                                            failed = true;
                                            break;
                                        }
                                    }
                                    if (functionDef->mFunctionInfo.mParameters.size() > 1)
                                    {
                                        if (!GetStructInfo(functionDef->mFunctionInfo.mParameters[1].mVariableType.c_str(), currentParsingShader->mOutput, parsedShaderProgram, currentParsingShader))
                                        {
                                            OnParseError(tokenParser, "Invalid shader output parameter in main");
                                            failed = true;
                                            break;
                                        }
                                    }
                                }
                                else
                                {
                                    if (currentParsingShader != nullptr)
                                        currentParsingShader->mFunctionDefinitions.push_back(functionDef);
                                    else
                                        parsedShaderProgram->mFunctionDefinitions.push_back(functionDef);
                                }
                                tokenParser.Advance();
                            }
                            else
                            {
                                OnParseError(tokenParser, "Invalid function definition statement block");
                                failed = true;
                                break;
                            }
                        }
                        else
                        {
                            OnParseError(tokenParser, "Invalid token");
                            failed = true;
                            break;
                        }
                    }
                }
            }

            if (failed)
            {
                LOG_ERROR() << "Failed parsing shader: " << inShaderProgramPath;
                delete parsedShaderProgram;
                return nullptr;
            }

            // TODO: Clear up the following code
            // Should iterate through all parsed shaders instead

            if (parsedShaderProgram->mVertexShader->mMainFunction->mFunctionInfo.mParameters.size() != 2)
            {
                LOG_ERROR() << "Invalid number of parameters in vertex shader main function. Expected 2 parameters.";
                delete parsedShaderProgram;
                return nullptr;
            }
            else if (parsedShaderProgram->mFragmentShader->mMainFunction->mFunctionInfo.mParameters.size() != 1)
            {
                LOG_ERROR() << "Invalid number of parameters in fragment shader main function. Expected 1 parameter.";
                delete parsedShaderProgram;
                return nullptr;
            }

            if (parsedShaderProgram->mVertexShader->mOutput.mStructName != parsedShaderProgram->mFragmentShader->mInput.mStructName)
            {
                LOG_ERROR() << "Input and output of shaders must match";
                delete parsedShaderProgram;
                return nullptr;
            }

            // TODO: Print any errors from here + PRINT THE LINE THAT HAS THE ERROR!

            LOG_INFO() << "Successfully parsed shader program: " << inShaderProgramPath;

            return parsedShaderProgram;
        }
    }
}
