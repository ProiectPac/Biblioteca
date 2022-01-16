#include "pch.h"
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
        throw std::exception("Error at socket: " + WSAGetLastError());
    }
}

TCPSocket::TCPSocket(SOCKET socket)
{
    this->connectSocket = socket;
}

TCPSocket::~TCPSocket()
{
    closesocket(connectSocket);
    EVP_PKEY_free(localKey);
    EVP_PKEY_free(remoteKey);
}

void TCPSocket::ConnectToServer(const std::string& address, uint16_t port)
{
    addrinfo* result = NULL, hints;

    ZeroMemory(&hints, sizeof(hints));	// memset to 0
    hints.ai_family = AF_INET;

    // *** Resolve the server address and port (can be also names like "localhost") ***
    int iResult = getaddrinfo(address.c_str(), std::to_string(port).c_str(), &hints, &result);
    if (iResult != 0)
    {
        throw std::exception("getaddrinfo failed: " + iResult);
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
        throw std::exception("Unable to connect to server!\n");
    }
    char serverPublicKeyBuffer[PUBLIC_KEY_SIZE];

    int iRecvResult = recv(connectSocket, serverPublicKeyBuffer, PUBLIC_KEY_SIZE, 0);
    if (iRecvResult < 0)
    {
        throw std::exception("receive failed: " + WSAGetLastError());
    };

    remoteKey = createPublicKey(std::string(serverPublicKeyBuffer));

    localKey = generate_rsa_key(KEY_SIZE);

    auto sessionPublicKey = TCPSocket::extractPublicKey(localKey);

    int iSendResult = send(connectSocket, sessionPublicKey.c_str(), sessionPublicKey.size(), 0);
    if (iSendResult < 0)
    {
        throw std::exception("send failed: " + WSAGetLastError());
    }
}

void TCPSocket::Listen(std::string address,uint16_t port)
{
    struct addrinfo* result = nullptr, * ptr = nullptr, hints;

    ZeroMemory(&hints, sizeof(hints));	// memset to 0
    hints.ai_family = AF_INET;			// IPv4

    // Resolve the local address and port to be used by the server
    int iResult = getaddrinfo(address.c_str(), std::to_string(port).c_str(), &hints, &result);
    if (iResult != 0)
    {
        throw std::exception("getaddrinfo failed: " + iResult);
    }

    // *** Binding the socket ***
    // Setup the TCP listening socket
    iResult = bind(connectSocket, result->ai_addr, (int)result->ai_addrlen);
    if (iResult == SOCKET_ERROR)
    {
        throw std::exception("bind failed with error: " + WSAGetLastError());

        freeaddrinfo(result);
        closesocket(connectSocket);
    }
    freeaddrinfo(result);

    // *** Listening on the socket ***
    if (listen(connectSocket, SOMAXCONN) == SOCKET_ERROR)
    {
        
        throw std::exception("Listen failed with error: " + WSAGetLastError());

        closesocket(connectSocket);
    }
    else
    {
        localKey = TCPSocket::generate_rsa_key(KEY_SIZE);
    }
}

void TCPSocket::Send(const std::string& message)
{
    std::string encryptedMessage;

    for(int index=0;index<message.size()/SLICE_SIZE + 1;index++)
    {
        std::string messageSlice = message.substr(index * SLICE_SIZE,SLICE_SIZE);
        encryptedMessage += TCPSocket::encrypt(remoteKey, messageSlice);
    }

    int iSendResult = send(connectSocket, encryptedMessage.c_str(), encryptedMessage.size(), 0);
    if (iSendResult < 0)
    {
        throw std::exception("send failed: " + WSAGetLastError());
    }
}

std::string TCPSocket::Receive()
{   
    std::string messageStub;
    messageStub.resize(RECEIVE_MESSAGE_SIZE);

    int iRecvResult = recv(connectSocket, (char*)messageStub.c_str(), messageStub.size(), 0);
    if (iRecvResult < 0)
    {
        throw std::exception("receive failed: " + WSAGetLastError());
    }
    else
    {
        std::string decryptedMessage = TCPSocket::decrypt(localKey, messageStub);        

        //Check if there are things left to read
        FD_SET set;
        FD_ZERO(&set);
        FD_SET(connectSocket,&set);
        TIMEVAL timeOut ={0,0};

        select(0,&set,nullptr,nullptr,&timeOut);

        while(FD_ISSET(connectSocket,&set))
        {
            messageStub.clear();
            messageStub.resize(RECEIVE_MESSAGE_SIZE);

            int iRecvResult = recv(connectSocket, (char*)messageStub.c_str(), messageStub.size(), 0);
            if (iRecvResult < 0)
            {
                throw std::exception("receive failed: " + WSAGetLastError());
            }

            messageStub.resize(iRecvResult);
            messageStub = decrypt(localKey,messageStub);

            if (messageStub[messageStub.size() - 1] == ';')
            {
                 messageStub.resize(messageStub.size() - 1);
                decryptedMessage += messageStub;
               break;
            }
            decryptedMessage+=messageStub;

            FD_ZERO(&set);
            FD_SET(connectSocket,&set);
            select(0,&set,nullptr,nullptr,&timeOut);
        }

        if (decryptedMessage[decryptedMessage.size() - 1] == ';')
        {
            decryptedMessage.resize(decryptedMessage.size() - 1);
        }

        return decryptedMessage;
    }
}

