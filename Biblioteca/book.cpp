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

const int &Book::getID() const
{
    return ID;
}

const int &Book::getBooksCount() const
{
    return booksCount;
}

const int &Book::getOriginalPublicationYear() const
{
    return originalPublicationYear;
}

const QString &Book::getLanguage() const
{
    return language;
}

const QString &Book::getImageURL() const
{
    return imageURL;
}

int Book::getRemaingDays() const
{
    return remaingDays;
}

void Book::setRemaingDays(int newRemaingDays)
{
    remaingDays = newRemaingDays;
}

Book::Book()
{
    name="";
    author="";
    ISBN="";
}

Book::Book(QString name,QString author,QString ISBN,int remaingDays)
{
    this->name=name;
    this->author=author;
    this->ISBN=ISBN;
    this->remaingDays = remaingDays;
}

