#include "database.h"


DataBase::DataBase()
{
    userList.resize(0);
    availableBooks.resize(0);
    std::ifstream fin("../Biblioteca/dataBase.txt");
    int numberOfUsers=0;
    fin>>numberOfUsers;
    fin.get();
    for(int i=0;i<numberOfUsers;++i)
    {
        std::string userName;
        fin>>userName;
        unsigned int passwordHash;
        fin>>passwordHash;
        int numberOfBooks;
        fin>>numberOfBooks;
        fin.get();
        User newUser(QString::fromStdString(userName), passwordHash);
        for(int j=0;j<numberOfBooks;++j)
        {
            std::string title;
            std::getline(fin,title);
            std::string author;
            std::getline(fin,author);
            std::string ISBN;
            std::getline(fin,ISBN);
            int remainingDays;
            fin>>remainingDays;
            fin.get();
            newUser.addBorrowedBook(Book(QString::fromStdString(title), QString::fromStdString(author), QString::fromStdString(ISBN),remainingDays));
        }
        addUser(newUser);
    }
    int numberAvailableBooks;
    fin>>numberAvailableBooks;
    fin.get();
    for(int j=0;j<numberAvailableBooks;++j)
    {
        std::string title;
        std::getline(fin,title);
        std::string author;
        std::getline(fin,author);
        std::string ISBN;
        std::getline(fin,ISBN);
        addAvailableBook(Book(QString::fromStdString(title), QString::fromStdString(author), QString::fromStdString(ISBN),-1));
    }
}

User DataBase::findUser(QString userName, unsigned int passwordHash)
{
    for(auto& user : userList)
    {
        if(user.getUserName()==userName&&user.getPasswordHash()==passwordHash)
            return user;
    }
    return User();
}

void DataBase::addUser(User user)
{
    userList.push_back(user);
}

void DataBase::removeUser(User user)
{
    for(int i=0;i<userList.size();++i)
        if(user.getUserName()==userList[i].getUserName())
            userList.remove(i);
}

QVector<Book> DataBase::getAvailableBooks()
{
    return availableBooks;
}

void DataBase::addAvailableBook(Book book)
{
    availableBooks.push_back(book);
}

void DataBase::removeBook(Book book)
{
    for(int i=0;i<availableBooks.size();i++)
        if(book.getISBN()==availableBooks[i].getISBN())
            availableBooks.remove(i);
}

DataBase::~DataBase()
{
    std::ofstream fout("../Biblioteca/dataBase.txt");
    fout<<userList.size()<<'\n';
    for(auto& user : userList)
    {
        fout<<user.getUserName().toStdString()<<' '<<user.getPasswordHash()<<'\n';
        fout<<user.getBorrowedBooks().size()<<'\n';
        for(auto& book : user.getBorrowedBooks())
        {
            fout<<book.getName().toStdString()<<'\n'<<book.getAuthor().toStdString()<<'\n'<<book.getISBN().toStdString() << '\n' << book.getRemaingDays() <<'\n';
        }

    }
    fout<<availableBooks.size()<<'\n';
    for(auto& book : availableBooks)
    {
        fout<<book.getName().toStdString()<<'\n'<<book.getAuthor().toStdString()<<'\n'<<book.getISBN().toStdString()<<'\n';
    }
}
