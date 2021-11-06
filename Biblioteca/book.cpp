#include "book.h"

const QString &Book::getAuthor() const
{
    return author;
}

const QString &Book::getISBN() const
{
    return ISBN;
}

const QString &Book::getName() const
{
    return name;
}

Book::Book()
{
    name="";
    author="";
    ISBN="";
}

Book::Book(QString name,QString author,QString ISBN)
{
    this->name=name;
    this->author=author;
    this->ISBN=ISBN;
}

