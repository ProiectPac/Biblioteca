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
};

#endif // SQLDATABASE_H
