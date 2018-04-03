#ifndef MING3D_ISERIALISABLE_H
#define MING3D_ISERIALISABLE_H

#include "data_writer.h"

namespace Ming3D
{
	/**
	* Interface for serialisable types.
	* Allows you to implement custom serialising and deserialising logic.
	*/
	class ISerialisable
	{
		virtual void Read(DataWriter& arg_writer) = 0;
		virtual void Write(DataWriter& arg_writer) = 0;
	};
}

#endif
