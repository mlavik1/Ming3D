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
            const int clientID = mConnections.size();
            LOG_INFO() << "Incoming connection from client";
            SetConnection(clientID, inConnection);
            HandleClientConnected(clientID);
        }
        
        if (!mIsHost && !mConnectedToHost)
            mConnectedToHost = mHostConnection->GetSocket()->Connect(); // TODO: Change return value to ESocketConnectResult

        HandleIncomingMessages();

        SendQueuedMessages();
    }

    void GameNetwork::HandleIncomingMessages()
    {
        for (IncomingMessage& msg : mIncomingMessages)
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
                    IncomingMessage clientMessage;
                    clientMessage.mClientID = i;

                    clientMessage.mMessage = netMsg;
                    mIncomingMessages.push_back(clientMessage);
                }
                mConnections[i]->ClearNewMessages();
            }
        }

        for (IncomingMessage& msg : mIncomingMessages)
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
                    obj->Deserialise(reader, PropertyFlag::InitReplicate, ObjectFlag::InitReplicate);
                    break;
                }
            }
        }
    }


    void GameNetwork::SendMessage(NetMessage* inMessage, NetTarget inTarget)
    {
        OutgoingMessage msg;
        msg.mTarget = inTarget;
        msg.mMessage = inMessage;
        mOutgoingMessages.push_back(msg);
    }

    void GameNetwork::SendMessage(NetMessage* inMessage, int inTarget)
    {
        OutgoingMessage msg;
        msg.mClientID = inTarget;
        msg.mMessage = inMessage;
        mOutgoingMessages.push_back(msg);
    }

    void GameNetwork::SendQueuedMessages()
    {
        for (OutgoingMessage& currMessage : mOutgoingMessages)
        {
            std::vector<int> targets;
            if (currMessage.mClientID > -1)
                targets.push_back(currMessage.mClientID);
            else
            {
                switch (currMessage.mTarget)
                {
                case NetTarget::Everyone:
                    SendMessageToSelf(currMessage.mMessage);

                    for (size_t i = mIsHost ? 1 : 0; i < mConnections.size(); i++)
                        targets.push_back(i);
                    break;
                case NetTarget::Others:
                    for (size_t i = mIsHost ? 1 : 0; i < mConnections.size(); i++)
                        targets.push_back(i);
                    break;
                case NetTarget::Clients:
                    if (!mIsHost)
                    {
                        SendMessageToSelf(currMessage.mMessage);
                    }
                    for (size_t i = 1; i < mConnections.size(); i++)
                        targets.push_back(i);
                    break;
                case NetTarget::Host:
                    if (mIsHost)
                    {
                        SendMessageToSelf(currMessage.mMessage);
                    }
                    else
                    {
                        targets.push_back(0);
                    }
                    break;
                }
            }
            
            for (int iClient : targets)
            {
                DataWriter* dataWriter = currMessage.mMessage->Serialise();
                SendDataToConnecton(dataWriter, mConnections[iClient]);
                delete dataWriter;
                delete currMessage.mMessage;
            }
        }
        mOutgoingMessages.clear();
    }

    void GameNetwork::SendMessageToSelf(NetMessage* inMessage)
    {
        IncomingMessage clientMessage;
        clientMessage.mClientID = -1;
        clientMessage.mMessage = inMessage;
        mIncomingMessages.push_back(clientMessage);
    }

    void GameNetwork::SendDataToConnecton(DataWriter* inWriter, NetConnection* inConnection)
    {
        inConnection->GetSocket()->Send(inWriter->GetData(), inWriter->GetSize());
    }

    void GameNetwork::SetConnection(int inSocketID, NetConnection* inConnection)
    {
        if (mConnections.size() <= inSocketID)
            mConnections.resize(inSocketID + 1);
        mConnections[inSocketID] = inConnection;
    }

    void GameNetwork::HandleClientConnected(int clientID)
    {
        // Replicate networked objects
        for (auto netObjPair : mNetworkedObjects)
        {
            GameObject* obj = netObjPair.second;
            NetMessage* msg = CreateRepConstructMessage(obj);
            SendMessage(msg, clientID);
        }
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
        inActor->SetObjectFlag(ObjectFlag::InitReplicate);
        inActor->mNetGUID = mNetGUIDSequence++;
        mNetworkedObjects[inActor->mNetGUID] = inActor;

        NetMessage* msg = CreateRepConstructMessage(inActor);
        SendMessage(msg, NetTarget::Others);
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

    NetMessage* GameNetwork::CreateRepConstructMessage(GameObject* inObject)
    {
        DataWriter* writer = new DataWriter(64);
        writer->Write(inObject->mNetGUID);
        size_t classNameLen = inObject->GetClass()->GetName().size() + 1;
        writer->Write(classNameLen);
        writer->Write(inObject->GetClass()->GetName().c_str(), classNameLen);
        inObject->Serialise(writer, PropertyFlag::InitReplicate, ObjectFlag::InitReplicate);
        NetMessage* msg = new NetMessage(NetMessageType::ObjectCreation, writer);
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
            SendMessage(msg, NetTarget::Host);
            break;
        case NetTarget::Clients:
            SendMessage(msg, NetTarget::Clients);
            break;
        case NetTarget::Everyone:
            SendMessage(msg, NetTarget::Everyone);
            break;
        }

    }
}
