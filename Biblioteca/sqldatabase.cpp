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
        booksQuery.prepare("SELECT ID,isbn,authors,original_publication_year,title,language_code,average_rating,image_url,small_image_url,remaining_days FROM books WHERE owner = ?");
        booksQuery.addBindValue(name);
        if(!booksQuery.exec())
          qWarning() << "ERROR: " << booksQuery.lastError().text();
        else
        {
            while(booksQuery.next())
            {
                foundUser.addBorrowedBook(Book(booksQuery.value(0).toInt(),booksQuery.value(1).toString(),booksQuery.value(2).toString(),booksQuery.value(3).toInt(), booksQuery.value(4).toString(),booksQuery.value(5).toString(),booksQuery.value(6).toFloat(),booksQuery.value(7).toString(),booksQuery.value(8).toString(),booksQuery.value(9).toInt()));
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
