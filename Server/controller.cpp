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

std::vector<std::string> Controller::parametrize(const std::string &message)
{
    std::vector<std::string> result;

    int lastPosition = 0;
    int position = message.find(' ');

    while(position !=std::string::npos)
    {
        result.push_back(message.substr(lastPosition,position - lastPosition));
        lastPosition = position + 1;
        position = message.find(' ',lastPosition);
    }

    result.push_back(message.substr(lastPosition,message.size() - lastPosition));

    return result;
}

void Controller::login(std::vector<std::string> message)
{
    if(message.size()==1)
    {
        client->Send("Username/Password can't be empty");
        return;
    }

    if(message.size()==2)
    {
        client->Send("Password can't be empty");
        return;
    }

    if(message.size()==3)
    {
        auto user = dataBase.findUser(QString::fromStdString(message[1]),QString::fromStdString(message[2]).toUInt());
        if(user.getUserName()=="")
        {
            client->Send("UserName/Password combination is not correct!");
        }
        else
        {
            client->Send("Logged in succesfully");
            loggedUser = user;
        }
    }
    else
    {
        client->Send("Too many parameters");
    }
}

void Controller::registerAccount(std::vector<std::string> message)
{
    if(message.size()==1)
    {
        client->Send("Username/Password can't be empty");
        return;
    }

    if(message.size()==2)
    {
        client->Send("Password can't be empty");
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
            client->Send("Registration went succesfuly");
        }
        else
        {
            client->Send("Username must be unique. Try another one!");
        }
    }
    else
    {
        client->Send("Too many parameters");
    }
}

void Controller::updateUserPassword(std::vector<std::string> message)
{
    if(loggedUser.getUserName()=="")
    {
        client->Send("You are not logged in!");
        return;
    }
    else
    {
        if(message.size()==1)
        {
            client->Send("Password can't be empty");
            return;
        }
        else if(message.size()==2)
        {
            loggedUser.setPasswordHash(QString::fromStdString(message[1]).toUInt());
            dataBase.updateUserPassword(loggedUser.getUserName(),QString::fromStdString(message[1]).toUInt());
            client->Send(loggedUser.getUserName().toStdString() + " has changed password succesfully");
        }
        else
        {
            client->Send("Too many parameters");
        }
    }
}

void Controller::addBook(std::vector<std::string> message)
{
    if(loggedUser.getUserName() == "")
    {
        client->Send("You are not logged in!");
        return;
    }
    if(message.size() != 10)
    {
        client->Send("Book was transmitted wrongly!");
        return;
    }
    Book newBook(0,QString::fromStdString(message[1]),QString::fromStdString(message[2]),QString::fromStdString(message[3]).toInt(),QString::fromStdString(message[4]),QString::fromStdString(message[5]),QString::fromStdString(message[6]).toFloat(),QString::fromStdString(message[7]),QString::fromStdString(message[8]),QString::fromStdString(message[9]).toInt());
    dataBase.addBook(newBook);
    client->Send("Book has been added succesfully!");

}

void Controller::deleteAccount(std::vector<std::string> message)
{
    if(loggedUser.getUserName()=="")
    {
        client->Send("You are not logged in!");
        return;
    }
    client->Send( loggedUser.getUserName().toStdString() + " has deleted his account!");
    dataBase.removeUser(loggedUser.getUserName());
    loggedUser=User();
}

void Controller::getBook(std::vector<std::string> message)
{
    if(loggedUser.getUserName()=="")
    {
        client->Send("You are not logged in!");
        return;
    }

    if(message.size()!=2)
    {
        client->Send("Wrong number of parameters!");
        return;
    }

    Book searchedBook = dataBase.getBook(QString::fromStdString(message[1]).toInt());

    if(searchedBook.getTitle()=="")
    {
        client->Send("The book does not exist!");
        return;
    }
    std::string bookString = QString::number(searchedBook.getID()).toStdString() + " " + searchedBook.getISBN().toStdString() + " " + searchedBook.getAuthor().toStdString() + " " + QString::number(searchedBook.getOriginalPublicationYear()).toStdString() + " " + searchedBook.getTitle().toStdString() + " " + searchedBook.getLanguage().toStdString() + " " + QString::number(searchedBook.getAverageRating()).toStdString() + " " + searchedBook.getImageURL().toStdString() + " " + searchedBook.getSmallImageURL().toStdString() + " " + QString::number(searchedBook.getBooksCount()).toStdString();
    client->Send(bookString);
}

