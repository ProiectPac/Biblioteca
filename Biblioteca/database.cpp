#include "database.h"


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
