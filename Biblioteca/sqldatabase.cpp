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
    query.prepare("INSERT INTO books(isbn,authors,original_publication_year,title,language_code,average_rating,image_url,small_image_url) VALUES(?,?,?,?,?,?,?,?)");
    query.addBindValue(book.getISBN());
    query.addBindValue(book.getAuthor());
    query.addBindValue(book.getOriginalPublicationYear());
    query.addBindValue(book.getTitle());
    query.addBindValue(book.getLanguage());
    query.addBindValue(book.getAverageRating());
    query.addBindValue(book.getImageURL());
    query.addBindValue(book.getSmallImageURL());
    if(!query.exec())
        qWarning() << "ERROR: " << query.lastError().text();
}

void SQLDataBase::removeBook(int bookId)
{
    QSqlQuery query;
    query.prepare("DELETE FROM books WHERE id = ?;");
    query.addBindValue(bookId);
    if(!query.exec())
        qWarning() << "ERROR: " << query.lastError().text();
}

std::vector<Book> SQLDataBase::getAvailableBooks(int pageNumber)
{
    std::vector<Book>availableBooks;
    QSqlQuery booksQuery;
    booksQuery.prepare("SELECT id,isbn,authors,original_publication_year,title,language_code,average_rating,image_url,small_image_url,books_count FROM books WHERE books_count>0 AND ROWID >= ? AND ROWID < ?");
    booksQuery.addBindValue(pageNumber*45);
    pageNumber++;
    booksQuery.addBindValue(pageNumber*45);
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
    /*if(getBorrowedBooks(0,userName).size()>=5)
    {
        QString message= "You have too many borrowed books!";
        QErrorMessage::qtHandler()->showMessage(message);
        return;
    }*/
    for(auto&book:getBorrowedBooks(0,userName))
    {
        if(book.getRemainingDays()==0)
        {
            QString message= "You have to return the borrowed book that's past the return limit!";
            QErrorMessage::qtHandler()->showMessage(message);
            return;
        }
        if(book.getID()==bookID)
        {
            QString message= "You cannot borrow the same book twice!";
            QErrorMessage::qtHandler()->showMessage(message);
            return;
        }
    }
    int booksCount=0;
    QSqlQuery booksCountQuery;
    booksCountQuery.prepare("SELECT books_count FROM books WHERE id=?");
    booksCountQuery.addBindValue(bookID);
    if(!booksCountQuery.exec())
    {
        qWarning() << "ERROR: " << booksCountQuery.lastError().text();
    }
    if(booksCountQuery.first())
    {
        booksCount=booksCountQuery.value(0).toInt();
        if(booksCountQuery.value(0)==0)
        {
            QString message= "There are no books left to borrow!";
            QErrorMessage::qtHandler()->showMessage(message);
            return;
        }
    }
    QSqlQuery insertQuery;
    insertQuery.prepare("INSERT INTO user_book VALUES(?,?,14)");
    insertQuery.addBindValue(bookID);
    insertQuery.addBindValue(userName);
    if(!insertQuery.exec())
        qWarning() << "ERROR: " << insertQuery.lastError().text();
    QSqlQuery updateQuery;
    updateQuery.prepare("UPDATE books SET books_count=? WHERE id=?");
    updateQuery.addBindValue(booksCount-1);
    updateQuery.addBindValue(bookID);
    if(!updateQuery.exec())
        qWarning() << "ERROR: " << updateQuery.lastError().text();
}

