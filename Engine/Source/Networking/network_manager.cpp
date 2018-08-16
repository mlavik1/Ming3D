#include "network_manager.h"

namespace Ming3D
{
    void NetworkManager::AddNetwork(GameNetwork* inNetwork)
    {
        mNetworks.push_back(inNetwork);
    }

    void NetworkManager::RemoveNetwork(GameNetwork* inNetwork)
    {
        for (auto i = mNetworks.begin(); i != mNetworks.end(); i++)
        {
            if (*i == inNetwork)
            {
                mNetworks.erase(i);
                return;
            }
        }
    }

    void NetworkManager::UpdateNetworks()
    {
        for (GameNetwork* network : mNetworks)
        {
            network->Update();
        }
    }
}
