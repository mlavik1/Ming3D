#ifndef MING3D_GAMENETWORK_H
#define MING3D_GAMENETWORK_H

#include "net_socket.h"
#include "net_connection.h"
#include <unordered_map>

namespace Ming3D
{
    struct ClientMessage
    {
        int mClientID = 0;
        NetMessage* mMessage = nullptr;
    };

    class GameNetwork
    {
    private:
#define MING3D_DEFAULT_BUFLEN 512
        char mDataBuffer[MING3D_DEFAULT_BUFLEN];

        std::vector<ClientMessage> mIncomingMessages;
        std::vector<ClientMessage> mOutgoingMessages;

        NetSocket* mListenSocket = nullptr;
        NetConnection* mHostConnection = nullptr;

        std::vector<NetConnection*> mConnections;
        //std::unordered_map<int, NetSocketTCP*> mClientSockets; // TODO: use a "ClientInfo" struct as value (has socket + IP info + more)

        bool mIsActive = false;
        bool mIsHost = false;

        bool mConnectedToHost = false;

        void HandleIncomingMessages();
        void SendQueuedMessages();

        void SendMessageInternal(NetMessage* inNetMessage, NetConnection* inConnection);
        void SetConnection(int inSocketID, NetConnection* inConnection);

    public:
        void Connect(const char* inHost, int inPort);
        void Update();

        void SendMessage(NetMessage* inMessage, int inClient);

        bool IsHost() { return mIsHost; }
        bool IsConnectedToHost() { return mConnectedToHost; }
        NetConnection* GetConnection(int id) { return mConnections[id]; }
        std::vector<ClientMessage> GetIncomingMessages() { return mIncomingMessages; }
    };
}

#endif
