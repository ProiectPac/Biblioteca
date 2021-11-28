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
    return title;
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
    return remainingDays;
}

void Book::setRemainingDays(int newRemainingDays)
{
    remainingDays = newRemainingDays;
}

Book::Book()
{
    title="";
    author="";
    ISBN="";
}

Book::Book(const int &ID, const QString &ISBN, const QString &author, const int &originalPublicationYear, const QString &title, const QString &language, const float &averageRating, const QString &imageURL, const QString &smallImageURL, const int &remainingDays)
{
    this->ID = ID;
    this->ISBN = ISBN;
    this->author = author;
    this->originalPublicationYear = originalPublicationYear;
    this->title = title;
    this->language = language;
    this->averageRating = averageRating;
    this->imageURL = imageURL;
    this->smallImageURL = smallImageURL;
    this->remainingDays = remainingDays;
}

