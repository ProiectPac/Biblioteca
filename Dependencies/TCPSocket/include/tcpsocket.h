#pragma once
#include <iostream>
#include <string>
#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "Ws2_32.lib")
#pragma comment(lib, "libcrypto.lib")
#pragma comment(lib, "libssl.lib")

#include <openssl/bio.h>
#include <openssl/rsa.h>
#include <openssl/crypto.h>
#include <openssl/pem.h>

constexpr int KEY_SIZE = 3072;
constexpr int RECEIVE_MESSAGE_SIZE = KEY_SIZE / 8;
constexpr int PUBLIC_KEY_SIZE = 625;
constexpr int SLICE_SIZE = 128;

class TCPSocket
{
public:
    TCPSocket();
    TCPSocket(SOCKET socket);
    ~TCPSocket();
    void ConnectToServer(const std::string&, uint16_t);
    void Listen(std::string address, uint16_t port);
    void Send(const std::string& message);
    std::string Receive();
    std::pair<SOCKET,std::string> Accept();
    void static startUp();
    static EVP_PKEY* generate_rsa_key(int bits);
    static std::string encrypt(EVP_PKEY* key, std::string message);
    static std::string decrypt(EVP_PKEY* key, std::string message);
    static std::string extractPublicKey(EVP_PKEY* key);
    static EVP_PKEY* createPublicKey(std::string publicKey);
    void setRemoteKey(EVP_PKEY *newRemoteKey);
    void setLocalKey(EVP_PKEY *newLocalKey);
    EVP_PKEY *getRemoteKey() const;
    EVP_PKEY *getLocalKey() const;
private:
    SOCKET connectSocket;
    EVP_PKEY* remoteKey;
    EVP_PKEY* localKey;
};
