#ifndef MING3D_CLASS_H
#define MING3D_CLASS_H

#include <string>
#include <vector>
#include <unordered_map>
#include "function.h"
#include "property.h"

namespace Ming3D
{
	// Forward declarations:
	class Object;

	/**
	* A Class structure for Ming::Object
	* Contains RunTime Type Information and some basic reflection-like functionalities.
	* Use DEFINE_CLASS and IMPLEMENT_CLASS, and inherit from Retro3D::Object to generate a Ming::Class.
	* See objdefs.h
	*/
	class Class
	{
		friend class Object;

		typedef Class*(*staticclassaccessor_t)();
		typedef Object*(*staticconstructor_t)();
		typedef void(*staticclassinitialiser_t)();

	private:
		/** Name of the class. */
		std::string mClassName;

		/** Pointer to base class. */
		Class* mBaseClass;

		/** Child classes of this class. */
		std::vector<Class*> mChildClasses;

		/** Static functions, used to create an instance of the class. */
		staticconstructor_t mStaticConstructor;

		/** Registered member functions. */
		std::unordered_map<std::string, Function*> mMemberFunctions;

        /** Registered class properties. */
        std::unordered_map<std::string, Property*> mProperties;

		/** Class initialiser function pointer. This will get called once for each class. */
		staticclassinitialiser_t mClassInitialiser;

	public:
		Class(const char* arg_name, staticclassinitialiser_t arg_initialiser, staticconstructor_t constructor = 0, Class* superclass = 0);

		void AddMemberFunction(Function* arg_function);

        void AddProperty(Property* arg_property);

		/**
		* Gets the full name of the class, with namespace.
		*
		*/
		const std::string &GetFullName() const;

		/**
		* Gets the simple name of the class, without namespace.
		*
		*/
		std::string GetName() const;

		/**
		* Returns a pointer to the base class of this class.
		*
		*/
		const Class* GetBaseClass() const;

		/**
		* Checks if this class is an subclass of the specified class.
		*
		*/
		bool IsSubclassOf(Class* arg_other) const;

		/**
		* Checks if this class is the same as, or inherits from, the specified class.
		*
		*/
		bool IsA(Class* arg_other) const;

		/**
		* Creates an instances of the class.
		*
		*/
		Object* CreateInstance();

		std::vector<Class*> GetChildClasses();

		/**
		* Recursively searches through the children searching for class by given name.
		*
		* @param arg_name				The name of the class to search for.
		* @param arg_fullname			Use full name (with namespace) when comparing class names.
		*/
		Class* GetChildClassByName(const char* arg_name, bool arg_fullname) const;

		Function* GetFunctionByName(const char* arg_name);

        Property* GetPropertyByName(const char* arg_name);

        /**
        * Returns all class properties.
        * @param arg_recursive          Include properties in base classes
        */
        std::vector<Property*> GetAllProperties(bool arg_recursive);

		/**
		* Initialises the class, which will call the static InitialiseClass-function where we register member functions.
		*/
		void InitialiseClass();

		/**
		* Returns a pointer to the static Class-instance of a class by the given name (if it exists).
		*
		* @param arg_name				The name of the class to search for.
		* @param arg_fullname			Use full name (with namespace) when comparing class names.
		*/
		static Class* GetClassByName(const char* arg_name, bool arg_fullname);


		template<typename ReturnType, typename Class, typename ... Param>
		void RegisterFunction(const char* inName, ReturnType(Class::*inFunc)(Param...))
		{
            Class::GetStaticClass()->AddMemberFunction(new Function(inName, new FunctionCaller<ReturnType, Class, Param...>(inFunc)));
		}

        template<typename VarType, typename Class>
        Property* RegisterProperty(const char* inName, VarType Class::*inVarPtr, PropertyFlag inFlags = (PropertyFlag)0)
        {
            PropertyHandleBase* propHandle = new PropertyHandle<VarType, Class>(inVarPtr);
            Property* prop = new Property(inName, propHandle, typeid(VarType).name(), inFlags);
            AddProperty(prop);
            return prop;
        }
	};
}

#endif
