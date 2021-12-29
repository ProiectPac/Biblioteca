#pragma once
#include <iostream>
#include <string>
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "Ws2_32.lib")

class TCPSocket
{
public:
    TCPSocket();
    TCPSocket(SOCKET socket);
    ~TCPSocket();
    void ConnectToServer(const std::string&, uint16_t);
private:
    SOCKET connectSocket;
};
