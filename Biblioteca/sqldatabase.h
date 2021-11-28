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
    QVector<User> userList;
    QVector<Book> availableBooks;
    void showError(const QSqlError &err);
public:
    SQLDataBase();
};

#endif // SQLDATABASE_H
