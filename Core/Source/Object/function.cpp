#include "function.h"
#include "object.h"

namespace Ming3D
{
	Function::Function(const char* inFuncName, FunctionCallerBase* inFuncCaller)
	{
		mFunctionName = inFuncName;
		mFunctionCaller = inFuncCaller;
	}

	void Function::CallFunction(Object* inObject, const FunctionArgs& inArgs)
	{
		mFunctionCaller->CallFunction(inObject, inArgs.mParamList);
	}

	std::string Function::GetFunctionName()
	{
		return mFunctionName;
	}

	void Function::SerialiseFunctionArgs(FunctionArgs& inArgs, DataWriter& outData)
	{
		mFunctionCaller->SerialiseFunctionArgs(inArgs.mParamList, outData);
	}

	FunctionArgs Function::DeserialiseFunctionArgs(DataWriter& inData)
	{
		std::vector<FunctionParamBase> params;
		mFunctionCaller->DeserialiseFunctionArgs(inData, params);
		return FunctionArgs(params);
	}
}
