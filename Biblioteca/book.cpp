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

const QString &Book::getTitle() const
{
    return title;
}

void Book::setTitle(const QString &newTitle)
{
    title = newTitle;
}

void Book::setAuthor(const QString &newAuthor)
{
    author = newAuthor;
}

void Book::setISBN(const QString &newISBN)
{
    ISBN = newISBN;
}

int Book::getRemainingDays() const
{
    return remainingDays;
}

void Book::setID(int newID)
{
    ID = newID;
}

void Book::setBooksCount(int newBooksCount)
{
    booksCount = newBooksCount;
}

void Book::setOriginalPublicationYear(int newOriginalPublicationYear)
{
    originalPublicationYear = newOriginalPublicationYear;
}

void Book::setLanguage(const QString &newLanguage)
{
    language = newLanguage;
}

void Book::setAverageRating(float newAverageRating)
{
    averageRating = newAverageRating;
}

void Book::setImageURL(const QString &newImageURL)
{
    imageURL = newImageURL;
}

const QString &Book::getSmallImageURL() const
{
    return smallImageURL;
}

void Book::setSmallImageURL(const QString &newSmallImageURL)
{
    smallImageURL = newSmallImageURL;
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