std::pair<SOCKET,std::string> TCPSocket::Accept()
{
    SOCKET ClientSocket = INVALID_SOCKET;
    ClientSocket = accept(connectSocket, NULL, NULL);
    if (ClientSocket == INVALID_SOCKET)
    {
        throw std::exception("accept failed: " + WSAGetLastError());
    }

    auto publicKeyBuffer = TCPSocket::extractPublicKey(localKey);

    int iSendResult = send(ClientSocket, publicKeyBuffer.c_str(), publicKeyBuffer.size(), 0);

    if (iSendResult < 0)
    {
        throw std::exception("send failed: " + WSAGetLastError());
    }

    unsigned char sessionPublicKeyBuffer[PUBLIC_KEY_SIZE + 1];

    int iRecvResult = recv(ClientSocket, (char*)sessionPublicKeyBuffer, PUBLIC_KEY_SIZE, 0);
    if (iRecvResult < 0)
    {
        throw std::exception("receive failed: " + WSAGetLastError());
    };

    sessionPublicKeyBuffer[PUBLIC_KEY_SIZE] = '\0';

    return std::make_pair(ClientSocket,std::string((char*)sessionPublicKeyBuffer));
}

void TCPSocket::startUp()
{
    WSADATA wsaData;

    int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0) {
        printf("WSAStartup failed: %d\n", iResult);
    }
}


EVP_PKEY* TCPSocket::generate_rsa_key(int bits)
{
    EVP_PKEY_CTX* ctx = EVP_PKEY_CTX_new_id(EVP_PKEY_RSA, NULL);
    EVP_PKEY* key = nullptr;

    // initialize generator
    EVP_PKEY_keygen_init(ctx);
    EVP_PKEY_CTX_set_rsa_keygen_bits(ctx, bits);

    // generate the key pair
    EVP_PKEY_keygen(ctx, &key);

    EVP_PKEY_CTX_free(ctx);
    return key;
}

std::string TCPSocket::encrypt(EVP_PKEY* key,std::string message)
{
    EVP_PKEY_CTX* ctx = EVP_PKEY_CTX_new(key, NULL);
    if (!ctx)
        throw std::exception("Error");


    if (EVP_PKEY_encrypt_init(ctx) <= 0)
        throw std::exception("Error");
    if (EVP_PKEY_CTX_set_rsa_padding(ctx, RSA_PKCS1_OAEP_PADDING) <= 0)
        throw std::exception("Error");

    size_t outlen;
    if (EVP_PKEY_encrypt(ctx, NULL, &outlen, (unsigned char*)message.c_str(),message.size()) <= 0)
        throw std::exception("Error");

    unsigned char *out = (unsigned char*)OPENSSL_malloc(outlen);

    if (!out)
        throw std::exception("Error");

    if (EVP_PKEY_encrypt(ctx, out, &outlen, (unsigned char*)message.c_str(),message.size()) <= 0)
        throw std::exception("Error");

    std::string encryptedMessage;
    encryptedMessage.resize(outlen);

    for (int index = 0; index < outlen; index++)
    {
        encryptedMessage[index] = out[index];
    }

    OPENSSL_free(out);

    return encryptedMessage;
}

std::string TCPSocket::decrypt(EVP_PKEY* key, std::string message)
{
    EVP_PKEY_CTX* ctx = EVP_PKEY_CTX_new(key, NULL);
    if (!ctx)
        throw std::exception("Error");
    if (EVP_PKEY_decrypt_init(ctx) <= 0)
        throw std::exception("Error");
    if (EVP_PKEY_CTX_set_rsa_padding(ctx, RSA_PKCS1_OAEP_PADDING) <= 0)
        throw std::exception("Error");

    /* Determine buffer length */
    size_t outlen;

    if (EVP_PKEY_decrypt(ctx, NULL, &outlen, (unsigned char*)message.c_str(),message.size()) <= 0)
        throw std::exception("Error");

    unsigned char *out = (unsigned char*)OPENSSL_malloc(outlen);

    if (!out)
        throw std::exception("Error");

    if (EVP_PKEY_decrypt(ctx, out, &outlen, (unsigned char*)message.c_str(),message.size()) <= 0)
        throw std::exception("Error");

    std::string decryptedMessage;
    decryptedMessage.resize(outlen);

    for (int index = 0; index < outlen; index++)
    {
        decryptedMessage[index] = out[index];
    }

    OPENSSL_free(out);

    return decryptedMessage;
}

std::string TCPSocket::extractPublicKey(EVP_PKEY* key)
{
    BIO* mem = BIO_new(BIO_s_mem());

    PEM_write_bio_PUBKEY(mem, key);

    BUF_MEM* bptr;

    BIO_get_mem_ptr(mem, &bptr);

    std::string publicKey(bptr->data);
    publicKey.resize(bptr->length);

    BIO_free(mem);

    return publicKey;
}

EVP_PKEY* TCPSocket::createPublicKey(std::string publicKey)
{
    BIO* mem = BIO_new(BIO_s_mem());
    BIO_write(mem, publicKey.c_str(), publicKey.size());

    auto key = PEM_read_bio_PUBKEY(mem, NULL, NULL, NULL);
    BIO_free(mem);

    return key;
}

void TCPSocket::setRemoteKey(EVP_PKEY *newRemoteKey)
{
    remoteKey = newRemoteKey;
}

void TCPSocket::setLocalKey(EVP_PKEY *newLocalKey)
{
    localKey = newLocalKey;
}

EVP_PKEY *TCPSocket::getRemoteKey() const
{
    return remoteKey;
}

EVP_PKEY *TCPSocket::getLocalKey() const
{
    return localKey;
}

