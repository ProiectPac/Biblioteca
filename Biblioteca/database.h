#ifndef DATABASE_H
#define DATABASE_H

#include "user.h"

#include<QString>
#include<QVector>
#include<QList>
#include<fstream>

class DataBase
{
private:
    QVector<User> userList;
    QVector<Book> availableBooks;
public:
    DataBase();
    User* findUser(QString userName, unsigned int passwordHash);
    void addUser(User user);
    void removeUser(User user);
    QVector<Book> getAvailableBooks();
    void addAvailableBook(Book book);
    void removeBook(Book book);
    ~DataBase();
};

#endif // DATABASE_H
