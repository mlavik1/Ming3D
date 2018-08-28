#ifndef MING3D_GAMENETWORK_H
#define MING3D_GAMENETWORK_H

#include "net_socket.h"
#include "net_connection.h"
#include <unordered_map>

#include "Object/game_object.h"

namespace Ming3D
{
    enum NetTarget
    {
        Host,
        Clients,
        Everyone
    };

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

        bool mIsActive = false;
        bool mIsHost = false;
        bool mConnectedToHost = false;

        std::unordered_map<netguid_t, GameObject*> mNetworkedObjects;

        void HandleIncomingMessages();
        void SendQueuedMessages();

        void SendMessageInternal(NetMessage* inNetMessage, NetConnection* inConnection);
        void SetConnection(int inSocketID, NetConnection* inConnection);

        NetMessage* CreateRPCMessage(GameObject* inObject, const char* inFunctionName, FunctionArgs inArgs);

    public:
        void Connect(const char* inHost, int inPort);
        void Update();

        void SendMessage(NetMessage* inMessage, int inClient);
        bool IsHost() { return mIsHost; }
        bool IsConnectedToHost() { return mConnectedToHost; }
        NetConnection* GetConnection(int id) { return mConnections[id]; }
        std::vector<ClientMessage> GetIncomingMessages() { return mIncomingMessages; }

        void RegisterNetworkedObject(GameObject* inObject, netguid_t inGUID); // TEMP TEST

        void CallRPC(GameObject* inObject, const char* inFunctionName, FunctionArgs inArgs, int inClient);
        void CallRPC(GameObject* inObject, const char* inFunctionName, FunctionArgs inArgs, NetTarget inTarget);

    };
}

#endif
