#ifdef _WIN32

#include "net_socket_winsock.h"

#include "Debug/debug.h"

namespace Ming3D
{
    NetSocketWinsock::~NetSocketWinsock()
    {
        if (mSocket != INVALID_SOCKET)
        {
            closesocket(mSocket);
        }
    }

    bool NetSocketWinsock::Initialise(const char* hostAddress, int remotePort, int localPort)
    {
        mSocketAddress.sin_family = AF_INET;
        mSocketAddress.sin_addr.s_addr = hostAddress == nullptr ? 0 : inet_addr(hostAddress); // "127.0.0.1"
        mSocketAddress.sin_port = htons(remotePort); // set to 0 for random available port

        mSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        if (mSocket == INVALID_SOCKET)
        {
            LOG_ERROR() << "Failed to create socket to " << hostAddress << " at port " << remotePort;
            return false;
        }

        u_long mode = 1;  // 1 to enable non-blocking socket
        ioctlsocket(mSocket, FIONBIO, &mode);

        return true;
    }

    bool NetSocketWinsock::Bind()
    {
        int iResult = bind(mSocket, (SOCKADDR*)&mSocketAddress, sizeof(mSocketAddress));
        if (iResult == SOCKET_ERROR)
        {
            LOG_ERROR() << "Failed to bind socket";
            return false;
        }
        return true;
    }

    bool NetSocketWinsock::Listen()
    {
        int iResult = listen(mSocket, SOMAXCONN);
        if (iResult == SOCKET_ERROR)
        {
            return false;
        }
        return true;
    }

    NetSocket* NetSocketWinsock::Accept()
    {
        SOCKET clientSocket = accept(mSocket, NULL, NULL);
        if (clientSocket != INVALID_SOCKET)
        {
            NetSocketWinsock* newSocket = new NetSocketWinsock();
            newSocket->mSocket = clientSocket;
            // TODO: Set mSocketAddress
            return newSocket;
        }
        return nullptr;
    }

    bool NetSocketWinsock::Connect() // TODO: return ESocketConnectResult
    {
        int result = connect(mSocket, (sockaddr*)&mSocketAddress, sizeof(mSocketAddress));
        return !(result == SOCKET_ERROR && WSAGetLastError() == WSAEWOULDBLOCK); // TEMP HACK - FIX ME
        // TODO: Return an enum indicating whether connection is in progress, succeeded or failed
    }

    int NetSocketWinsock::Recv(char* buffer, int bufferLength)
    {
        const int result = recv(mSocket, buffer, bufferLength, 0);
        if (result == SOCKET_ERROR) // received nothing
            return 0;
        else if (result == 0) // connection closed
            return -1;
        return result;
    }
    
    int NetSocketWinsock::Send(const char* buffer, int bufferLength)
    {
        return send(mSocket, buffer, bufferLength, 0);
    }

    bool NetSocketWinsock::Shutdown()
    {
        return shutdown(mSocket, SD_SEND) != SOCKET_ERROR;
    }

    void NetSocketWinsock::PrintSocketInfo()
    {
        // Print the socket port number
        struct sockaddr_in sin;
        int addrlen = sizeof(sin);
        if (getsockname(mSocket, (struct sockaddr*)&sin, &addrlen) == 0 &&
        sin.sin_family == AF_INET &&
        addrlen == sizeof(sin))
        {
            int local_port = ntohs(sin.sin_port);
            printf("Listen socket port: %d\n", local_port);
        }
    }

    int NetSocketWinsock::GetLocalPort()
    {
        struct sockaddr_in sin;
        int addrlen = sizeof(sin);
        if (getsockname(mSocket, (struct sockaddr *)&sin, &addrlen) == 0 && sin.sin_family == AF_INET && addrlen == sizeof(sin))
        {
            return ntohs(sin.sin_port);
        }
        return -1;
    }
}

#endif
