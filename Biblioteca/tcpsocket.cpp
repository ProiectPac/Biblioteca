#include "tcpsocket.h"
#include <winsock2.h>
#include <ws2tcpip.h>

TCPSocket::TCPSocket()
{
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



void TCPSocket::ConnectToServer(const std::string& address, uint16_t port)
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

void TCPSocket::Listen(uint16_t port)
{
    struct addrinfo* result = nullptr, hints;

    ZeroMemory(&hints, sizeof(hints));    // memset to 0
    hints.ai_family = AF_INET;            // IPv4

    // Resolve the local address and port to be used by the server
    int iResult = getaddrinfo(NULL, std::to_string(port).c_str(), &hints, &result);
    if (iResult != 0)
    {
        std::cerr << "getaddrinfo failed: " << iResult;
    }

    // *** Binding the socket ***
    // Setup the TCP listening socket
    iResult = bind(connectSocket, result->ai_addr, (int)result->ai_addrlen);
    if (iResult == SOCKET_ERROR)
    {
        std::cerr << "bind failed with error: " << WSAGetLastError();
        freeaddrinfo(result);
        closesocket(connectSocket);
    }
    freeaddrinfo(result);

    // *** Listening on the socket ***
    if (listen(connectSocket, SOMAXCONN) == SOCKET_ERROR)
    {
        std::cerr << "Listen failed with error: " << WSAGetLastError();
        closesocket(connectSocket);
    }
}

void TCPSocket::Send(void *data, uint32_t size)
{
    int iSendResult = send(connectSocket, (char*)data, size, 0);
    if (iSendResult < 0)
    {
        std::cerr << "send failed: " << WSAGetLastError();
    }
}

void TCPSocket::Receive(void *data, uint32_t size)
{
    int iRecvResult = recv(connectSocket, (char*)data, size, 0);
    if (iRecvResult < 0)
    {
        std::cerr << "receive failed: " << WSAGetLastError();
    }
}

SOCKET TCPSocket::Accept()
{
    SOCKET ClientSocket = INVALID_SOCKET;
    ClientSocket = accept(connectSocket, NULL, NULL);
    if (ClientSocket == INVALID_SOCKET)
    {
        std::cerr << "accept failed: " << WSAGetLastError();
    }
    return ClientSocket;
}

void TCPSocket::startUp()
{
    WSADATA wsaData;

    int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0) {
        printf("WSAStartup failed: %d\n", iResult);
    }
}
