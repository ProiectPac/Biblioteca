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

void Controller::addBook(std::vector<std::string> message)
{
    if(loggedUser.getUserName() == "")
    {
        const int len = 512;
        char buffer[len]="You are not logged in!";
        client->Send(buffer,len);
        return;
    }
    if(message.size() != 10)
    {
        const int len = 512;
        char buffer[len]="Book was transmitted wrongly!";
        client->Send(buffer,len);
        return;
    }
    Book newBook(0,QString::fromStdString(message[1]),QString::fromStdString(message[2]),QString::fromStdString(message[3]).toInt(),QString::fromStdString(message[4]),QString::fromStdString(message[5]),QString::fromStdString(message[6]).toFloat(),QString::fromStdString(message[7]),QString::fromStdString(message[8]),QString::fromStdString(message[9]).toInt());
    dataBase.addBook(newBook);
    const int len = 512;
    char buffer[len]="Book has been added succesfully!";
    client->Send(buffer,len);

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
    QString buffer = loggedUser.getUserName() + " has deleted his account!";
    buffer.resize(512);
    client->Send(buffer.data(),buffer.size());
    dataBase.removeUser(loggedUser.getUserName());
    loggedUser=User();
}

void Controller::getBook(std::vector<std::string> message)
{
    if(loggedUser.getUserName()=="")
    {
        const int len = 512;
        char buffer[len]="You are not logged in!";
        client->Send(buffer,len);
        return;
    }
    Book searchedBook = dataBase.getBook(QString::fromStdString(message[1]).toInt());
    std::string bookString = QString::number(searchedBook.getID()).toStdString() + " " + searchedBook.getISBN().toStdString() + " " + searchedBook.getAuthor().toStdString() + " " + QString::number(searchedBook.getOriginalPublicationYear()).toStdString() + " " + searchedBook.getTitle().toStdString() + " " + searchedBook.getLanguage().toStdString() + " " + QString::number(searchedBook.getAverageRating()).toStdString() + " " + searchedBook.getImageURL().toStdString() + " " + searchedBook.getSmallImageURL().toStdString() + " " + QString::number(searchedBook.getBooksCount()).toStdString();
    const int len = 512;
    bookString.resize(len);
    client->Send((void*)bookString.c_str(),len);
}

void Controller::getBorrowedBook(std::vector<std::string>message)
{
    if(loggedUser.getUserName()=="")
    {
        const int len = 512;
        char buffer[len]="You are not logged in!";
        client->Send(buffer,len);
        return;
    }
    Book searchedBook = dataBase.getBorrowedBook(QString::fromStdString(message[1]).toInt(),loggedUser.getUserName());
    std::string bookString = QString::number(searchedBook.getID()).toStdString() + " " + searchedBook.getISBN().toStdString() + " " + searchedBook.getAuthor().toStdString() + " " + QString::number(searchedBook.getOriginalPublicationYear()).toStdString() + " " + searchedBook.getTitle().toStdString() + " " + searchedBook.getLanguage().toStdString() + " " + QString::number(searchedBook.getAverageRating()).toStdString() + " " + searchedBook.getImageURL().toStdString() + " " + searchedBook.getSmallImageURL().toStdString() + " " + QString::number(searchedBook.getBooksCount()).toStdString() + " " + QString::number(searchedBook.getRemainingDays()).toStdString();
    const int len = 512;
    bookString.resize(len);
    client->Send((void*)bookString.c_str(),len);
}

void Controller::borrowBook(std::vector<std::string> message)
{
    if(loggedUser.getUserName()=="")
    {
        const int len = 512;
        char buffer[len]="You are not logged in!";
        client->Send(buffer,len);
        return;
    }
    if(message.size()!=2)
    {
        const int len = 512;
        char buffer[len]="Wrong number of parameters!";
        client->Send(buffer,len);
        return;
    }
    auto error = dataBase.borrowBook(loggedUser.getUserName(),QString::fromStdString(message[1]).toInt());
    const int len = 512;
    error.resize(len);
    client->Send((void*) error.c_str(),len);
    return;
}

void Controller::returnBook(std::vector<std::string> message)
{
    if(loggedUser.getUserName()=="")
    {
        const int len = 512;
        char buffer[len]="You are not logged in!";
        client->Send(buffer,len);
        return;
    }
    if(message.size()!=2)
    {
        const int len = 512;
        char buffer[len]="Wrong number of parameters!";
        client->Send(buffer,len);
        return;
    }
    dataBase.returnBook(loggedUser.getUserName(),QString::fromStdString(message[1]).toInt());
    const int len = 512;
    char buffer[len]="Book returned succesfully!";
    client->Send(buffer,len);
    return;

}

