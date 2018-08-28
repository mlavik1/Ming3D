#if MING3D_TESTTYPE == 5

#include "GameEngine/game_engine.h"
#include "Networking/game_network.h"
#include "Networking/network_manager.h"
#include "Debug/debug.h"

#include "test_actor.h"

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
        _sleep(300);
    }

    bool hasSentRPC = false;

    TestActor* testActor = new TestActor();
    network->RegisterNetworkedObject(testActor, 1);

    while (true)
    {
        // Host
        if (network->IsHost())
        {
            
        }
        // Client
        else if (network->IsConnectedToHost())
        {
            if (!hasSentRPC)
            {
                network->CallRPC(testActor, "IntBoolTestFunction", FunctionArgs({ FunctionParam<int>(42), FunctionParam<bool>(true) }), 0);
                hasSentRPC = true;
            }
        }
        gameEngine.Update(); // Update the engine (to send and receive messages through game network)
    }

    return 0;
}

#endif
