#include "game_network.h"

#include "Debug/debug.h"
#include "GameEngine/game_engine.h"
#include "Platform/platform.h"

namespace Ming3D
{
    void GameNetwork::Connect(const char* inHost, int inPort)
    {
        mIsHost = inHost == nullptr;
        
        mListenSocket = GGameEngine->GetPlatform()->CreateSocket();

        int listenPort = inPort;
        if (!mIsHost)
        {
            NetSocket* hostSocket = GGameEngine->GetPlatform()->CreateSocket();
            mHostConnection = new NetConnection(hostSocket);
            hostSocket->Initialise(inHost, inPort, 0);
            mConnectedToHost = hostSocket->Connect();
            listenPort = hostSocket->GetLocalPort();
            if (inPort == -1)
            {
                LOG_ERROR() << "Failed to initialise sockets. Local port of host socket is invalid";
                return; // TODO: cleanup
            }
            SetConnection(0, mHostConnection);
        }
        else
        {
            SetConnection(0, nullptr);
        }

        mListenSocket->Initialise(nullptr, listenPort, 0);
        mListenSocket->Bind();
        mListenSocket->Listen();

        mIsActive = true;
    }

    void GameNetwork::Update()
    {
        if (!mIsActive)
            return;

        NetSocket* inConnSock = mListenSocket->Accept();
        NetConnection* inConnection = new NetConnection(inConnSock);
        if (inConnSock != nullptr)
        {
            LOG_INFO() << "Incoming connection from client";
            SetConnection(mConnections.size(), inConnection);
        }
        
        if (!mIsHost && !mConnectedToHost)
            mConnectedToHost = mHostConnection->GetSocket()->Connect(); // TODO: Change return value to ESocketConnectResult

        HandleIncomingMessages();

        SendQueuedMessages();
    }

    void GameNetwork::HandleIncomingMessages()
    {
        for (ClientMessage& msg : mIncomingMessages)
        {
            delete msg.mMessage;
        }
        mIncomingMessages.clear(); // clear messages from last frame

        for (size_t i = 1; i < mConnections.size(); i++)
        {
            bool receivedData = mConnections[i]->Recv();
            if (receivedData)
            {
                for (NetMessage* netMsg : mConnections[i]->GetNewMessages())
                {
                    ClientMessage clientMessage;
                    clientMessage.mClientID = i;

                    clientMessage.mMessage = netMsg;
                    mIncomingMessages.push_back(clientMessage);
                }
                mConnections[i]->ClearNewMessages();
            }
        }

        for (ClientMessage& msg : mIncomingMessages)
        {
            switch (msg.mMessage->GetMessageType())
            {
            case NetMessageType::Log:
                LOG_INFO() << msg.mMessage->GetMessageData();
                break;
            case NetMessageType::RPC:
                // TODO: Call RPC
                break;
            }
        }
    }

    void GameNetwork::SendMessage(NetMessage* inMessage, int inClient)
    {
        ClientMessage msg;
        msg.mClientID = inClient;
        msg.mMessage = inMessage;
        mOutgoingMessages.push_back(msg);
    }

    void GameNetwork::SendQueuedMessages()
    {
        for (ClientMessage& currMessage : mOutgoingMessages)
        {
            SendMessageInternal(currMessage.mMessage, mConnections[currMessage.mClientID]);
        }
        mOutgoingMessages.clear();
    }

    void GameNetwork::SendMessageInternal(NetMessage* inNetMessage, NetConnection* inConnection)
    {
        DataWriter* dataWriter = inNetMessage->Serialise();
        inConnection->GetSocket()->Send(dataWriter->GetData(), dataWriter->GetSize());
        delete dataWriter;
        delete inNetMessage;
    }

    void GameNetwork::SetConnection(int inSocketID, NetConnection* inConnection)
    {
        if (mConnections.size() <= inSocketID)
            mConnections.resize(inSocketID + 1);
        mConnections[inSocketID] = inConnection;
    }
}
