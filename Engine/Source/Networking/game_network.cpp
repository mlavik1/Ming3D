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
            mHostSocket = GGameEngine->GetPlatform()->CreateSocket();
            mHostSocket->Initialise(inHost, inPort, 0);
            mConnectedToHost = mHostSocket->Connect();
            listenPort = mHostSocket->GetLocalPort();
            if (inPort == -1)
            {
                LOG_ERROR() << "Failed to initialise sockets. Local port of host socket is invalid";
                return; // TODO: cleanup
            }
            SetSocket(0, mHostSocket);
        }
        else
        {
            SetSocket(0, mListenSocket);
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
        if (inConnSock != nullptr)
        {
            LOG_INFO() << "Incoming connection from client";
            SetSocket(mSockets.size(), inConnSock);
        }
        
        if (!mIsHost && !mConnectedToHost)
            mConnectedToHost = mHostSocket->Connect(); // TODO: Change return value to ESocketConnectResult

        HandleIncomingMessages();

        SendQueuedMessages();
    }

    void GameNetwork::HandleIncomingMessages()
    {
        for (size_t i = 0; i < mSockets.size(); i++)
        {
            int bytesReceived = mSockets[i]->Recv(mDataBuffer, MING3D_DEFAULT_BUFLEN);
            if (bytesReceived > 0)
            {
                ClientMessage msg;
                msg.mClientID = i;
                // Read data
                DataWriter dataWriter(bytesReceived);
                dataWriter.Write(mDataBuffer, bytesReceived);
                // Create NetMessage and deserialise message data
                NetMessage* netMsg = new NetMessage();
                netMsg->Deserialise(&dataWriter);
                msg.mMessage = netMsg;
                mIncomingMessages.push_back(msg);
            }
        }

        for (ClientMessage& msg : mIncomingMessages)
        {
            LOG_INFO() << msg.mMessage->GetMessageData(); // TEMP TEST - REMOVE ME!!
            // TODO: Call RPCs, etc.. !!!
            delete msg.mMessage;
        }

        mIncomingMessages.clear();
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
            SendMessageInternal(currMessage.mMessage, mSockets[currMessage.mClientID]);
        }
        mOutgoingMessages.clear();
    }

    void GameNetwork::SendMessageInternal(NetMessage* inNetMessage, NetSocket* inSocket)
    {
        DataWriter* dataWriter = inNetMessage->Serialise();
        inSocket->Send(dataWriter->GetData(), dataWriter->GetSize());
        delete dataWriter;
        delete inNetMessage;
    }

    void GameNetwork::SetSocket(int inSocketID, NetSocket* inSocket)
    {
        if (mSockets.size() <= inSocketID)
            mSockets.resize(inSocketID + 1);
        mSockets[inSocketID] = inSocket;
    }
}
