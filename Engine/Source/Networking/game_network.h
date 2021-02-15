#ifndef MING3D_GAMENETWORK_H
#define MING3D_GAMENETWORK_H

#include "net_socket.h"
#include "net_connection.h"
#include <unordered_map>
#include "Actors/actor.h"
#include "net_target.h"

#include "Object/game_object.h"

namespace Ming3D
{
    struct IncomingMessage
    {
        int mClientID;
        NetMessage* mMessage;
    };

    struct OutgoingMessage
    {
        NetTarget mTarget;
        int mClientID = -1;
        NetMessage* mMessage;
    };

    /**
    * Game Network class.
    * Handles communication between clients and a host.
    */
    class GameNetwork
    {
    private:
        char mDataBuffer[MING3D_DEFAULT_BUFLEN];

        std::vector<IncomingMessage> mIncomingMessages;
        std::vector<OutgoingMessage> mOutgoingMessages;

        NetSocket* mListenSocket = nullptr;
        NetConnection* mHostConnection = nullptr;

        std::vector<NetConnection*> mConnections;

        bool mIsActive = false;
        bool mIsHost = false;
        bool mConnectedToHost = false;

        std::unordered_map<netguid_t, GameObject*> mNetworkedObjects;

        netguid_t mNetGUIDSequence = 0;

        void HandleIncomingMessages();
        void SendQueuedMessages();
        void SendMessageToSelf(NetMessage* inMessage);

        void SendDataToConnecton(DataWriter* inWriter, NetConnection* inConnection);
        void SetConnection(int inSocketID, NetConnection* inConnection);
        void HandleClientConnected(int clientID);

        /** Crates a NetMessage for calling the specified function (with the specified arguments) remotely. */
        NetMessage* CreateRPCMessage(GameObject* inObject, const char* inFunctionName, FunctionArgs inArgs);
        /** Serialises the object and creates a Netmessage for creating the networked object remotely. */
        NetMessage* CreateRepConstructMessage(GameObject* inObject);

    public:
        void Connect(const char* inHost, int inPort);
        void Update();

        /** Send a message to the specified NetTarget. */
        void SendMessage(NetMessage* inMessage, NetTarget inTarget);
        /** Send a message to a single client. */
        void SendMessage(NetMessage* inMessage, int inClient);
        /** Returns true if running as host of the game network. */
        bool IsHost() { return mIsHost; }
        /** Returns true if a connection with the host has been established */
        bool IsConnectedToHost() { return mConnectedToHost; }
        NetConnection* GetConnection(int id) { return mConnections[id]; }
        size_t GetNumConnections() { return mConnections.size(); }
        std::vector<IncomingMessage> GetIncomingMessages() { return mIncomingMessages; }
        std::vector<GameObject*> GetNetworkedObjects();

        /** Replicates an object to all connected clients. The object will be created on all clients, and have its properties replicated. */
        void ReplicateNetworkedObject(GameObject* inActor);
        /** Registers a networked object, with the specified net GUID. */
        void RegisterNetworkedObject(GameObject* inObject, netguid_t inGUID); // TEMP TEST

        void CallRPC(GameObject* inObject, const char* inFunctionName, FunctionArgs inArgs, int inClient);
        void CallRPC(GameObject* inObject, const char* inFunctionName, FunctionArgs inArgs, NetTarget inTarget);

    };
}

#endif
