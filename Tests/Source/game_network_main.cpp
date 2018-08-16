#if MING3D_TESTTYPE == 4

#include "GameEngine/game_engine.h"
#include "Networking/game_network.h"
#include "Networking/network_manager.h"
#include "Debug/debug.h"

#include <iostream>

#define TEST_PORT_NUMBER 2024

using namespace Ming3D;

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
    }

    bool hasSentMessage = false;

    while (true)
    {
        if (!hasSentMessage && !network->IsHost() && network->IsConnectedToHost())
        {
            LOG_INFO() << "Sending message to server...";
            NetMessage* msg = new NetMessage(NetMessageType::Log, "Hello, from client");
            network->SendMessage(msg, 0);
            hasSentMessage = true;
        }

        gameEngine.Update();
    }

    return 0;
}

#endif
