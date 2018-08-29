#include "data_writer.h"

namespace Ming3D
{
	DataWriter::DataWriter(const size_t& arg_preallocateBytes)
	{
		Reset(arg_preallocateBytes);
	}

	DataWriter::DataWriter(const DataWriter& arg_other)
	{
		mData = new char[arg_other.mBytesAllocated];
		mDataReadPos = mData;
		mBytesAllocated = arg_other.mBytesAllocated;
		mBytesWritten = arg_other.mBytesWritten;
		memcpy(mData, arg_other.mData, mBytesWritten);
	}

	DataWriter::~DataWriter()
	{
		delete[] mData;
	}

	void DataWriter::Reset(const size_t& arg_preallocateBytes)
	{
		if (mData != nullptr)
		{
			delete[] mData;
		}

		mData = new char[arg_preallocateBytes];
		mDataReadPos = mData;
		mBytesAllocated = arg_preallocateBytes;
		mBytesWritten = 0;
	}

	void DataWriter::Write(const void* arg_data, const size_t& arg_bytes)
	{
		const size_t newSize = mBytesWritten + arg_bytes;
		if (newSize > mBytesAllocated)
		{
            mBytesAllocated = arg_bytes > mBytesAllocated ? newSize : mBytesAllocated * 2;
			char* newData = new char[mBytesAllocated];
			if (mData != nullptr)
			{
				memcpy(newData, mData, mBytesWritten);
				delete[] mData;
			}
			mData = newData;
			mDataReadPos = mData;
		}
		memcpy(mData + mBytesWritten, arg_data, arg_bytes);

		mBytesWritten = newSize;
	}

    void DataWriter::SetData(void* arg_data, const size_t& arg_bytes)
    {
        if (mData != nullptr)
        {
            delete[] mData;
        }
        mData = (char*)arg_data;
        mBytesWritten = arg_bytes;
        mBytesAllocated = arg_bytes;
    }

	void DataWriter::Read(void* arg_location, const size_t& arg_bytes)
	{
		memcpy(arg_location, mDataReadPos, arg_bytes);
		mDataReadPos += arg_bytes;
	}

	void DataWriter::SkipBytes(const size_t& arg_bytes)
	{
		mDataReadPos += arg_bytes;
	}

    void DataWriter::SetReadPos(size_t arg_pos)
    {
        mDataReadPos = mData + arg_pos;
    }

} // namespace Ming3D
