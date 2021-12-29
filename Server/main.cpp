#include <QCoreApplication>
#include "tcpsocket.h"
#include "sqldatabase.h"
#include "book.h"
#include "user.h"
#include <string.h>


int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    TCPSocket connectionsSocket;
    connectionsSocket.Listen(27015);
    auto client = connectionsSocket.Accept();
    std::vector<std::string>message;
    while(true)
    {
        const int length = 512;
        char buffer[length];
        client.Receive(buffer,length);
        char *pointer = std::strtok(buffer," ");
        while(pointer)
        {
            std::string word(pointer);
            message.push_back(word);
            pointer=std::strtok(NULL, " ");

        }
    }
    return a.exec();
}
