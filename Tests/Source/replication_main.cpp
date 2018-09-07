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


    // Host
    if (network->IsHost())
    {
        TestActor* testActor1 = nullptr; // registered before client connect
        testActor1 = new TestActor();
        testActor1->TestPropertyInt = 123;
        testActor1->TestPropertyFloat = 321.0f;
        network->ReplicateNetworkedObject(testActor1);

        TestActor* testActor2 = nullptr; // registered after client connect
        testActor2 = new TestActor();
        testActor2->TestPropertyInt = 456;
        testActor2->TestPropertyFloat = 654.3f;

        bool hasReplicatedActor = false;

        while (true)
        {
            if (!hasReplicatedActor && network->GetNumConnections() > 1)
            {
                network->ReplicateNetworkedObject(testActor2);
                hasReplicatedActor = true;
            }
            gameEngine.Update();
        }
    }
    else
    {
        size_t nextNetObjIndex = 0;
        while (true)
        {
            while (nextNetObjIndex < network->GetNetworkedObjects().size())
            {
                GameObject* obj = network->GetNetworkedObjects().at(nextNetObjIndex);
                if (obj->GetClass() == TestActor::GetStaticClass())
                {
                    TestActor* testActor = (TestActor*)obj;
                    LOG_INFO() << "Received replicated actor: " << testActor->GetStaticClass()->GetName();
                    LOG_INFO() << "Replicated properties: " << testActor->TestPropertyInt << ", " << testActor->TestPropertyFloat;
                }
                nextNetObjIndex++;
            }
            gameEngine.Update();
        }
    }

    return 0;
}

#endif
