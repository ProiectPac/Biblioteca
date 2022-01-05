#include "controller.h"

Controller::Commands Controller::interpret(const std::string &message)
{
    if(message=="findUser")
        return Commands::FindUser;

    if(message=="addUser")
        return Commands::AddUser;

    if(message=="removeUser")
        return Commands::RemoveUser;

    if(message=="updateUserPassword")
        return Commands::UpdateUserPassword;

    if(message=="addBook")
        return Commands::AddBook;

    if(message=="removeBook")
        return Commands::RemoveBook;

    if(message=="getAvailableBooks")
        return Commands::GetAvailableBooks;

    if(message=="getBorrowedBooks")
        return Commands::GetBorrowedBooks;

    if(message=="getBook")
        return Commands::GetBook;

    if(message=="getBorrowedBook")
        return Commands::GetBorrowedBook;

    if(message=="borrowBook")
        return Commands::BorrowBook;

    if(message=="returnBook")
        return Commands::ReturnBook;

    if(message=="searchAvailableBooks")
        return Commands::SearchAvailableBooks;

    if(message=="searchBorrowedBooks")
        return Commands::SearchBorrowedBooks;

    if(message=="logOut")
        return Commands::LogOut;

    return Commands::None;
}

void Controller::findUser(std::vector<std::string> message)
{
    if(message.size()==1)
    {
        const int len = 512;
        char buffer[len]="Username/Password can't be empty";
        client->Send(buffer,len);
        return;
    }

    if(message.size()==2)
    {
        const int len = 512;
        char buffer[len]="Password can't be empty";
        client->Send(buffer,len);
        return;
    }

    if(message.size()==3)
    {
        auto user = dataBase.findUser(QString::fromStdString(message[1]),QString::fromStdString(message[2]).toUInt());
        if(user.getUserName()=="")
        {
            const int len = 512;
            char buffer[len]="UserName/Password combination is not correct!";
            client->Send(buffer,len);
        }
        else
        {
            const int len = 512;
            char buffer[len]="User found";
            client->Send(buffer,len);
        }
    }
    else
    {
        const int len = 512;
        char buffer[len]="Too many parameters";
        client->Send(buffer,len);
    }
}

void Controller::receiveComand()
{
    std::vector<std::string>message;
    const int length = 512;
    char buffer[length];
    client->Receive(buffer,length);
    char *pointer = std::strtok(buffer," ");

    while(pointer)
    {
        std::string word(pointer);
        message.push_back(word);
        pointer=std::strtok(NULL, " ");
    }

    switch(interpret(message[0]))
    {
    case Controller::Commands::FindUser:
        findUser(message);
        break;

    case Controller::Commands::AddUser:
        break;

    case Controller::Commands::RemoveUser:
        break;

    case Controller::Commands::UpdateUserPassword:
        break;

    case Controller::Commands::AddBook:
        break;

    case Controller::Commands::RemoveBook:
        break;

    case Controller::Commands::GetAvailableBooks:
        break;

    case Controller::Commands::GetBorrowedBooks:
        break;

    case Controller::Commands::GetBook:
        break;

    case Controller::Commands::GetBorrowedBook:
        break;

    case Controller::Commands::BorrowBook:
        break;

    case Controller::Commands::ReturnBook:
        break;

    case Controller::Commands::SearchAvailableBooks:
        break;

    case Controller::Commands::SearchBorrowedBooks:
        break;

    case Controller::Commands::LogOut:
        break;

    case Controller::Commands::None:
        const int len = 512;
        char buffer[len]="Unkown command";
        client->Send(buffer,len);
        break;
    }
    message.clear();
}

void Controller::run(SOCKET client)
{
    Controller controller;
    controller.client = new TCPSocket(client);
    while(true)
    {
        controller.receiveComand();
    }
}
