#include "shader_parser.h"

#include <fstream>
#include <string>
#include <numeric>
#include "Debug/debug.h"
#include "Debug/st_assert.h"
#include "shader_tokeniser.h"
#include "shader_preprocessor.h"
#include "Debug/st_assert.h"

#define MING3D_BreakOnShaderParserError

namespace Ming3D::Rendering
{
    ShaderParser::ShaderParser()
    {
        mBuiltinDatatypes.emplace("float", ShaderDatatypeInfo(EShaderDatatype::Float, "float"));
        mBuiltinDatatypes.emplace("int", ShaderDatatypeInfo(EShaderDatatype::Int, "int"));
        mBuiltinDatatypes.emplace("bool", ShaderDatatypeInfo(EShaderDatatype::Bool, "bool"));
        mBuiltinDatatypes.emplace("void", ShaderDatatypeInfo(EShaderDatatype::Void, "void"));
        mBuiltinDatatypes.emplace("Texture2D", ShaderDatatypeInfo(EShaderDatatype::Texture2D, "Texture2D"));

        mBuiltinDatatypes.emplace("vec2", ShaderDatatypeInfo(EShaderDatatype::Vec2, "vec2", { ShaderStructMember(mBuiltinDatatypes["float"], "x"), ShaderStructMember(mBuiltinDatatypes["float"], "y"), ShaderStructMember(mBuiltinDatatypes["float"], "r"), ShaderStructMember(mBuiltinDatatypes["float"], "g") }));
        mBuiltinDatatypes.emplace("vec3", ShaderDatatypeInfo(EShaderDatatype::Vec3, "vec3", { ShaderStructMember(mBuiltinDatatypes["float"], "x"), ShaderStructMember(mBuiltinDatatypes["float"], "y"), ShaderStructMember(mBuiltinDatatypes["float"], "z"), ShaderStructMember(mBuiltinDatatypes["float"], "r"), ShaderStructMember(mBuiltinDatatypes["float"], "g"), ShaderStructMember(mBuiltinDatatypes["float"], "b") }));
        mBuiltinDatatypes.emplace("vec4", ShaderDatatypeInfo(EShaderDatatype::Vec4, "vec4", { ShaderStructMember(mBuiltinDatatypes["float"], "x"), ShaderStructMember(mBuiltinDatatypes["float"], "y"), ShaderStructMember(mBuiltinDatatypes["float"], "z"), ShaderStructMember(mBuiltinDatatypes["float"], "w"), ShaderStructMember(mBuiltinDatatypes["float"], "r"), ShaderStructMember(mBuiltinDatatypes["float"], "g"), ShaderStructMember(mBuiltinDatatypes["float"], "b"), ShaderStructMember(mBuiltinDatatypes["float"], "a") }));
        mBuiltinDatatypes.emplace("mat4", ShaderDatatypeInfo(EShaderDatatype::Mat4x4, "mat4", { }));

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
        mBinaryOperatorsMap.emplace("+=", OperatorInfo{ "+=", 16, EOperatorAssociativity::LeftToRight });
        mBinaryOperatorsMap.emplace("-=", OperatorInfo{ "-=", 16, EOperatorAssociativity::LeftToRight });
    }

    void ShaderParser::OnParseError(TokenParser inTokenParser, const std::string& inErrorString)
    {
        // TODO: PRINT THE LINE THAT HAS THE ERROR
        LOG_ERROR() << "Parsing error, in line " << inTokenParser.GetCurrentToken().mLineNumber << ":" << inErrorString;
#ifdef MING3D_BreakOnShaderParserError
        __Assert(false);
#endif
    }

    void ShaderParser::PushScopeStack()
    {
        mScopeStack.push(ShaderParserScope());
    }

    void ShaderParser::PopScopeStack()
    {
        for (const ShaderVariableInfo& var : mScopeStack.top().mVariables)
        {
            mVariablesInScope.erase(var.mName);
        }
        mScopeStack.pop();
    }

    void ShaderParser::AddVariableToScope(const ShaderVariableInfo& inVariable)
    {
        mScopeStack.top().mVariables.push_back(inVariable);
        mVariablesInScope.emplace(inVariable.mName, inVariable);
    }

    void ShaderParser::AddFunctionToScope(const ShaderFunctionInfo& inFunc)
    {
        mScopeStack.top().mFunctions.push_back(inFunc);
        mFunctionsInScope.emplace(inFunc.mFunctionName, inFunc);
    }

