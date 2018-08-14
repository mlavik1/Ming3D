#if MING3D_TESTTYPE == 2
#include "net_socket.h"

#include <iostream>

#include "Debug/debug.h"

#ifdef _WIN32
#include "net_socket_winsock.h"
#pragma comment (lib, "Ws2_32.lib")
#endif

#define DEFAULT_BUFLEN 512
#define TEST_PORT_NUMBER 2024

using namespace Ming3D;

NetSocket* CreateSocket()
{
#ifdef _WIN32
    return new NetSocketWinsock();
#else
    return nullptr;
#endif
}

int main()
{
#ifdef _WIN32
    WSADATA wsaData;
    int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0)
    {
        LOG_ERROR() << "WSAStartup failed";
        return 1;
    }
#endif

    int val;
    std::cin >> val;

    if (val == 2)
    {
        NetSocket* listenSocket = CreateSocket();
        listenSocket->Initialise(nullptr, TEST_PORT_NUMBER, 0);

        NetSocket* clientSocket = nullptr;

        char recvbuf[DEFAULT_BUFLEN];
        int recvbuflen = DEFAULT_BUFLEN;

        listenSocket->Bind();

        listenSocket->PrintSocketInfo();

        listenSocket->Listen();
        listenSocket->Accept();

        // Accept a client socket
        while (clientSocket == nullptr)
            clientSocket = listenSocket->Accept();

        // Shutdown listen socket
        delete listenSocket;

        // Receive data from client socket
        while (true)
        {
            int bytesReceived = clientSocket->Recv(recvbuf, recvbuflen);
            if (bytesReceived > 0)
            {
                printf("Bytes received: %d\n", bytesReceived);
                printf(recvbuf);

                // Send a response to the client
                char *sendbuf = "Hello, from server";
                int bytesSent = clientSocket->Send(sendbuf, strlen(sendbuf));
                if (bytesSent <= 0)
                {
                    printf("FAILURE IN SEND!!!");
                    break;
                }
                printf("Bytes sent: %d\n", bytesSent);
            }
        }
        clientSocket->Shutdown();
        delete clientSocket;
    }
    else
    {
        NetSocket* connectSocket = CreateSocket();

        char *sendbuf = "Hello, from client";
        char recvbuf[DEFAULT_BUFLEN];
        int recvbuflen = DEFAULT_BUFLEN;

        // Create a socket for connecting to server
        connectSocket->Initialise("127.0.0.1", TEST_PORT_NUMBER, 0);
        if (connectSocket == nullptr)
            return 1; // TODO: error + cleanup



        bool connected = false;
        while (!connected)
        {
            connected = connectSocket->Connect();
        }

        int bytesSent = connectSocket->Send(sendbuf, strlen(sendbuf));

        printf("Bytes Sent: %ld\n", bytesSent);

        // shutdown the connection since no more data will be sent
        if (!connectSocket->Shutdown())
            return 1; // TODO: error + cleanup

                    // Receive until the peer closes the connection
        while (true)
        {

            int bytesReceived = connectSocket->Recv(recvbuf, recvbuflen);
            if (bytesReceived > 0)
            {
                printf("Bytes received: %d\n", bytesReceived);
                printf(recvbuf);
            }
            else if (bytesReceived == -1)
            {
                printf("Connection closed\n");
                break;
            }
        }

        delete connectSocket;
    }
    
#ifdef _WIN32
    WSACleanup();
#endif
    
    return 0;
}

#endif
