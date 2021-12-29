#include "TCPSocket.h"
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "Ws2_32.lib")

TCPSockets::TCPSockets()
{
    WSADATA wsaData;

    int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0) {
        printf("WSAStartup failed: %d\n", iResult);
    }
    // *** Create a socket ***
    connectSocket = INVALID_SOCKET;

    // Create a SOCKET for connecting to server: IPv4, stream socket, TCP
    connectSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    if (connectSocket == INVALID_SOCKET)
    {
        std::cerr << "Error at socket: " << WSAGetLastError();
    }
}

TCPSockets::TCPSockets(SOCKET socket)
{
    this->connectSocket = socket;
}

TCPSockets::~TCPSockets()
{
    closesocket(connectSocket);
}

void TCPSockets::ConnectToServer(const std::string& address, uint16_t port)
{
    addrinfo* result = NULL, hints;

    ZeroMemory(&hints, sizeof(hints));    // memset to 0
    hints.ai_family = AF_INET;

    // *** Resolve the server address and port (can be also names like "localhost") ***
    int iResult = getaddrinfo(address.c_str(), std::to_string(port).c_str(), &hints, &result);
    if (iResult != 0)
    {
        std::cerr << "getaddrinfo failed: " << iResult;
        freeaddrinfo(result);
    }

    // Attempt to connect to the first address returned by the call to getaddrinfo
    iResult = connect(connectSocket, result->ai_addr, (int)result->ai_addrlen);
    if (iResult == SOCKET_ERROR)
    {
        closesocket(connectSocket);
        connectSocket = INVALID_SOCKET;
    }

    freeaddrinfo(result);

    if (connectSocket == INVALID_SOCKET)
    {
        std::cerr << "Unable to connect to server!\n";
    }
}
