#include <QCoreApplication>
#include "tcpsocket.h"
#include "sqldatabase.h"
#include "book.h"
#include "user.h"
#include <string.h>
#include "messageinterpreter.h"
#include <QString>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    TCPSocket connectionsSocket;
    SQLDataBase dataBase;
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
        MessageInterpreter messageInterpreter;
        switch(messageInterpreter.interpret(message[0]))
        {
        case MessageInterpreter::Commands::FindUser:
            if(message[1]==""&&message[2]=="")
            {
                const int len = 512;
                char buffer[len]="Username/Password can't be empty";
                client.Send(buffer,len);
            }
            else if(message[1]=="")
            {
                const int len = 512;
                char buffer[len]="Username can't be empty";
                client.Send(buffer,len);
            }
            else if(message[2]=="")
            {
                const int len = 512;
                char buffer[len]="Password can't be empty";
                client.Send(buffer,len);
            }
            else
            {
                auto user = dataBase.findUser(QString::fromStdString(message[1]),QString::fromStdString(message[2]).toUInt());
                if(user.getUserName()=="")
                {
                    const int len = 512;
                    char buffer[len]="UserName/Password combination is not correct!";
                    client.Send(buffer,len);
                }
                else
                {
                    const int len = 512;
                    char buffer[len]="User found";
                    client.Send(buffer,len);
                }
            }
            break;

        case MessageInterpreter::Commands::AddUser:
            break;

        case MessageInterpreter::Commands::RemoveUser:
            break;

        case MessageInterpreter::Commands::UpdateUserPassword:
            break;

        case MessageInterpreter::Commands::AddBook:
            break;

        case MessageInterpreter::Commands::RemoveBook:
            break;

        case MessageInterpreter::Commands::GetAvailableBooks:
            break;

        case MessageInterpreter::Commands::GetBorrowedBooks:
            break;

        case MessageInterpreter::Commands::GetBook:
            break;

        case MessageInterpreter::Commands::GetBorrowedBook:
            break;

        case MessageInterpreter::Commands::BorrowBook:
            break;

        case MessageInterpreter::Commands::ReturnBook:
            break;

        case MessageInterpreter::Commands::SearchAvailableBooks:
            break;

        case MessageInterpreter::Commands::SearchBorrowedBooks:
            break;

        case MessageInterpreter::Commands::LogOut:
            break;

        case MessageInterpreter::Commands::None:
            break;
        }
    }
    return a.exec();
}
