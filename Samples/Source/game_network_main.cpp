#if MING3D_TESTTYPE == 4

#include "GameEngine/game_engine.h"
#include "Networking/game_network.h"
#include "Networking/network_manager.h"
#include "Debug/debug.h"

#include <iostream>

#define TEST_PORT_NUMBER 2024

using namespace Ming3D;

enum NetworkTestStage
{
    NormalMessage = 1,
    PartialMessage = 2
};

NetworkTestStage currentTestStage = NetworkTestStage::NormalMessage;

const char* clientNormalMessage = "This is a normal message";
const char* clientPartialMessage = "This is a partial message";

int main()
{
    GameEngine gameEngine;
    
    gameEngine.Initialise();
    
    GameNetwork* network = new GameNetwork();
    gameEngine.GetNetworkManager()->AddNetwork(network);

    LOG_INFO() << "Type 2 for server or 1 for client: ";
    int val;
    std::cin >> val;
    if (val == 2)
    {
        LOG_INFO() << "I am a server";
        network->Connect(nullptr, TEST_PORT_NUMBER);
    }
    else
    {
        LOG_INFO() << "I am a client";
        network->Connect("127.0.0.1", TEST_PORT_NUMBER);
        _sleep(300);
    }

    bool hasSentMessage = false;

    while (true)
    {
        // Host
        if (network->IsHost())
        {
            std::vector<ClientMessage> newMessages = network->GetIncomingMessages();
            if (newMessages.size() > 0)
            {
                if (currentTestStage > NetworkTestStage::PartialMessage)
                {
                    LOG_ERROR() << "UNEXPECTED MESSAGE: " << newMessages[0].mMessage->GetMessageData();
                }
                for (ClientMessage& msg : newMessages)
                {
                    NetMessage* netMsg = msg.mMessage;
                    LOG_INFO() << "Expected message: " << (currentTestStage == NetworkTestStage::NormalMessage ? clientNormalMessage : clientPartialMessage);
                    LOG_INFO() << "Received message: " << netMsg->GetMessageData();
                    currentTestStage = (NetworkTestStage)((int)currentTestStage + 1);
                }
            }
        }
        // Client
        else if (network->IsConnectedToHost())
        {
            switch (currentTestStage)
            {
                case NetworkTestStage::NormalMessage:
                {
                    LOG_INFO() << "Sending message: " << clientNormalMessage;
                    NetMessage* msg = new NetMessage(NetMessageType::Log, clientNormalMessage);
                    network->SendMessage(msg, 0);
                    currentTestStage = (NetworkTestStage)((int)currentTestStage + 1);
                    break;
                }
                case NetworkTestStage::PartialMessage:
                {
                    LOG_INFO() << "Sending message: " << clientPartialMessage;
                    NetMessage* msg = new NetMessage(NetMessageType::Log, strlen(clientPartialMessage), clientPartialMessage);
                    DataWriter* serialisedData = msg->Serialise();
                    int msgPart1 = serialisedData->GetSize() - 6;
                    network->GetConnection(0)->GetSocket()->Send(serialisedData->GetData(), msgPart1);
                    _sleep(500);
                    network->GetConnection(0)->GetSocket()->Send(serialisedData->GetData() + msgPart1, 6);
                    currentTestStage = (NetworkTestStage)((int)currentTestStage + 1);
                    delete msg;
                    delete serialisedData;
                    break;
                }
            }
        }
        gameEngine.Update(); // Update the engine (to send and receive messages through game network)

        /*
        if (!hasSentMessage && !network->IsHost() && network->IsConnectedToHost())
        {
            LOG_INFO() << "Sending message to server...";
            NetMessage* msg = new NetMessage(NetMessageType::Log, "Hello, from client");
            network->SendMessage(msg, 0);
            hasSentMessage = true;
        }
        gameEngine.Update();
        */
    }

    return 0;
}

#endif
