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
    int ID;
    int booksCount;
    int originalPublicationYear;
    QString language;
    float averageRating;
    QString imageURL;
public:
    Book();
    Book(QString name,QString author,QString ISBN,int remaingDays);
    const QString &getAuthor() const;
    const QString &getISBN() const;
    const QString &getName() const;
    const int &getID() const;
    const int &getBooksCount() const;
    const int &getOriginalPublicationYear() const;
    const float &getAverageRating() const;
    const QString &getLanguage() const;
    const QString &getImageURL() const;
    int getRemaingDays() const;
    void setRemaingDays(int newRemaingDays);
};

#endif // BOOK_H
