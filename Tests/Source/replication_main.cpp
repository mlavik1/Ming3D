#if MING3D_TESTTYPE == 6

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

    TestActor* testActor = nullptr;
    if (network->IsHost())
    {
        testActor = new TestActor();
        testActor->TestPropertyInt = 123;
        testActor->TestPropertyFloat = 321.0f;
    }
    bool hasReplicatedActor = false;

    while (true)
    {
        // Host
        if (network->IsHost())
        {
            if (!hasReplicatedActor && network->GetNumConnections() > 1)
            {
                network->ReplicateNetworkedObject(testActor);
                hasReplicatedActor = true;
            }
        }
        // Client
        else if (network->IsConnectedToHost())
        {
            if (testActor == nullptr)
            {
                for (GameObject* obj : network->GetNetworkedObjects())
                {
                    if (obj->GetClass() == TestActor::GetStaticClass())
                    {
                        testActor = (TestActor*)obj;
                        LOG_INFO() << "Received replicated actor: " << testActor->GetStaticClass()->GetName();
                        LOG_INFO() << "Replicated properties: " << testActor->TestPropertyInt << ", " << testActor->TestPropertyFloat;
                    }
                }
            }
        }
        gameEngine.Update(); // Update the engine (to send and receive messages through game network)
    }

    return 0;
}

#endif
