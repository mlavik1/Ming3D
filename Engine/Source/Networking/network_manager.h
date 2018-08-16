#ifndef MING3D_NETWORKMANAGER_H
#define MING3D_NETWORKMANAGER_H

#include "game_network.h"
#include <vector>

namespace Ming3D
{
    class NetworkManager
    {
    private:
        std::vector<GameNetwork*> mNetworks;

    public:
        void AddNetwork(GameNetwork* inNetwork);
        void RemoveNetwork(GameNetwork* inNetwork);
        void UpdateNetworks();
    };
}

#endif
