#pragma once
#include <iostream>
#include <string>
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "Ws2_32.lib")

class TCPSockets
{
public:
    TCPSockets();
    TCPSockets(SOCKET socket);
    ~TCPSockets();
    void ConnectToServer(const std::string&, uint16_t);
private:
    SOCKET connectSocket;
};
