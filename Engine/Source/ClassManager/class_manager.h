#ifndef MING3D_CLASSMANAGER_H
#define MING3D_CLASSMANAGER_H

#include "Object/class.h"

namespace Ming3D
{
	class ClassManager
	{
	private:
		void InitialiseClassRecursive(Class* inClass);
	public:
		void InitialiseClasses();
	};
}

#endif
