#ifndef SQLDATABASE_H
#define SQLDATABASE_H
#include"user.h"
#include"book.h"
#include <QSqlDatabase>
#include <QSqlDriver>
#include <QSqlError>
#include <QSqlQuery>

class SQLDataBase
{
private:
    QVector<User> userList;
    QVector<Book> availableBooks;
public:
    SQLDataBase();
};

#endif // SQLDATABASE_H