void Controller::getBorrowedBook(std::vector<std::string>message)
{
    if(loggedUser.getUserName()=="")
    {
        client->Send("You are not logged in!");
        return;
    }

    if(message.size()!=2)
    {
        client->Send("Wrong number of parameters!");
        return;
    }

    Book searchedBook = dataBase.getBorrowedBook(QString::fromStdString(message[1]).toInt(),loggedUser.getUserName());

    if(searchedBook.getTitle()=="")
    {
        client->Send("The book does not exist!");
        return;
    }
    std::string bookString = QString::number(searchedBook.getID()).toStdString() + " " + searchedBook.getISBN().toStdString() + " " + searchedBook.getAuthor().toStdString() + " " + QString::number(searchedBook.getOriginalPublicationYear()).toStdString() + " " + searchedBook.getTitle().toStdString() + " " + searchedBook.getLanguage().toStdString() + " " + QString::number(searchedBook.getAverageRating()).toStdString() + " " + searchedBook.getImageURL().toStdString() + " " + searchedBook.getSmallImageURL().toStdString() + " " + QString::number(searchedBook.getBooksCount()).toStdString() + " " + QString::number(searchedBook.getRemainingDays()).toStdString();
    client->Send(bookString);
}

void Controller::borrowBook(std::vector<std::string> message)
{
    if(loggedUser.getUserName()=="")
    {
        client->Send("You are not logged in!");
        return;
    }
    if(message.size()!=2)
    {
        client->Send("Wrong number of parameters!");
        return;
    }
    auto error = dataBase.borrowBook(loggedUser.getUserName(),QString::fromStdString(message[1]).toInt());
    client->Send(error);
    return;
}

void Controller::returnBook(std::vector<std::string> message)
{
    if(loggedUser.getUserName()=="")
    {
        client->Send("You are not logged in!");
        return;
    }
    if(message.size()!=2)
    {
        client->Send("Wrong number of parameters!");
        return;
    }
    dataBase.returnBook(loggedUser.getUserName(),QString::fromStdString(message[1]).toInt());
    client->Send("Book returned succesfully!");
    return;
}

