#include <QCoreApplication>
#include "tcpsocket.h"
#include "sqldatabase.h"
#include "book.h"
#include "user.h"
#include <string.h>
#include "controller.h"
#include <QString>
#include <thread>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    TCPSocket::startUp();
    TCPSocket connectionsSocket;    
    connectionsSocket.Listen(27015);
    while(true)
    {       
       std::thread clientThread(Controller::run,connectionsSocket.Accept());
       clientThread.detach();
    }
    return a.exec();
}
