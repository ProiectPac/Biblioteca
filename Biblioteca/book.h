#ifndef BOOK_H
#define BOOK_H

#include <QString>

class Book
{
private:
    QString name;
    QString author;
    QString ISBN;
    int remaingDays;
public:
    Book();
    Book(QString name,QString author,QString ISBN,int remaingDays);
    const QString &getAuthor() const;
    const QString &getISBN() const;
    const QString &getName() const;
    int getRemaingDays() const;
};

#endif // BOOK_H
