#ifndef MING3D_FUNCTION_H
#define MING3D_FUNCTION_H

#include <string>
#include "function_reflection.h"

namespace Ming3D
{
	class Object;

	/**
	* Function argument container.
	* Used when calling functions through Function::CallFunction.
	*/
	class FunctionArgs
	{
	public:
		std::vector<FunctionParamBase> mParamList;
		FunctionArgs(const std::vector<FunctionParamBase>& inVec)
		{
			mParamList = inVec;
		}
	};

	/**
	* RTTI and reflection data for member functions of a Ming::Object.
	*/
	class Function
	{
	private:
		std::string mFunctionName;
		FunctionCallerBase* mFunctionCaller;

	public:
		Function(const char* inFuncName, FunctionCallerBase* inFuncCaller);

		/**
		* Calls the function on the specified object with the given arguments.
		* @param inObject  Object to call function on.
		* @param inArgs    Function arguments.
		*/
		void CallFunction(Object* inObject, const FunctionArgs& inArgs);

		std::string GetFunctionName();

		/**
		* Serialises a collection of function arguments.
		* @param inArgs   Arguments to serialise.
		* @param outData  DataWriter to store serialised arguments in.
		*/
		void SerialiseFunctionArgs(FunctionArgs& inArgs, DataWriter& outData);

		/**
		* Deserialises serialised function arguments.
		* @return  Collection of deserialised function arguments.
		*/
		FunctionArgs DeserialiseFunctionArgs(DataWriter& inData);

	};
}

#endif