void Controller::searchAvailableBooks(std::vector<std::string> message)
{
    if(loggedUser.getUserName()=="")
    {
        const int len = 512;
        char buffer[len]="You are not logged in!";
        client->Send(buffer,len);
        return;
    }

    if(message.size()!=5)
    {
        const int len = 512;
        char buffer[len]="Wrong number of parameters!";
        client->Send(buffer,len);
        return;
    }

    static int lastPage;
    static QString staticName;
    static QString staticAuthor;
    static QString staticISBN;

    if(staticName.toStdString()!=message[1])
    {
        staticName = QString::fromStdString(message[1]);
        lastPage=0;
    }
    if(staticAuthor.toStdString()!=message[2])
    {
        staticAuthor = QString::fromStdString(message[2]);
        lastPage=0;
    }
    if(staticISBN.toStdString()!=message[3])
    {
        staticISBN = QString::fromStdString(message[3]);
        lastPage=0;
    }
    for(int index=lastPage;index<QString::fromStdString(message[4]).toInt();index++)
    {
        dataBase.searchAvailableBooks(QString::fromStdString(message[1]),QString::fromStdString(message[2]),QString::fromStdString(message[3]),index);
    }

    auto availableBooks = dataBase.searchAvailableBooks(QString::fromStdString(message[1]),QString::fromStdString(message[2]),QString::fromStdString(message[3]),QString::fromStdString(message[4]).toInt());

    const int len = 512;
    std::string response = "Found " + QString::number(availableBooks.size()).toStdString() + " books";
    response.resize(len);
    client->Send((void*)response.c_str(),len);

    for(auto& book:availableBooks)
    {
        std::string bookString = QString::number(book.getID()).toStdString() + " " + book.getISBN().toStdString() + " " + book.getAuthor().toStdString() + " " + QString::number(book.getOriginalPublicationYear()).toStdString() + " " + book.getTitle().toStdString() + " " + book.getLanguage().toStdString() + " " + QString::number(book.getAverageRating()).toStdString() + " " + book.getImageURL().toStdString() + " " + book.getSmallImageURL().toStdString() + " " + QString::number(book.getBooksCount()).toStdString();
        const int len = 512;
        bookString.resize(len);
        client->Send((void*)bookString.c_str(),len);
    }

}

void Controller::logOut(std::vector<std::string> message)
{
    if(loggedUser.getUserName()=="")
    {
        const int len = 512;
        char buffer[len]="You are not logged in!";
        client->Send(buffer,len);
        return;
    }
    const int len = 512;
    std::string response ="User " + loggedUser.getUserName().toStdString() + " has logged out.";
    response.resize(len);
    client->Send((void*)response.c_str(),len);

    loggedUser = User();
}
void Controller::removeBook(std::vector<std::string> message)
{
    if(loggedUser.getUserName()=="")
    {
        const int len = 512;
        char buffer[len]="You are not logged in!";
        client->Send(buffer,len);
        return;
    }
    if(dataBase.getBook(QString::fromStdString(message[1]).toInt()).getTitle() == "")
    {
        const int len = 512;
        char buffer[len]="Book doesn't exist";
        client->Send(buffer,len);
        return;
    }
    dataBase.removeBook(QString::fromStdString(message[1]).toInt());
    const int len = 512;
    char buffer[len]="Book has been deleted succesfully!";
    client->Send(buffer,len);

}

void Controller::getAvailableBooks(std::vector<std::string> message)
{
    if(loggedUser.getUserName()=="")
    {
        const int len = 512;
        char buffer[len]="You are not logged in!";
        client->Send(buffer,len);
        return;
    }
    if(message.size()!=2)
    {
        const int len = 512;
        char buffer[len]="Wrong number of parameters!";
        client->Send(buffer,len);
        return;
    }
    auto availabelBooks = dataBase.getAvailableBooks(QString::fromStdString(message[1]).toInt());

    const int len = 512;
    std::string response = "Found " + QString::number(availabelBooks.size()).toStdString() + " books";
    response.resize(len);
    client->Send((void*)response.c_str(),len);

    for(auto& book:availabelBooks)
    {
        std::string bookString = QString::number(book.getID()).toStdString() + " " + book.getISBN().toStdString() + " " + book.getAuthor().toStdString() + " " + QString::number(book.getOriginalPublicationYear()).toStdString() + " " + book.getTitle().toStdString() + " " + book.getLanguage().toStdString() + " " + QString::number(book.getAverageRating()).toStdString() + " " + book.getImageURL().toStdString() + " " + book.getSmallImageURL().toStdString() + " " + QString::number(book.getBooksCount()).toStdString();
        const int len = 512;
        bookString.resize(len);
        client->Send((void*)bookString.c_str(),len);
    }
}

void Controller::getBorrowedBooks(std::vector<std::string> message)
{
    if(loggedUser.getUserName()=="")
    {
        const int len = 512;
        char buffer[len]="You are not logged in!";
        client->Send(buffer,len);
        return;
    }
    if(message.size()!=2)
    {
        const int len = 512;
        char buffer[len]="Wrong number of parameters!";
        client->Send(buffer,len);
        return;
    }
    auto availabelBooks = dataBase.getBorrowedBooks(QString::fromStdString(message[1]).toInt(),loggedUser.getUserName());

    const int len = 512;
    std::string response = "Found " + QString::number(availabelBooks.size()).toStdString() + " books";
    response.resize(len);
    client->Send((void*)response.c_str(),len);

    for(auto& book:availabelBooks)
    {
        std::string bookString = QString::number(book.getID()).toStdString() + " " + book.getISBN().toStdString() + " " + book.getAuthor().toStdString() + " " + QString::number(book.getOriginalPublicationYear()).toStdString() + " " + book.getTitle().toStdString() + " " + book.getLanguage().toStdString() + " " + QString::number(book.getAverageRating()).toStdString() + " " + book.getImageURL().toStdString() + " " + book.getSmallImageURL().toStdString() + " " + QString::number(book.getBooksCount()).toStdString() + " " + QString::number(book.getRemainingDays()).toStdString();
        const int len = 512;
        bookString.resize(len);
        client->Send((void*)bookString.c_str(),len);
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
