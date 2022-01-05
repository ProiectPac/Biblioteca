#pragma once
#include <iostream>
#include <string>
#include <winsock2.h>
#include <ws2tcpip.h>

class TCPSocket
{
public:
    TCPSocket();
    TCPSocket(SOCKET socket);
    ~TCPSocket();
    void ConnectToServer(const std::string&, uint16_t);
    void Listen(uint16_t);
    void Send(void* data, uint32_t size);
    void Receive(void* data, uint32_t size);
    SOCKET Accept();
    void static startUp();
private:
    SOCKET connectSocket;
};
