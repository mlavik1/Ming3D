#ifndef MING3D_DATASERIALISATION_H
#define MING3D_DATASERIALISATION_H

#include <string>
#include <vector>

#include "i_serialisable.h"

namespace Ming3D
{
	template<typename T, typename ENABLE = void>
	struct TypeSerialisationTraits
	{
		static constexpr bool valid = true;

		template<typename DataWriter>
		static void Write(DataWriter& writer, const T& v)
		{
			writer.Write(&v, sizeof(v));
		}

		template<typename DataReader>
		static void Read(DataReader& reader, T& v)
		{
			reader.Read(&v, sizeof(v));
		}
	};

	template<typename T>
	struct TypeSerialisationTraits<T, typename std::enable_if_t<std::is_arithmetic<T>::value>>
	{
		static constexpr bool valid = true;

		template<typename DataWriter>
		static void Write(DataWriter& writer, const T& v)
		{
			writer.Write(&v, sizeof(v));
		}

		template<typename DataReader>
		static void Read(DataReader& reader, T& v)
		{
			reader.Read(&v, sizeof(v));
		}
	};

	template<>
	struct TypeSerialisationTraits<std::string>
	{
		static constexpr bool valid = true;

		template<typename DataWriter>
		static void Write(DataWriter& writer, const std::string& v)
		{
			writer.Write(v.c_str(), v.length() + 1);
		}

		template<typename DataReader>
		static void Read(DataReader& reader, std::string& v)
		{
			v = reader.PeekData();
			reader.SkipBytes(v.length() + 1);
		}
	};

	template<typename T>
	struct TypeSerialisationTraits<std::vector<T>>
	{
		static constexpr bool valid = true;

		template<typename DataWriter>
		static void Write(DataWriter& writer, const std::vector<T>& v)
		{
			size_t len = v.size();
			writer.Write(&len, sizeof(size_t));
			for (const T& elem : v)
			{
				TypeSerialisationTraits<T>::Write(writer, elem);
			}
		}

		template<typename DataReader>
		static void Read(DataReader& reader, std::vector<T>& v)
		{
			size_t len;
			reader.Read(&len, sizeof(size_t));
			for (size_t i = 0; i < len; i++)
			{
				T elem;
				TypeSerialisationTraits<T>::Read(reader, elem);
				v.push_back(elem);
			}
		}
	};


	template<typename T>
	struct TypeSerialisationTraits<T, typename std::enable_if_t<std::is_base_of<ISerialisable,T>::value>>
	{
		static constexpr bool valid = true;

		template<typename DataWriter>
		static void Write(DataWriter& writer, T& v)
		{
			v.Write(writer);
		}

		template<typename DataReader>
		static void Read(DataReader& reader, T& v)
		{
			v.Read(reader);
		}
	};


} // namespace Ming3D

#endif // MING3D_DATASERIALISATION_H