void Controller::searchAvailableBooks(std::vector<std::string> message)
{
    if(loggedUser.getUserName()=="")
    {
        client->Send("You are not logged in!");
        return;
    }

    if(message.size()!=5)
    {
        client->Send("Wrong number of parameters!");
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

    std::string booksString = "Found " + QString::number(availableBooks.size()).toStdString() + " books\n";

    for(auto& book:availableBooks)
    {
        booksString += QString::number(book.getID()).toStdString() + " " + book.getISBN().toStdString() + " " + book.getAuthor().toStdString() + " " + QString::number(book.getOriginalPublicationYear()).toStdString() + " " + book.getTitle().toStdString() + " " + book.getLanguage().toStdString() + " " + QString::number(book.getAverageRating()).toStdString() + " " + book.getImageURL().toStdString() + " " + book.getSmallImageURL().toStdString() + " " + QString::number(book.getBooksCount()).toStdString() + '\n';
    }

    client->Send(booksString);
}

void Controller::searchBorrowedBooks(std::vector<std::string> message)
{
    if(loggedUser.getUserName()=="")
    {
        client->Send("You are not logged in!");
        return;
    }

    if(message.size()!=5)
    {
        client->Send("Wrong number of parameters!");
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
        dataBase.searchBorrowedBooks(QString::fromStdString(message[1]),QString::fromStdString(message[2]),QString::fromStdString(message[3]),index,loggedUser.getUserName());
    }

    auto borrowedBooks = dataBase.searchBorrowedBooks(QString::fromStdString(message[1]),QString::fromStdString(message[2]),QString::fromStdString(message[3]),QString::fromStdString(message[4]).toInt(),loggedUser.getUserName());

    std::string booksString = "Found " + QString::number(borrowedBooks.size()).toStdString() + " books\n";

    for(auto& book:borrowedBooks)
    {
        booksString += QString::number(book.getID()).toStdString() + " " + book.getISBN().toStdString() + " " + book.getAuthor().toStdString() + " " + QString::number(book.getOriginalPublicationYear()).toStdString() + " " + book.getTitle().toStdString() + " " + book.getLanguage().toStdString() + " " + QString::number(book.getAverageRating()).toStdString() + " " + book.getImageURL().toStdString() + " " + book.getSmallImageURL().toStdString() + " " + QString::number(book.getBooksCount()).toStdString() + " " + QString::number(book.getRemainingDays()).toStdString() + '\n';
    }
    client->Send(booksString);
}

void Controller::logOut(std::vector<std::string> message)
{
    if(loggedUser.getUserName()=="")
    {
        client->Send("You are not logged in!");
        return;
    }
    std::string response ="User " + loggedUser.getUserName().toStdString() + " has logged out.";

    client->Send(response);

    loggedUser = User();
}
void Controller::removeBook(std::vector<std::string> message)
{
    if(loggedUser.getUserName()=="")
    {
        client->Send("You are not logged in!");
        return;
    }
    if(dataBase.getBook(QString::fromStdString(message[1]).toInt()).getTitle() == "")
    {
        client->Send("Book doesn't exist");
        return;
    }
    dataBase.removeBook(QString::fromStdString(message[1]).toInt());

    client->Send("Book has been deleted succesfully!");

}

void Controller::getAvailableBooks(std::vector<std::string> message)
{
    if(loggedUser.getUserName()=="")
    {
        client->Send("You are not logged in!");
        return;
    }
    if(message.size()!=2)
    {
        client->Send("Wrong number of parameters!");
        return;
    }
    auto availableBooks = dataBase.getAvailableBooks(QString::fromStdString(message[1]).toInt());

    std::string booksString = "Found " + QString::number(availableBooks.size()).toStdString() + " books\n";

    for(auto& book:availableBooks)
    {
        booksString += QString::number(book.getID()).toStdString() + " " + book.getISBN().toStdString() + " " + book.getAuthor().toStdString() + " " + QString::number(book.getOriginalPublicationYear()).toStdString() + " " + book.getTitle().toStdString() + " " + book.getLanguage().toStdString() + " " + QString::number(book.getAverageRating()).toStdString() + " " + book.getImageURL().toStdString() + " " + book.getSmallImageURL().toStdString() + " " + QString::number(book.getBooksCount()).toStdString() + '\n';
    }

    client->Send(booksString);
}

void Controller::getBorrowedBooks(std::vector<std::string> message)
{
    if(loggedUser.getUserName()=="")
    {
        client->Send("You are not logged in!");
        return;
    }
    if(message.size()!=2)
    {
        client->Send("Wrong number of parameters!");
        return;
    }
    auto borrowedBooks = dataBase.getBorrowedBooks(QString::fromStdString(message[1]).toInt(),loggedUser.getUserName());

    std::string booksString = "Found " + QString::number(borrowedBooks.size()).toStdString() + " books\n";

    for(auto& book:borrowedBooks)
    {
        booksString += QString::number(book.getID()).toStdString() + " " + book.getISBN().toStdString() + " " + book.getAuthor().toStdString() + " " + QString::number(book.getOriginalPublicationYear()).toStdString() + " " + book.getTitle().toStdString() + " " + book.getLanguage().toStdString() + " " + QString::number(book.getAverageRating()).toStdString() + " " + book.getImageURL().toStdString() + " " + book.getSmallImageURL().toStdString() + " " + QString::number(book.getBooksCount()).toStdString() + " " + QString::number(book.getRemainingDays()).toStdString() + '\n';

    }

    client->Send(booksString);
}

void Controller::receiveComand()
{
    std::vector<std::string> message = parametrize(client->Receive());

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
        addBook(message);
        break;

    case Controller::Commands::RemoveBook:
        removeBook(message);
        break;

    case Controller::Commands::GetAvailableBooks:
        getAvailableBooks(message);
        break;

    case Controller::Commands::GetBorrowedBooks:
        getBorrowedBooks(message);
        break;

    case Controller::Commands::GetBook:
        getBook(message);
        break;

    case Controller::Commands::GetBorrowedBook:
        getBorrowedBook(message);
        break;

    case Controller::Commands::BorrowBook:
        borrowBook(message);
        break;

    case Controller::Commands::ReturnBook:
        returnBook(message);
        break;

    case Controller::Commands::SearchAvailableBooks:
        searchAvailableBooks(message);
        break;

    case Controller::Commands::SearchBorrowedBooks:
        searchBorrowedBooks(message);
        break;

    case Controller::Commands::LogOut:
        logOut(message);
        break;

    case Controller::Commands::None:
        client->Send("Unkown command");
        break;
    }
    message.clear();
}

void Controller::run(std::pair<SOCKET,std::string> clientData, EVP_PKEY* localKey)
{
    Controller controller;
    controller.client = new TCPSocket(clientData.first);


    auto remoteKey = TCPSocket::createPublicKey(clientData.second);
    controller.client->setRemoteKey(remoteKey);

    controller.client->setLocalKey(localKey);

    while(true)
    {
        controller.receiveComand();
    }
}
