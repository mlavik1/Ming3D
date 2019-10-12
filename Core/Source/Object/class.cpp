#include "class.h"
#include "object.h"
#include "Source/Debug/st_assert.h"
#include <cstring>

namespace Ming3D
{
	Class::Class(const char* arg_name, staticclassinitialiser_t arg_initialiser, staticconstructor_t constructor, Ming3D::Class* superclass)
		: mClassName(arg_name), mClassInitialiser(arg_initialiser), mStaticConstructor(constructor), mBaseClass(superclass)
	{
		__AssertComment(arg_name != "", "ClassName cannot be empty");
		if (mBaseClass != nullptr)
		{
			mBaseClass->mChildClasses.push_back(this);
		}
	}

	void Class::AddMemberFunction(Function* arg_function)
	{
		mMemberFunctions[arg_function->GetFunctionName()] = arg_function;
	}

    void Class::AddProperty(Property* arg_property)
    {
        mProperties[arg_property->GetPropertyName()] = arg_property;
    }

	const std::string &Class::GetFullName() const
	{
		return mClassName;
	}


	std::string Class::GetName() const
	{
		size_t i = mClassName.find_last_of(':');
		if (i == std::string::npos)
		{
			return mClassName;
		}
		return mClassName.substr(i + 1, mClassName.size() - i - 1);
	}


	const Class* Class::GetBaseClass() const
	{
		return mBaseClass;

	}


	bool Class::IsSubclassOf(Class* arg_other) const
	{
		if (this == arg_other)
			return true;
		else if (mBaseClass == nullptr)
			return false;
		else
			return mBaseClass->IsSubclassOf(arg_other);
	}


	bool Class::IsA(Class* arg_other) const
	{
		return this == arg_other || this->IsSubclassOf(arg_other);
	}


	Object* Class::CreateInstance()
	{
		if (mStaticConstructor != nullptr)
		{
			Object* createdObject = mStaticConstructor();
			return createdObject;
		}
		else
		{
			return nullptr;
		}
	}

	std::vector<Class*> Class::GetChildClasses()
	{
		return mChildClasses;
	}

	Class* Class::GetChildClassByName(const char* arg_name, bool arg_fullname) const
	{
		for (Class* childClass : mChildClasses)
		{
			if ((arg_fullname && std::strcmp(childClass->GetFullName().c_str(), arg_name) == 0)
				|| (!arg_fullname && std::strcmp(childClass->GetName().c_str(), arg_name) == 0))
			{
				return childClass;
			}
			return childClass->GetChildClassByName(arg_name, arg_fullname);
		}
		return nullptr;
	}

	Function* Class::GetFunctionByName(const char* arg_name)
	{
		auto iter = mMemberFunctions.find(arg_name);
		if (iter != mMemberFunctions.end())
		{
			return iter->second;
		}
		if (mBaseClass != nullptr)
			return mBaseClass->GetFunctionByName(arg_name); // TODO: ADD BASE CLASS FUNCTIONS TO MAP?
		return nullptr;
	}

    Property* Class::GetPropertyByName(const char* arg_name)
    {
        auto iter = mProperties.find(arg_name);
        if (iter != mProperties.end())
        {
            return iter->second;
        }
        if (mBaseClass != nullptr)
            return mBaseClass->GetPropertyByName(arg_name); // TODO: ADD BASE CLASS PROEPRTIES TO MAP?
        return nullptr;
    }

    std::vector<Property*> Class::GetAllProperties(bool arg_recursive)
    {
        std::vector<Property*> props;
        for (auto prop : mProperties)
            props.push_back(prop.second);
        if (arg_recursive)
        {
            Class* currClass = mBaseClass;
            while (currClass != nullptr)
            {
                for (auto prop : currClass->mProperties)
                    props.push_back(prop.second);
                currClass = currClass->mBaseClass;
            }
        }
        return props;
    }


	void Class::InitialiseClass()
	{
		mClassInitialiser();
	}

	Class* Class::GetClassByName(const char* arg_name, bool arg_fullname)
	{
		return Object::GetStaticClass()->GetChildClassByName(arg_name, arg_fullname);
	}
}
