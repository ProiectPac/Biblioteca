#ifndef SQLDATABASE_H
#define SQLDATABASE_H
#include"user.h"
#include"book.h"
#include <QSqlDatabase>
#include <QSqlDriver>
#include <QSqlError>
#include <QSqlQuery>
#include <QMessageBox>

class SQLDataBase
{
private:
    QSqlDatabase dataBase;
public:
    void addUser(User user);
    SQLDataBase();
    User findUser(QString name, unsigned int passwordHash);
    void removeUser(QString name);
    void addBook(Book book);
};

#endif // SQLDATABASE_H