void SQLDataBase::returnBook(QString userName, int bookID)
{
    int booksCount=0;
    QSqlQuery booksCountQuery;
    booksCountQuery.prepare("SELECT books_count FROM books WHERE id=?");
    booksCountQuery.addBindValue(bookID);
    if(!booksCountQuery.exec())
    {
        qWarning() << "ERROR: " << booksCountQuery.lastError().text();
    }

    if(booksCountQuery.first())
    {
        booksCount=booksCountQuery.value(0).toInt();
    }

    QSqlQuery updateQuery;
    updateQuery.prepare("UPDATE books SET books_count=? WHERE id=?");
    updateQuery.addBindValue(booksCount+1);
    updateQuery.addBindValue(bookID);
    if(!updateQuery.exec())
        qWarning() << "ERROR: " << updateQuery.lastError().text();

    QSqlQuery deleteQuery;
    deleteQuery.prepare("DELETE FROM user_book WHERE book_id = ? AND user_name = ?");
    deleteQuery.addBindValue(bookID);
    deleteQuery.addBindValue(userName);

    if(!deleteQuery.exec())
        qWarning() << "ERROR: " << deleteQuery.lastError().text();
}

std::vector<Book> SQLDataBase::getBorrowedBooks(int pageNumber, QString userName)
{
    std::vector<Book>previousBooks;
    QSqlQuery booksQuery;
    QSqlQuery createTable("CREATE TABLE borrowed_books (id,isbn,authors,original_publication_year,title,language_code,average_rating,image_url,small_image_url,remaining_days, PRIMARY KEY(id));");
    QSqlQuery insert;

    insert.prepare(" INSERT INTO borrowed_books(id,isbn,authors,original_publication_year,title,language_code,average_rating,image_url,small_image_url,remaining_days) SELECT books.id,books.isbn,books.authors,books.original_publication_year,books.title,books.language_code,books.average_rating,books.image_url,books.small_image_url,user_book.remaining_days FROM ((user_book INNER JOIN users ON users.name = user_book.user_name) INNER JOIN books ON books.id = user_book.book_id) WHERE users.name = ?;");
    booksQuery.prepare(" SELECT * FROM borrowed_books WHERE ROWID >= ? AND ROWID < ? ;");
    insert.addBindValue(userName);
    booksQuery.addBindValue(pageNumber*45);
    pageNumber++;
    booksQuery.addBindValue(pageNumber*45);
    if(!insert.exec())
        qWarning() << "ERROR: " << insert.lastError().text();
    else
    {
        if(!booksQuery.exec())
            qWarning() << "ERROR: " << booksQuery.lastError().text();
        else
        {
            while(booksQuery.next())
            {
                previousBooks.push_back(Book(booksQuery.value(0).toInt(), booksQuery.value(1).toString(),booksQuery.value(2).toString(),booksQuery.value(3).toInt(), booksQuery.value(4).toString(),booksQuery.value(5).toString(),booksQuery.value(6).toFloat(),booksQuery.value(7).toString(),booksQuery.value(8).toString(),0,booksQuery.value(9).toInt()));
            }
        }
        QSqlQuery drop("DROP TABLE borrowed_books");
    }
    return previousBooks;
}

std::vector<Book> SQLDataBase::searchAvailableBooks(QString name, QString author, QString ISBN,int pageNumber)
{
    static std::vector<int> pageCorespondence;
    static QString staticName;
    static QString staticAuthor;
    static QString staticISBN;

    if(staticName!=name)
    {
        staticName = name;
        pageCorespondence.resize(0);
        pageCorespondence.push_back(0);
    }
    if(staticAuthor!=author)
    {
        staticAuthor = author;
        pageCorespondence.resize(0);
        pageCorespondence.push_back(0);
    }
    if(staticISBN!=ISBN)
    {
        staticISBN = ISBN;
        pageCorespondence.resize(0);
        pageCorespondence.push_back(0);
    }
    std::vector<Book> matchingBooks;
    if(pageNumber>=pageCorespondence.size())
        return matchingBooks;
    std::vector<Book> currentBooks = getAvailableBooks(pageCorespondence[pageNumber]);
    int numberOfPages=pageCorespondence[pageNumber]+1;
    while(matchingBooks.size()<45 && currentBooks.size() != 0)
    {
        for(auto &book: currentBooks)
        {
            int sumDistance = 0;
            if(name.length()>0)
            {
                sumDistance += levenshteinDistance(book.getTitle().toStdString(),name.toStdString());
            }
            if(author.length()>0)
            {
                sumDistance += levenshteinDistance(book.getAuthor().toStdString(),author.toStdString());
            }
            if(ISBN.length()>0)
            {
                sumDistance += levenshteinDistance(book.getISBN().toStdString(),ISBN.toStdString());
            }

            if(sumDistance < 5)
            {
                matchingBooks.push_back(book);
            }
        }
        numberOfPages++;
        currentBooks = getAvailableBooks(numberOfPages);
    }
    Comp comparator(name,author,ISBN);
    std::sort(matchingBooks.begin(),matchingBooks.end(),comparator);
    if(pageNumber+1>=pageCorespondence.size())
    {
        pageCorespondence.push_back(numberOfPages);
    }
    return matchingBooks;
}


