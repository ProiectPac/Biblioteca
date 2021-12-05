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

void SQLDataBase::removeBook(QString title)
{
    QSqlQuery query;
    query.prepare("DELETE FROM books WHERE title = ?;");
    query.addBindValue(title);
    if(!query.exec())
        qWarning() << "ERROR: " << query.lastError().text();
}

std::vector<Book> SQLDataBase::getPreviousAvailableBooks(int pageNumber)
{
    std::vector<Book>previousBooks;
    QSqlQuery booksQuery;
    booksQuery.prepare("SELECT id,isbn,authors,original_publication_year,title,language_code,average_rating,image_url,small_image_url,remaining_days FROM books WHERE NOT EXISTS  (SELECT book_id FROM user_book WHERE books.id = user_book.book_id) AND ROWID < ? AND ROWID >= ?");
    booksQuery.addBindValue(pageNumber*10);
    pageNumber--;
    booksQuery.addBindValue(pageNumber*10);
    if(!booksQuery.exec())
        qWarning() << "ERROR: " << booksQuery.lastError().text();
    else
    {
        while(booksQuery.next())
        {
            previousBooks.push_back(Book(booksQuery.value(0).toInt(), booksQuery.value(1).toString(),booksQuery.value(2).toString(),booksQuery.value(3).toInt(), booksQuery.value(4).toString(),booksQuery.value(5).toString(),booksQuery.value(6).toFloat(),booksQuery.value(7).toString(),booksQuery.value(8).toString(),booksQuery.value(9).toInt()));
        }
    }
    return previousBooks;
}

std::vector<Book> SQLDataBase::getNextAvailableBooks(int pageNumber)
{
    std::vector<Book>availableBooks;
    QSqlQuery booksQuery;
    booksQuery.prepare("SELECT id,isbn,authors,original_publication_year,title,language_code,average_rating,image_url,small_image_url,remaining_days FROM books WHERE NOT EXISTS  (SELECT book_id FROM user_book WHERE books.id = user_book.book_id) AND ROWID >= ? AND ROWID < ?");
    booksQuery.addBindValue(pageNumber*10);
    pageNumber++;
    booksQuery.addBindValue(pageNumber*10);
    if(!booksQuery.exec())
        qWarning() << "ERROR: " << booksQuery.lastError().text();
    else
    {
        while(booksQuery.next())
        {
            availableBooks.push_back(Book(booksQuery.value(0).toInt(), booksQuery.value(1).toString(),booksQuery.value(2).toString(),booksQuery.value(3).toInt(), booksQuery.value(4).toString(),booksQuery.value(5).toString(),booksQuery.value(6).toFloat(),booksQuery.value(7).toString(),booksQuery.value(8).toString(),booksQuery.value(9).toInt()));
        }
    }
    return availableBooks;
}

void SQLDataBase::updateUserPassword(QString userName, unsigned int newPasswordHash)
{
    QSqlQuery query;
    query.prepare("UPDATE users SET passwordHash = ? WHERE name = ?");
    query.addBindValue(newPasswordHash);
    query.addBindValue(userName);
    if(!query.exec())
        qWarning() << "ERROR: " << query.lastError().text();
}

void SQLDataBase::borrowBook(QString userName, int bookID)
{
    QSqlQuery insertQuery;
    insertQuery.prepare("INSERT INTO user_book VALUES(?,?)");
    insertQuery.addBindValue(bookID);
    insertQuery.addBindValue(userName);

    if(!insertQuery.exec())
        qWarning() << "ERROR: " << insertQuery.lastError().text();
}

void SQLDataBase::returnBook(QString userName, int bookID)
{

    QSqlQuery deleteQuery;
    deleteQuery.prepare("DELETE FROM user_book WHERE book_id = ? AND user_name = ?");
    deleteQuery.addBindValue(bookID);
    deleteQuery.addBindValue(userName);

    if(!deleteQuery.exec())
        qWarning() << "ERROR: " << deleteQuery.lastError().text();
}
