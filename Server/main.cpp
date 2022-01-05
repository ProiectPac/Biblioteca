#include <QCoreApplication>
#include "tcpsocket.h"
#include "sqldatabase.h"
#include "book.h"
#include "user.h"
#include <string.h>
#include "controler.h"
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
       std::thread clientThread(Controler::run,connectionsSocket.Accept());
       clientThread.detach();
    }
    return a.exec();
}
