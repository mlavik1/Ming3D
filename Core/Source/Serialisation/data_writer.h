#ifndef MING3D_DATAWRITER_H
#define MING3D_DATAWRITER_H

#include <memory>

namespace Ming3D
{
    /**
    * Datawriter.
    * Used for reading/writing raw data.
    */
	class DataWriter
	{
	public:
		char* mData = nullptr;
		char* mDataReadPos = nullptr;
		size_t mBytesWritten = 0;
		size_t mBytesAllocated = 0;

	public:
		DataWriter(const size_t& arg_preallocateBytes);
		DataWriter(const DataWriter& arg_other);
		~DataWriter();

		void Reset(const size_t& arg_preallocateBytes);

		void Write(const void* arg_data, const size_t& arg_bytes);
        void SetData(void* arg_data, const size_t& arg_bytes);

		void Read(void* arg_location, const size_t& arg_bytes);

		void SkipBytes(const size_t& arg_bytes);

		inline const char* PeekData() const { return mDataReadPos; }
		inline const char* GetData() const { return mData; }
		inline size_t GetSize() { return mBytesWritten; }

	};
} // namespace Ming3D

#endif
