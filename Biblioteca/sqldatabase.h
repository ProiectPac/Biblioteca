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
    void removeBook(int bookId);
    std::vector<Book> getAvailableBooks(int pageNumber);
    void updateUserPassword(QString userName, unsigned int newPasswordHash);
    void borrowBook(QString userName, int bookId);
    void returnBook(QString userName, int bookId);
    std::vector<Book> getBorrowedBooks(int pageNumber, QString userName);

};

#endif // SQLDATABASE_H
