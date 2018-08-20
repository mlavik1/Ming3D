#ifndef MING3D_NETCONNECTION_H
#define MING3D_NETCONNECTION_H

#include "net_socket.h"
#include "net_message.h"
#include "Serialisation/data_writer.h"

namespace Ming3D
{
    /**
    * A wrapper around a NetSocket.
    * Used for receiving and sending data.
    */
    class NetConnection
    {
    private:
#define MING3D_DEFAULT_BUFLEN 1024
        char mDataBuffer[MING3D_DEFAULT_BUFLEN];

        NetSocket* mSocket = nullptr;

        std::vector<NetMessage*> mNewMessages;

        int mIncomingPartialMessageLength = 0;
        int mBytesLeft = 0;
        int mReadPos = 0;
        DataWriter* mReader = nullptr;
        void ReadMessage(const char* inData, int inBytesRead);

    public:
        NetConnection(NetSocket* inSocket);

        /** Receive new messages */
        bool Recv();

        /** Returns the NetSocket of the connection */
        inline NetSocket* GetSocket() { return mSocket; }

        /** Returns all messages received, since last call to ClearNewMessages */
        inline std::vector<NetMessage*> GetNewMessages() { return mNewMessages; }

        /** Clears all received messages. Call this when you are done handling all messages, and before calling NetConnection::Recv again */
        void ClearNewMessages();
    };
}

#endif
