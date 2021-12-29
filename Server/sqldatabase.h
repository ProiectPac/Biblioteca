#ifndef SQLDATABASE_H
#define SQLDATABASE_H
#include"user.h"
#include"book.h"
#include <QSqlDatabase>
#include <QSqlDriver>
#include <QSqlError>
#include <QSqlQuery>
#include <map>

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
    Book getBook(int id);
    void updateUserPassword(QString userName, unsigned int newPasswordHash);
    void borrowBook(QString userName, int bookId);
    void returnBook(QString userName, int bookId);
    std::vector<Book> getBorrowedBooks(int pageNumber, QString userName);
    Book getBorrowedBook(int id,QString userName);

    std::vector<Book> searchAvailableBooks(QString name, QString author, QString ISBN, int pageNumber);
    std::vector<Book> searchBorrowedBooks(QString name, QString author, QString ISBN, int pageNumber, QString userName);
    static int levenshteinDistance(std::string draft, std::string original);

    class Comp
    {
        QString name,author,ISBN;
    public:
        Comp(QString name,QString author,QString ISBN)
        {
            this->name=name;
            this->author=author;
            this->ISBN=ISBN;
        }
        bool operator()(Book bookOne, Book bookTwo)
        {
            int sumDistance = 0;
            if(name.length()>0)
            {
                sumDistance += levenshteinDistance(bookOne.getTitle().toStdString(),name.toStdString());
            }
            if(author.length()>0)
            {
                sumDistance += levenshteinDistance(bookOne.getAuthor().toStdString(),author.toStdString());
            }
            if(ISBN.length()>0)
            {
                sumDistance += levenshteinDistance(bookOne.getISBN().toStdString(),ISBN.toStdString());
            }

            int sumDistanceAux=0;
            if(name.length()>0)
            {
                sumDistanceAux += levenshteinDistance(bookTwo.getTitle().toStdString(),name.toStdString());
            }
            if(author.length()>0)
            {
                sumDistanceAux += levenshteinDistance(bookTwo.getAuthor().toStdString(),author.toStdString());
            }
            if(ISBN.length()>0)
            {
                sumDistanceAux += levenshteinDistance(bookTwo.getISBN().toStdString(),ISBN.toStdString());
            }
            return sumDistance<sumDistanceAux;
        }
    };

};

#endif // SQLDATABASE_H
