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
    QVector<Book>borrowedBooks;
public:
    User();
    User(QString userName, unsigned int passwordHash);

    const QString &getUserName() const;
    const unsigned int getPasswordHash();
    void setUserName(const QString &newUserName);
    void setPasswordHash(unsigned int newPasswordHash);
    const QVector<Book> &getBorrowedBooks() const;
    void addBorrowedBook(Book newBook);
    void removeBorrowedBook(Book book);
};

#endif // USER_H
