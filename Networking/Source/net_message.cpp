#include "net_message.h"

#include <sstream>
#include "Debug/st_assert.h"

namespace Ming3D
{
    NetMessage::NetMessage()
    {
        mDataWriter = nullptr;
        mMessageType = NetMessageType::Ignored;
    }

    NetMessage::NetMessage(const DataWriter& inWriter)
    {
        mDataWriter = new DataWriter(inWriter);
    }

    NetMessage::NetMessage(NetMessageType in_type, msglen_t in_length, const void* in_message)
    {
        mMessageType = in_type;
        mDataWriter = new DataWriter(in_length);
        mDataWriter->Write(in_message, in_length);
	}

	NetMessage::NetMessage(NetMessageType in_type, std::string in_message)
    {
        const size_t msgLen = in_message.size();
        mMessageType = in_type;
        mDataWriter = new DataWriter(msgLen);
        mDataWriter->Write(in_message.c_str(), msgLen + 1);
	}

    NetMessage::NetMessage(const NetMessage& in_other)
        : mMessageType(in_other.mMessageType)
    {
        mMessageType = in_other.mMessageType;
        mDataWriter = new DataWriter(*in_other.mDataWriter);
    }

	NetMessage::~NetMessage()
	{
		if (mDataWriter != nullptr)
		{
			delete mDataWriter;
		}
	}

	NetMessage NetMessage::operator=(NetMessage &in_other)
	{
		return NetMessage(in_other.GetMessageType(), in_other.GetMessageLength(), in_other.GetMessageData());
	}

	bool NetMessage::GetIsValid() const
	{
		return true; // TODO
	}

	const char*	NetMessage::GetMessageData() const
	{
        return mDataWriter->GetData();
	}

    DataWriter* NetMessage::Serialise()
    {
        const size_t dataLength = GetTotalLength();
        const msglen_t messageLength = GetMessageLength();
        DataWriter* serialisedDataWriter = new DataWriter(dataLength);
        serialisedDataWriter->Write(&mMessageType, sizeof(msgtype_t));
        serialisedDataWriter->Write(&messageLength, sizeof(msglen_t));
        serialisedDataWriter->Write(mDataWriter->GetData(), dataLength);
        return serialisedDataWriter;
    }

    void NetMessage::Deserialise(DataWriter* inDataWriter)
    {
        msglen_t msgLength;
        // Read message type
        inDataWriter->Read(&mMessageType, sizeof(msgtype_t));
        // Read message length
        inDataWriter->Read(&msgLength, sizeof(msglen_t));
        void* msgData = new char[msgLength];
        // Read message data
        inDataWriter->Read(msgData, msgLength);
        // Write message data to data writer
        if (mDataWriter == nullptr)
            mDataWriter = new DataWriter(msgLength);
        mDataWriter->SetData(msgData, msgLength);
    }

}
