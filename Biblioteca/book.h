#ifndef BOOK_H
#define BOOK_H

#include <QString>

class Book
{
private:
    QString name;
    QString author;
    QString ISBN;
public:
    Book();
    Book(QString name,QString author,QString ISBN);
    const QString &getAuthor() const;
    const QString &getISBN() const;
    const QString &getName() const;
};

#endif // BOOK_H
