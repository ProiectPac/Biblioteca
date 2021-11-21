#include "user.h"

const QString &User::getUserName() const
{
    return userName;
}

unsigned int User::getPasswordHash() const
{
    return passwordHash;
}

void User::setUserName(const QString &newUserName)
{
    userName = newUserName;
}

void User::setPasswordHash(const unsigned int& newPasswordHash)
{
    passwordHash = newPasswordHash;
}

QVector<Book> &User::getBorrowedBooks()
{
    return borrowedBooks;
}

void User::addBorrowedBook(const Book& newBook)
{
    borrowedBooks.push_back(newBook);
}

void User::removeBorrowedBook(const Book& book)
{
    for(int index=0;index<borrowedBooks.size();++index)
    {
        if(borrowedBooks[index].getISBN()==book.getISBN())
            borrowedBooks.remove(index);
        break;
    }
}

User::User()
{
    userName="";
    passwordHash=0;
}

User::~User()
{

}

User::User(const QString& userName, const unsigned int& passwordHash)
{
    this->userName=userName;
    this->passwordHash=passwordHash;
}


