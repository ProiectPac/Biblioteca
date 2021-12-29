#ifndef USER_H
#define USER_H

#include "book.h"
#include <QString>
#include <QList>


class User
{
private:
    QString userName;
    unsigned int passwordHash;

public:
    User();
    ~User();
    User(const QString& userName, const unsigned int& passwordHash);

    const QString &getUserName() const;
    unsigned int getPasswordHash() const;
    void setUserName(const QString &newUserName);
    void setPasswordHash(const unsigned int& newPasswordHash);
    QVector<Book> &getBorrowedBooks();
    void addBorrowedBook(const Book& newBook);
    void removeBorrowedBook(const Book& book);
};

#endif // USER_H