    bool ShaderParser::GetTypeInfo(const std::string& inName, ShaderDatatypeInfo& outTypeInfo)
    {
        if (mBuiltinDatatypes.find(inName) != mBuiltinDatatypes.end())
        {
            outTypeInfo = mBuiltinDatatypes[inName];
            return true;
        }
        if (mCurrentProgramStructDefs.find(inName) != mCurrentProgramStructDefs.end())
        {
            outTypeInfo = mCurrentProgramStructDefs[inName];
            return true;
        }
        if (mCurrentShaderStructDefs.find(inName) != mCurrentShaderStructDefs.end())
        {
            outTypeInfo = mCurrentShaderStructDefs[inName];
            return true;
        }

        return false;
    }

    bool ShaderParser::IsTypeIdentifier(const char* inTokenString)
    {
        if (mBuiltinDatatypes.find(inTokenString) != mBuiltinDatatypes.end())
            return true;
        if (mCurrentProgramStructDefs.find(inTokenString) != mCurrentProgramStructDefs.end())
            return true;
        if (mCurrentShaderStructDefs.find(inTokenString) != mCurrentShaderStructDefs.end())
            return true;
        if (mFunctionsInScope.find(inTokenString) != mFunctionsInScope.end())
            return true;
        return false;
    }

    bool ShaderParser::IsControlStatementIdentifier(const char* inTokenString)
    {
        if (mControlStatementIdentifiers.find(inTokenString) != mControlStatementIdentifiers.end())
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

    EParseResult ShaderParser::ParseStructBody(TokenParser& inTokenParser, ShaderDatatypeInfo* outStructInfo)
    {
        while (inTokenParser.GetCurrentToken().mTokenString != "}")
        {
            const Token typeToken = inTokenParser.GetCurrentToken();
            if (IsTypeIdentifier(typeToken.mTokenString.c_str()))
            {
                inTokenParser.Advance();
                const Token identifierToken = inTokenParser.GetCurrentToken();
                inTokenParser.Advance();
                
                if (identifierToken.mTokenString == "")
                {
                    OnParseError(inTokenParser, "Missing identifier for struct member");
                    return EParseResult::Error;
                }

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
                    
                ShaderDatatypeInfo typeInfo;
                if (!GetTypeInfo(typeToken.mTokenString, typeInfo))
                {
                    OnParseError(inTokenParser, "Invalid datatype");
                    return EParseResult::Error;
                }

                ShaderStructMember structMember;
                structMember.mDatatype = typeInfo;
                structMember.mName = identifierToken.mTokenString;
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
        std::string strRetType = inTokenParser.GetCurrentToken().mTokenString;
        ShaderDatatypeInfo retType;
        if (!GetTypeInfo(strRetType, retType))
        {
            OnParseError(inTokenParser, std::string("Invalid return type: ") + strRetType);
            return EParseResult::Error;
        }
        funcInfo.mReturnValueType = retType;
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
            std::string typeString = inTokenParser.GetCurrentToken().mTokenString;
            ShaderDatatypeInfo typeInfo;
            if (!GetTypeInfo(typeString, typeInfo))
            {
                OnParseError(inTokenParser, "Invalid function parameter type");
                return EParseResult::Error;
            }
            ShaderVariableInfo funcParam;
            funcParam.mDatatypeInfo = typeInfo;
            inTokenParser.Advance();
            funcParam.mName = inTokenParser.GetCurrentToken().mTokenString;
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
        EParseResult statementParseRes = ParseStatementBlock(inTokenParser, TerminatorType::CurlyBrackets, &funcDefinition->mStatementBlock);

        *outDefinition = funcDefinition;

        PopScopeStack();

        return statementParseRes;
    }

    EParseResult ShaderParser::ParseStatement(TokenParser& inTokenParser, ShaderStatement** outStatement)
    {
        const std::string& tokenString = inTokenParser.GetCurrentToken().mTokenString;
        if (IsControlStatementIdentifier(tokenString.c_str())) // TODO: put in separate function
        {
            std::string controlStatementIdentifier = tokenString;
            inTokenParser.Advance();
            if (controlStatementIdentifier == "else" && inTokenParser.GetCurrentToken().mTokenString == "if")
            {
                controlStatementIdentifier = "else if";
                inTokenParser.Advance();
            }
            if (inTokenParser.GetCurrentToken().mTokenString != "(")
            {
                OnParseError(inTokenParser, "Expected ( after control statement identifier");
                return EParseResult::Error;
            }
            inTokenParser.Advance();
            ShaderStatementBlock* exprStatements = nullptr;
            EParseResult exprParseResult = ParseStatementBlock(inTokenParser, TerminatorType::Parenthesis, &exprStatements);
            if (exprParseResult != EParseResult::Parsed)
                return exprParseResult;
            if (inTokenParser.GetCurrentToken().mTokenString != ")")
            {
                delete exprStatements;
                OnParseError(inTokenParser, "Expected ( after control statement identifier");
                return EParseResult::Error;
            }
            inTokenParser.Advance();
            ShaderStatementBlock* statementBlock = nullptr;
            EParseResult statementParseResult;
            if (inTokenParser.GetCurrentToken().mTokenString == "{")
            {
                inTokenParser.Advance();
                statementParseResult = ParseStatementBlock(inTokenParser, TerminatorType::CurlyBrackets, &statementBlock);
            }
            else
            {
                statementParseResult = ParseStatementBlock(inTokenParser, TerminatorType::LineBreak, &statementBlock);
            }
            if (statementParseResult == EParseResult::Error)
            {
                delete exprStatements;
                return statementParseResult;
            }
            ControlStatement* controlStatement = new ControlStatement();
            controlStatement->mIdentifier = controlStatementIdentifier;
            controlStatement->mExpressionStatements = exprStatements;
            controlStatement->mStatementBlock = statementBlock;
            *outStatement = controlStatement;
            return EParseResult::Parsed;
        }
        else if (IsTypeIdentifier(tokenString.c_str()))
        {
            VariableDefinitionStatement* varDefStatement = new VariableDefinitionStatement();

            varDefStatement->mVariableType = inTokenParser.GetCurrentToken().mTokenString;

            inTokenParser.Advance();

            varDefStatement->mVariableName = inTokenParser.GetCurrentToken().mTokenString; // TODO: check if valid identifier name

            inTokenParser.Advance();

            if (inTokenParser.GetCurrentToken().mTokenString == "=")
            {
                inTokenParser.Advance();
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
        }
        return EParseResult::Parsed;
    }

    EParseResult ShaderParser::ParseStatementBlock(TokenParser& inTokenParser, TerminatorType inTerminator, ShaderStatementBlock** outStatementBlock)
    {
        *outStatementBlock = new ShaderStatementBlock();

        char terminatorChar = inTerminator == TerminatorType::LineBreak ? '\n' : (inTerminator == TerminatorType::CurlyBrackets ? '}' : ')');

        while (inTokenParser.GetCurrentToken().mTokenString[0] != terminatorChar)
        {
            ShaderStatement* statement = nullptr;
            EParseResult statementParseResult = ParseStatement(inTokenParser, &statement);
            if (statementParseResult == EParseResult::Parsed)
            {
                (*outStatementBlock)->mStatements.push_back(statement);
                if (!((inTokenParser.GetCurrentToken().mTokenString == ")") && (terminatorChar == ')')))
                    inTokenParser.Advance();
            }
            else
            {
                return statementParseResult;
            }
        }
        return EParseResult::Parsed;
    }

    ParsedShaderProgram* ShaderParser::ParseShaderProgram(const ShaderParserParams& inParams)
    {
        mParams = inParams;

        ParsedShaderProgram* parsedShaderProgram = new ParsedShaderProgram();
        parsedShaderProgram->mProgramPath = inParams.mShaderProgramPath;
        std::ifstream shaderFile(inParams.mShaderProgramPath);
        std::string shaderString((std::istreambuf_iterator<char>(shaderFile)), std::istreambuf_iterator<char>());

        if (shaderString == "")
        {
            LOG_ERROR() << "Failed to read shader: " << inParams.mShaderProgramPath;
            __AssertComment(false, "Faield to read shader");
            delete parsedShaderProgram;
            return nullptr;
        }

        TokenParser tokenParser(shaderString.c_str());

        ShaderPreprocessor preprocessor(tokenParser);
        for (auto preprocdef : mParams.mPreprocessorDefinitions)
            preprocessor.AddDefinition(preprocdef.first, preprocdef.second);
        preprocessor.PreprocessShader();

        std::vector<ParsedShader*> parsedShaders;
        ParsedShader* currentParsingShader = nullptr;

        bool failed = false;

        while (tokenParser.HasMoreTokens())
        {
            const Token currToken = tokenParser.GetCurrentToken();

            if (currToken.mTokenString == "cbuffer")
            {
                tokenParser.Advance();
                std::string cbName = tokenParser.GetCurrentToken().mTokenString;
                tokenParser.Advance();
                __AssertComment(tokenParser.GetCurrentToken().mTokenString == "{", "Missing {"); // TODO: don't assert
                tokenParser.Advance();
                ShaderDatatypeInfo inputStruct;
                EParseResult inputParseResult = ParseStructBody(tokenParser, &inputStruct);
                if (inputParseResult == EParseResult::Parsed)
                {
                    ConstantBufferInfo cbuffer;
                    cbuffer.mName = cbName;
                    for (ShaderStructMember member : inputStruct.mMemberVariables)
                    {
                        ShaderVariableInfo uniformInfo;
                        uniformInfo.mName = member.mName;
                        uniformInfo.mDatatypeInfo = member.mDatatype;
                        cbuffer.mShaderUniforms.push_back(uniformInfo);
                    }
                    parsedShaderProgram->mConstantBufferInfos.push_back(cbuffer);
                    tokenParser.Advance();
                }
                else if (inputParseResult == EParseResult::Error)
                {
                    failed = true;
                    break;
                }
            }
            else if (currToken.mTokenString == "uniform")
            {
                tokenParser.Advance();
                std::string uniformType = tokenParser.GetCurrentToken().mTokenString;
                tokenParser.Advance();
                std::string uniformName = tokenParser.GetCurrentToken().mTokenString;
                tokenParser.Advance();
                tokenParser.Advance();


                ShaderDatatypeInfo typeInfo;
                if (!GetTypeInfo(uniformType, typeInfo))
                {
                    OnParseError(tokenParser, "Invalid funiform type");
                    failed = true;
                    break;
                }
                ShaderVariableInfo uniformInfo;
                uniformInfo.mDatatypeInfo = typeInfo;
                uniformInfo.mName = uniformName;
                uniformInfo.mDatatypeInfo = typeInfo;

                parsedShaderProgram->mUniforms.push_back(uniformInfo);
            }
            else if (currToken.mTokenString == "Texture2D")
            {
                tokenParser.Advance();
                std::string textureName = tokenParser.GetCurrentToken().mTokenString;
                tokenParser.Advance();
                tokenParser.Advance();

                ShaderTextureInfo textureInfo;
                textureInfo.mTextureName = textureName;
                textureInfo.mTextureType = "Texture2D"; // TODO: use enum
                parsedShaderProgram->mShaderTextures.push_back(textureInfo);
            }
            else if (currToken.mTokenString == "struct")
            {
                tokenParser.Advance();
                ShaderDatatypeInfo structInfo;
                structInfo.mName = tokenParser.GetCurrentToken().mTokenString; // TODO: Check valid identifier name
                tokenParser.Advance(); // TODO: check that current token is "{"
                tokenParser.Advance();
                EParseResult inputParseResult = ParseStructBody(tokenParser, &structInfo);
                if (inputParseResult == EParseResult::Parsed)
                {
                    if (currentParsingShader != nullptr)
                        mCurrentShaderStructDefs.emplace(structInfo.mName, structInfo);
                    else
                        mCurrentProgramStructDefs.emplace(structInfo.mName, structInfo);
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
                        for (auto& structDef : mCurrentShaderStructDefs)
                            currentParsingShader->mStructDefinitions.push_back(structDef.second);
                        mCurrentShaderStructDefs.clear();

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
                                    currentParsingShader->mInput = functionDef->mFunctionInfo.mParameters[0].mDatatypeInfo;
                                }
                                if (functionDef->mFunctionInfo.mParameters.size() > 1)
                                {
                                    currentParsingShader->mOutput = functionDef->mFunctionInfo.mParameters[1].mDatatypeInfo;
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
            LOG_ERROR() << "Failed parsing shader: " << inParams.mShaderProgramPath;
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

        if (parsedShaderProgram->mVertexShader->mOutput.mDatatype != parsedShaderProgram->mFragmentShader->mInput.mDatatype)
        {
            LOG_ERROR() << "Input and output of shaders must match";
            delete parsedShaderProgram;
            return nullptr;
        }

        for (auto& structDef : mCurrentProgramStructDefs)
            parsedShaderProgram->mStructDefinitions.push_back(structDef.second);
        mCurrentProgramStructDefs.clear();

        // TODO: Print any errors from here + PRINT THE LINE THAT HAS THE ERROR!

        LOG_INFO() << "Successfully parsed shader program: " << inParams.mShaderProgramPath;

        return parsedShaderProgram;
    }
}
