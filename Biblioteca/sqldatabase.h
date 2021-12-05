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
    SQLDataBase();
    void addUser(User user);
    User findUser(QString name, unsigned int passwordHash);
    void removeUser(QString name);
    void addBook(Book book);
    void removeBook(QString title);
    std::vector<Book> getNextAvailableBooks(int& pageNumber);
    void updateUserPassword(QString userName, unsigned int newPasswordHash);
    void borrowBook(QString userName, QString bookTitle);
    void returnBook(QString userName, QString bookTitle);
};

#endif // SQLDATABASE_H
