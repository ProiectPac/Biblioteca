#include "controller.h"

Controller::Commands Controller::interpret(const std::string &message)
{
    if(message=="login")
        return Commands::Login;

    if(message=="registerAccount")
        return Commands::RegisterAccount;

    if(message=="deleteAccount")
        return Commands::DeleteAccount;

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

void Controller::login(std::vector<std::string> message)
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
            loggedUser = user;
        }
    }
    else
    {
        const int len = 512;
        char buffer[len]="Too many parameters";
        client->Send(buffer,len);
    }
}

void Controller::registerAccount(std::vector<std::string> message)
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
        auto user = dataBase.findUser(QString::fromStdString(message[1]));
        if(user.getUserName()=="")
        {
            user.setUserName(QString::fromStdString(message[1]));
            user.setPasswordHash(QString::fromStdString(message[2]).toUInt());
            dataBase.addUser(user);
            loggedUser = user;
            const int len = 512;
            char buffer[len]="Registration went succesfuly";
            client->Send(buffer,len);
        }
        else
        {
            const int len = 512;
            char buffer[len]="Username must be unique. Try another one!";
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

void Controller::updateUserPassword(std::vector<std::string> message)
{
    if(loggedUser.getUserName()=="")
    {
        const int len = 512;
        char buffer[len]="You are not logged in!";
        client->Send(buffer,len);
        return;
    }
    else
    {
        if(message.size()==1)
        {
            const int len = 512;
            char buffer[len]="Password can't be empty";
            client->Send(buffer,len);
            return;
        }
        else if(message.size()==2)
        {
            loggedUser.setPasswordHash(QString::fromStdString(message[2]).toUInt());
            QString buffer = loggedUser.getUserName() + " has changed password succesfully";
            buffer.resize(512);
            client->Send(buffer.data(),buffer.size());
        }
        else
        {
            const int len = 512;
            char buffer[len]="Too many parameters";
            client->Send(buffer,len);
        }
    }
}

void Controller::deleteAccount(std::vector<std::string> message)
{
    if(loggedUser.getUserName()=="")
    {
        const int len = 512;
        char buffer[len]="You are not logged in!";
        client->Send(buffer,len);
        return;
    }
    else
    {
        QString buffer = loggedUser.getUserName() + " has deleted his account!";
        buffer.resize(512);
        client->Send(buffer.data(),buffer.size());
        dataBase.removeUser(loggedUser.getUserName());
        loggedUser=User();
        return;
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
    case Controller::Commands::Login:
        login(message);
        break;

    case Controller::Commands::RegisterAccount:
        registerAccount(message);
        break;

    case Controller::Commands::DeleteAccount:
        deleteAccount(message);
        break;

    case Controller::Commands::UpdateUserPassword:
        updateUserPassword(message);
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
