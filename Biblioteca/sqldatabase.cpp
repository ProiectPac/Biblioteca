#include "sqldatabase.h"

void SQLDataBase::addUser(User user)
{
    QSqlQuery query;
    query.prepare("INSERT INTO users VALUES(?,?)");
    query.addBindValue(user.getUserName());
    query.addBindValue(user.getPasswordHash());
    if(!query.exec())
      qWarning() << "ERROR: " << query.lastError().text();
}

SQLDataBase::SQLDataBase()
{
    const QString driver ="QSQLITE";
    if(QSqlDatabase::isDriverAvailable(driver))
    {
        dataBase = QSqlDatabase::addDatabase(driver);
        dataBase.setDatabaseName("../dataBase.db");
        if(!dataBase.open())
        {
            qWarning() << "ERROR: " << dataBase.lastError();
        }
    }
}

User SQLDataBase::findUser(QString name, unsigned int passwordHash)
{
    QSqlQuery query;
    query.prepare("SELECT name,passwordHash FROM users WHERE name=? AND passwordHash = ?");
    query.addBindValue(name);
    query.addBindValue(passwordHash);
    if(!query.exec())
      qWarning() << "ERROR: " << query.lastError().text();
    else
    if(query.first())
    {
        User foundUser(name,passwordHash);
        QSqlQuery booksQuery;
        booksQuery.prepare("SELECT isbn,authors,original_publication_year,title,language_code,average_rating,image_url,small_image_url,remaining_days FROM books WHERE owner = ?");
        booksQuery.addBindValue(name);
        if(!booksQuery.exec())
          qWarning() << "ERROR: " << booksQuery.lastError().text();
        else
        {
            while(booksQuery.next())
            {
                foundUser.addBorrowedBook(Book(booksQuery.value(0).toString(),booksQuery.value(1).toString(),booksQuery.value(2).toInt(), booksQuery.value(3).toString(),booksQuery.value(4).toString(),booksQuery.value(5).toFloat(),booksQuery.value(6).toString(),booksQuery.value(7).toString(),booksQuery.value(8).toInt()));
            }

        }
        return foundUser;
    }
    else
      return User();
}
void SQLDataBase::removeUser(QString name)
{
    QSqlQuery query;
    query.prepare("DELETE FROM users WHERE name = ?;");
    query.addBindValue(name);
    if(!query.exec())
        qWarning() << "ERROR: " << query.lastError().text();
}

void SQLDataBase::addBook(Book book)
{
    QSqlQuery query;
    query.prepare("INSERT INTO books(isbn,authors,original_publication_year,title,language_code,average_rating,image_url,small_image_url,remaining_days) VALUES(?,?,?,?,?,?,?,?,?)");
    query.addBindValue(book.getISBN());
    query.addBindValue(book.getAuthor());
    query.addBindValue(book.getOriginalPublicationYear());
    query.addBindValue(book.getTitle());
    query.addBindValue(book.getLanguage());
    query.addBindValue(book.getAverageRating());
    query.addBindValue(book.getImageURL());
    query.addBindValue(book.getSmallImageURL());
    query.addBindValue(book.getRemainingDays());
    if(!query.exec())
        qWarning() << "ERROR: " << query.lastError().text();
}
