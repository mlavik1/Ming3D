#include "class_manager.h"

#include "Object/object.h"

namespace Ming3D
{
	void ClassManager::InitialiseClassRecursive(Class* inClass)
	{
		inClass->InitialiseClass();
		for (auto subClass : inClass->GetChildClasses())
		{
			InitialiseClassRecursive(subClass);
		}
	}

	void ClassManager::InitialiseClasses()
	{
		InitialiseClassRecursive(Object::GetStaticClass());
	}
}
