#ifndef MING3D_NETSOCKET_H
#define MING3D_NETSOCKET_H

#include "net_message.h"

namespace Ming3D
{
    /**
    * Base class for sockets.
    */
    class NetSocket
    {
    public:
        virtual bool Initialise(const char* hostAddress, int remotePort, int localPort) = 0;
        virtual bool Bind() = 0;
        virtual bool Listen() = 0;
        virtual NetSocket* Accept() = 0;
        virtual bool Connect() = 0;
        virtual int Recv(char* buffer, int bufferLength) = 0;
        virtual int Send(const char* buffer, int bufferLength) = 0;
        virtual bool Shutdown() = 0;
        virtual void PrintSocketInfo() = 0;
        virtual int GetLocalPort() = 0;
    };
}

#endif
