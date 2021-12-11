#ifndef BOOK_H
#define BOOK_H

#include <QString>

class Book
{
private:
    QString title;
    QString author;
    QString ISBN;
    int remainingDays;
    int ID;
    int booksCount;
    int originalPublicationYear;
    QString language;
    float averageRating;
    QString imageURL;
    QString smallImageURL;


public:
    Book();
    Book(const int& ID, const QString& ISBN, const QString& author, const int& originalPublicationYear, const QString& title, const QString& language, const float& averageRating, const QString& imageURL, const QString& smallImageURL,const int& remainingDays=-1);
    const QString &getAuthor() const;
    const QString &getISBN() const;
    const QString &getTitle() const;
    const int &getID() const;
    const int &getBooksCount() const;
    const int &getOriginalPublicationYear() const;
    const float &getAverageRating() const;
    const QString &getLanguage() const;
    const QString &getImageURL() const;
    void setRemainingDays(int newRemainingDays);
    void setTitle(const QString &newTitle);
    void setAuthor(const QString &newAuthor);
    void setISBN(const QString &newISBN);
    int getRemainingDays() const;
    void setID(int newID);
    void setBooksCount(int newBooksCount);
    void setOriginalPublicationYear(int newOriginalPublicationYear);
    void setLanguage(const QString &newLanguage);
    void setAverageRating(float newAverageRating);
    void setImageURL(const QString &newImageURL);
    const QString &getSmallImageURL() const;
    void setSmallImageURL(const QString &newSmallImageURL);
};

#endif // BOOK_H
