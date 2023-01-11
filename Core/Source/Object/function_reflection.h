#ifndef MING3D_FUNCTIONREFLECTION_H
#define MING3D_FUNCTIONREFLECTION_H

#include "Serialisation/data_serialisation.h"
#include "Serialisation/data_writer.h"
#include <memory>
#include <vector>

namespace Ming3D
{
	/**
	* Base class for function parameters.
	* Stores a single argument.
	* Does not do proper serialisation, but simply copies the value using the copy constructor.
	*/
	class FunctionParamBase
	{
	protected:
		std::shared_ptr<void> mValuePtr;
	public:
		const void* GetValuePtr() const
		{
			return mValuePtr.get();
		}
	};

	/**
	* Template subclass of FunctionParam for function parameter of type T.
	*/
	template<typename T>
	class FunctionParam : public FunctionParamBase
	{
	public:
		FunctionParam(const T& newValue)
		{
			// Create a copy using the copy constructor
			mValuePtr = std::shared_ptr<void>(new T(newValue));
		}
	};

	/**
	* Helper class for calling functions.
	*/
	class FunctionCallHelper
	{
	public:
		template<typename ReturnType, typename Class, typename ... Param, std::size_t ... I>
		static void CallFunction(ReturnType(Class::*func)(Param...), void* obj, std::vector<FunctionParamBase> funcParams, std::index_sequence<I...>)
		{
			(((Class*)obj)->*func)(*(Param*)funcParams[I].GetValuePtr()...);
		}
	};

	/**
	* Helper class for serialising and deserialising function parameters.
	*/
	class FunctionSerialisationHelper
	{
	public:
		template<typename T>
		static bool SerialiseArgument(FunctionParamBase inArg, DataWriter& outData)
		{
			TypeSerialisationTraits<T>::Write(outData, *(T*)inArg.GetValuePtr());
			return true;
		}

		template<typename T>
		static FunctionParamBase DeserialiseArgument(DataWriter& inData)
		{
			T val;
			TypeSerialisationTraits<T>::Read(inData, val);
			return FunctionParam<T>(val);
		}
	};

	/**
	* Reflection wrapper for member functions.
	*/
	class FunctionCallerBase
	{
	public:
		/**
		* Calls the function with the given list of arguments.
		* @param obj       Object to call the function on (function must be a member function of the object's class).
		* @param funcArgs  List of function arguments.
		*/
		virtual void CallFunction(void* obj, std::vector<FunctionParamBase> funcArgs) = 0;

		/**
		* Serialises a list of function arguments.
		* @param inArgs   Arguments to serialise.
		* @param outData  DataWriter where the serialised arguments will be stored.
		*/
		virtual void SerialiseFunctionArgs(const std::vector<FunctionParamBase>& inArgs, DataWriter& outData) = 0;

		/**
		* Deserialises serialised function arguments.
		* @param inData   DataWriter where the serialised arguments are stored.
		* @param outArgs  Function argument container to store deserialised arguments in.
		*/
		virtual void DeserialiseFunctionArgs(DataWriter& inData, std::vector<FunctionParamBase>& outArgs) = 0;
	};

	/**
	* Template subclass of FunctionCallerBase.
	* Stores a pointer to the function, and has implementation for calling the function from a list of parameters.
	*/
	template<typename ReturnType, typename Class, typename ... Param>
	class FunctionCaller : public FunctionCallerBase
	{
	private:
		/** Function pointer to the member function. */
		ReturnType(Class::*mFunc)(Param...);

	public:
		FunctionCaller(ReturnType(Class::*func)(Param...))
		{
			mFunc = func;
		}

		virtual void SerialiseFunctionArgs(const std::vector<FunctionParamBase>& inArgs, DataWriter& outData) override
		{
			size_t i = 0;
			[[maybe_unused]] auto list = { (FunctionSerialisationHelper::SerialiseArgument<Param>(inArgs[i++], outData))... };
		}

		virtual void DeserialiseFunctionArgs(DataWriter& inData, std::vector<FunctionParamBase>& outArgs) override
		{
			outArgs = { (FunctionSerialisationHelper::DeserialiseArgument<Param>(inData))... };
		}

		virtual void CallFunction(void* obj, std::vector<FunctionParamBase> funcArgs) override
		{
			FunctionCallHelper::CallFunction<ReturnType, Class, Param...>(mFunc, obj, funcArgs, std::index_sequence_for<Param...>{});
		}
	};
}

#endif
