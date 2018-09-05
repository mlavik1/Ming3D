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

        for (size_t i = mIsHost ? 1 : 0/*TODO*/; i < mConnections.size(); i++)
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
                {
                    LOG_INFO() << msg.mMessage->GetMessageData();
                    break;
                }
                case NetMessageType::RPC:
                {
                    DataWriter* reader = msg.mMessage->GetDataWriter();
                    reader->SetReadPos(0);
                    msglen_t msgLength = reader->GetSize();

                    netguid_t netGUID = 0;
                    size_t funcNameLength = 0;

                    reader->Read(&netGUID, sizeof(netguid_t));
                    reader->Read(&funcNameLength, sizeof(size_t));
                    char* funcName = new char[funcNameLength];
                    reader->Read(funcName, funcNameLength);

                    // Find the object
                    auto objIter = mNetworkedObjects.find(netGUID);
                    if (objIter != mNetworkedObjects.end())
                    {
                        GameObject* targetObject = objIter->second;
                        // Find the function
                        Function* func = targetObject->GetClass()->GetFunctionByName(funcName);
                        FunctionArgs funcArgs = func->DeserialiseFunctionArgs(*reader);
                        // Call RPC function on object
                        func->CallFunction(targetObject, funcArgs);
                    }
                    else
                    {
                        LOG_ERROR() << "No registered networked object with GUID: " << netGUID;
                    }

                    delete funcName;
                    break;
                }
                case NetMessageType::ObjectCreation:
                {
                    DataWriter* reader = msg.mMessage->GetDataWriter();
                    reader->SetReadPos(0);
                    netguid_t netGUID = 0;
                    reader->Read(&netGUID, sizeof(netguid_t));
                    size_t classNameLen = 0;
                    reader->Read(&classNameLen, sizeof(size_t));
                    char* className = new char[classNameLen];
                    reader->Read(className, classNameLen);

                    Class* objClass = Class::GetClassByName(className, false);
                    if (objClass == nullptr)
                    {
                        delete[] className;
                        return;
                    }
                    // Create object
                    GameObject* obj = (GameObject*)objClass->CreateInstance();
                    // Register object in network
                    RegisterNetworkedObject(obj, netGUID);
                    // Deserialise properties/components/children
                    obj->ReceiveReplicateConstruct(reader);
                    break;
                }
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
            if (currMessage.mClientID == -1)
            {
                // TEMP: Implement NetMessage targets
                for(int iClient = mIsHost ? 1 : 0/*TODO*/; iClient < mConnections.size(); iClient++)
                    SendMessageInternal(currMessage.mMessage, mConnections[iClient]);
            }
            else
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

    std::vector<GameObject*> GameNetwork::GetNetworkedObjects()
    {
        std::vector<GameObject*> objects;
        for (auto pair : mNetworkedObjects)
            objects.push_back(pair.second);
        return objects;
    }

    void GameNetwork::ReplicateNetworkedObject(GameObject* inActor)
    {
        inActor->mNetGUID = mNetGUIDSequence++;
        mNetworkedObjects[inActor->mNetGUID] = inActor;
        DataWriter* writer = new DataWriter(64);
        writer->Write(inActor->mNetGUID);
        size_t classNameLen = inActor->GetClass()->GetName().size() + 1;
        writer->Write(classNameLen);
        writer->Write(inActor->GetClass()->GetName().c_str(), classNameLen);
        inActor->ReplicateConstruct(writer);
        NetMessage* msg = new NetMessage(NetMessageType::ObjectCreation, writer);
        SendMessage(msg, -1);
    }

    void GameNetwork::RegisterNetworkedObject(GameObject* inObject, netguid_t inGUID)
    {
        inObject->mNetGUID = inGUID;
        mNetworkedObjects[inGUID] = inObject;
    }

    NetMessage* GameNetwork::CreateRPCMessage(GameObject* inObject, const char* inFunctionName, FunctionArgs inArgs)
    {
        Function* func = inObject->GetClass()->GetFunctionByName(inFunctionName);
        if (func == nullptr)
        {
            LOG_ERROR() << "Found no function by name: " << inFunctionName;
            return nullptr;
        }
        DataWriter* writer = new DataWriter(sizeof(inArgs));

        const size_t funcNameLen = std::strlen(inFunctionName) + 1;
        writer->Write(&inObject->mNetGUID, sizeof(netguid_t)); // GUID
        writer->Write(&funcNameLen, sizeof(size_t)); // function name length
        writer->Write(inFunctionName, funcNameLen); // function name (TODO: Use index or something else)
        func->SerialiseFunctionArgs(inArgs, *writer); // function arguments

        NetMessage* msg = new NetMessage(NetMessageType::RPC, writer);
        msg->SetMessageData(writer);
        return msg;
    }

    void GameNetwork::CallRPC(GameObject* inObject, const char* inFunctionName, FunctionArgs inArgs, int inClient)
    {
        NetMessage* msg = CreateRPCMessage(inObject, inFunctionName, inArgs);
        
        if (msg == nullptr)
            return;

        SendMessage(msg, inClient);
    }

    void GameNetwork::CallRPC(GameObject* inObject, const char* inFunctionName, FunctionArgs inArgs, NetTarget inTarget)
    {
        NetMessage* msg = CreateRPCMessage(inObject, inFunctionName, inArgs);

        if (msg == nullptr)
            return;

        switch (inTarget)
        {
        case NetTarget::Host:
            SendMessage(msg, 0);
            break;
        case NetTarget::Clients:
            for (int i = 1; i < mConnections.size(); i++)
            {
                SendMessage(msg, i);
            }
            break;
        case NetTarget::Everyone:
            for (int i = 1; i < mConnections.size(); i++)
            {
                SendMessage(msg, i);
            }
            inObject->GetStaticClass()->GetFunctionByName(inFunctionName)->CallFunction(inObject, inArgs);
            break;
        }

    }
}
