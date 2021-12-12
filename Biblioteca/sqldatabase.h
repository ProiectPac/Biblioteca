#ifndef SQLDATABASE_H
#define SQLDATABASE_H
#include"user.h"
#include"book.h"
#include <QSqlDatabase>
#include <QSqlDriver>
#include <QSqlError>
#include <QSqlQuery>
#include <QMessageBox>
#include <QErrorMessage>

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

    std::vector<Book> searchAvailableBooks(QString name, QString author, QString ISBN, int pageNumber);
    std::vector<Book> searchBorrowedBooks(QString name, QString author, QString ISBN, int pageNumber, QString userName);
    int levenshteinDistance(std::string draft, std::string original);

};

#endif // SQLDATABASE_H
