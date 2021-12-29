#include "TCPSocket.h"
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "Ws2_32.lib")

TCPSocket::TCPSocket()
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

TCPSocket::TCPSocket(SOCKET socket)
{
    this->connectSocket = socket;
}

TCPSocket::~TCPSocket()
{
    closesocket(connectSocket);
}
