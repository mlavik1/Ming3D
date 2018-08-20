#include "net_connection.h"

#include <algorithm>

namespace Ming3D
{
    NetConnection::NetConnection(NetSocket* inSocket)
    {
        mSocket = inSocket;
        mReader = new DataWriter(1024); // TODO
    }

    void NetConnection::ReadMessage(const char* inData, int inBytesRead)
    {
        int dataReadPos = 0;
        int bytesLeft = inBytesRead;
        while (bytesLeft > 0)
        {
            int bytesToRead;
            if (mReadPos == 0)
            {
                // Did we receive the beginning of a message, which has fewer bytes than the NetMessage header?
                if (bytesLeft < sizeof(msgtype_t) + sizeof(msglen_t))
                {
                    bytesToRead = bytesLeft;
                    mIncomingPartialMessageLength = -1;
                }
                else
                {
                    int messageContentLength = (int)*(msglen_t*)(inData + sizeof(msgtype_t));
                    int messageLength = messageContentLength + sizeof(msgtype_t) + sizeof(msglen_t);
                    mIncomingPartialMessageLength = messageLength;
                    bytesToRead = bytesLeft < messageLength ? bytesLeft : messageLength;
                }
            }

            mReader->Write(inData + dataReadPos, bytesToRead);
            dataReadPos += bytesToRead;
            bytesLeft -= bytesToRead;

            if (mIncomingPartialMessageLength == -1 && mReader->GetSize() >= sizeof(msgtype_t) + sizeof(msglen_t))
            {
                mIncomingPartialMessageLength = (int)*(msglen_t*)(mReader->GetData() + sizeof(msgtype_t)) + sizeof(msgtype_t);
            }

            // End of message?
            if (mReader->GetSize() == mIncomingPartialMessageLength)
            {
                NetMessage* newMessage = new NetMessage();
                newMessage->Deserialise(mReader);
                mNewMessages.push_back(newMessage);
                mReader = new DataWriter(1024); // the new NetMessage will delete the old mReader
                mIncomingPartialMessageLength = 0;
            }
        }
    }

    bool NetConnection::Recv()
    {
        int bytesReceived = mSocket->Recv(mDataBuffer, MING3D_DEFAULT_BUFLEN);
        if (bytesReceived > 0)
        {
            ReadMessage(mDataBuffer, bytesReceived);
            return true;
        }
        return false;
    }

    void NetConnection::ClearNewMessages()
    {
        mNewMessages.clear();
    }

}
