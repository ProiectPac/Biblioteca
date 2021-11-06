#include "user.h"

const QString &User::getUserName() const
{
    return userName;
}

void User::setUserName(const QString &newUserName)
{
    userName = newUserName;
}

void User::setPasswordHash(unsigned int newPasswordHash)
{
    passwordHash = newPasswordHash;
}

const QVector<Book> &User::getBorrowedBooks() const
{
    return borrowedBooks;
}

void User::addBorrowedBook(Book newBook)
{
    borrowedBooks.push_back(newBook);
}

void User::removeBorrowedBook(Book book)
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

User::User(QString userName, unsigned int passwordHash)
{
    this->userName=userName;
    this->passwordHash=passwordHash;
}


