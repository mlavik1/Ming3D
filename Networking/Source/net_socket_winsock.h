#ifndef MING3D_NETSOCKETWINSOCK_H
#define MING3D_NETSOCKETWINSOCK_H

#include "net_socket.h"

#include <winsock2.h>
#include <windows.h>
#include <ws2tcpip.h>
#define MING3D_DEFAULT_BUFLEN 512

namespace Ming3D
{
    class NetSocketWinsock : public NetSocket
    {
        friend class GameEngine; // !!! TEMP TEST !!!   REMOVE ME

    private:
        SOCKET mSocket = INVALID_SOCKET;
        sockaddr_in mSocketAddress;

        char mDataBuffer[MING3D_DEFAULT_BUFLEN];

    public:
        ~NetSocketWinsock();
        virtual bool Initialise(const char* hostAddress, int remotePort, int localPort) override;
        virtual bool Bind() override;
        virtual bool Listen() override;
        virtual NetSocket* Accept() override;
        virtual bool Connect() override;
        virtual int Recv(char* buffer, int bufferLength) override;
        virtual int Send(char* buffer, int bufferLength) override;
        virtual bool Shutdown() override;
        virtual void PrintSocketInfo() override;
    };
}

#endif