std::vector<Book> SQLDataBase::searchBorrowedBooks(QString name, QString author, QString ISBN,int pageNumber, QString userName)
{
    static std::vector<int> pageCorespondence;
    static QString staticName;
    static QString staticAuthor;
    static QString staticISBN;

    if(staticName!=name)
    {
        staticName = name;
        pageCorespondence.resize(0);
        pageCorespondence.push_back(0);
    }
    if(staticAuthor!=author)
    {
        staticAuthor = author;
        pageCorespondence.resize(0);
        pageCorespondence.push_back(0);
    }
    if(staticISBN!=ISBN)
    {
        staticISBN = ISBN;
        pageCorespondence.resize(0);
        pageCorespondence.push_back(0);
    }
    std::vector<Book> matchingBooks;
    if(pageNumber>=pageCorespondence.size())
            return matchingBooks;
    std::vector<Book> currentBooks = getBorrowedBooks(pageCorespondence[pageNumber],userName);
    int numberOfPages=pageCorespondence[pageNumber]+1;
    while(matchingBooks.size()<45 && currentBooks.size()!=0)
    {
        for(auto &book: currentBooks)
        {
            int sumDistance = 0;
            if(name.length()>0)
            {
                sumDistance += levenshteinDistance(book.getTitle().toStdString(),name.toStdString());
            }
            if(author.length()>0)
            {
                sumDistance += levenshteinDistance(book.getAuthor().toStdString(),author.toStdString());
            }
            if(ISBN.length()>0)
            {
                sumDistance += levenshteinDistance(book.getISBN().toStdString(),ISBN.toStdString());
            }

            if(sumDistance < 5)
            {
                matchingBooks.push_back(book);
            }
        }
        numberOfPages++;
        currentBooks = getBorrowedBooks(numberOfPages,userName);
    }
    Comp comparator(name,author,ISBN);
    std::sort(matchingBooks.begin(),matchingBooks.end(),comparator);
    if(pageNumber+1>=pageCorespondence.size())
    {
        pageCorespondence.push_back(numberOfPages);
    }
    return matchingBooks;
}
int SQLDataBase::levenshteinDistance(std::string p_string1, std::string p_string2)
{
    int l_string_length1 = p_string1.length();
    int l_string_length2 = p_string2.length();
    int d[l_string_length1+1][l_string_length2+1];

    int i;
    int j;
    int l_cost;

    for (i = 0;i <= l_string_length1;i++)
    {
        d[i][0] = i;
    }
    for(j = 0; j<= l_string_length2; j++)
    {
        d[0][j] = j;
    }
    for (i = 1;i <= l_string_length1;i++)
    {
        for(j = 1; j<= l_string_length2; j++)
        {
            if( p_string1[i-1] == p_string2[j-1] )
            {
                l_cost = 0;
            }
            else
            {
                l_cost = 1;
            }
            d[i][j] = std::min(d[i-1][j] + 1,std::min(d[i][j-1] + 1,d[i-1][j-1] + l_cost));
            if( (i > 1) && (j > 1) && (p_string1[i-1] == p_string2[j-2]) && (p_string1[i-2] == p_string2[j-1]))
            {
                d[i][j] = std::min(d[i][j], d[i-2][j-2] + l_cost);
            }
        }
    }
        return d[l_string_length1][l_string_length2];
}
