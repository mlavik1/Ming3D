#ifndef MING3D_NETMESSAGE_H
#define MING3D_NETMESSAGE_H

#include <string>
#include <vector>
#include "net_message_type.h"
#include "Serialisation/data_writer.h"

typedef uint8_t msgtype_t;
typedef uint16_t msglen_t;

namespace Ming3D
{
	class NetMessage
	{
	private:
		NetMessageType mMessageType;
        DataWriter* mDataWriter;

	public:
        NetMessage();
        NetMessage(const DataWriter& inWriter);
		NetMessage(NetMessageType in_type, msglen_t in_length, const void* in_message);
		NetMessage(NetMessageType in_type, std::string in_message);
		NetMessage(const NetMessage& in_other);
        ~NetMessage();
        NetMessage operator=(NetMessage &in_other);

		inline NetMessageType GetMessageType() const { return mMessageType; }
		inline msglen_t GetMessageLength() const { return mDataWriter->GetSize(); }
		inline size_t GetTotalLength() const { return GetMessageLength() + sizeof(msgtype_t) + sizeof(msglen_t); }

		const char* GetMessageData() const;

        DataWriter* Serialise();
        void Deserialise(DataWriter* inDataWriter);

		bool GetIsValid() const;
	};
}

#endif // #ifndef MING3D_NETMESSAGE_H
