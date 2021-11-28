#include "database.h"


DataBase::DataBase()
{
    userList.resize(0);
    availableBooks.resize(0);
    std::ifstream fin("../Biblioteca/dataBase.txt");
    int numberOfUsers=0;
    fin>>numberOfUsers;
    fin.get();
    for(int i=0;i<numberOfUsers;++i)
    {
        std::string userName;
        fin>>userName;
        unsigned int passwordHash;
        fin>>passwordHash;
        int numberOfBooks;
        fin>>numberOfBooks;
        fin.get();
        User newUser(QString::fromStdString(userName), passwordHash);
        for(int j=0;j<numberOfBooks;++j)
        {
            std::string title;
            std::getline(fin,title);
            std::string author;
            std::getline(fin,author);
            std::string ISBN;
            std::getline(fin,ISBN);
            int remainingDays;
            fin>>remainingDays;
            fin.get();
           // newUser.addBorrowedBook(Book(QString::fromStdString(title), QString::fromStdString(author), QString::fromStdString(ISBN),remainingDays));
        }
        addUser(newUser);
    }
    int numberAvailableBooks;
    fin>>numberAvailableBooks;
    fin.get();
    for(int j=0;j<numberAvailableBooks;++j)
    {
        std::string title;
        std::getline(fin,title);
        std::string author;
        std::getline(fin,author);
        std::string ISBN;
        std::getline(fin,ISBN);
        //addAvailableBook(Book(QString::fromStdString(title), QString::fromStdString(author), QString::fromStdString(ISBN),-1));
    }
}


User* DataBase::findUser(QString userName, unsigned int passwordHash)
{
    for(auto& user : userList)
    {
        if(user.getUserName()==userName&&user.getPasswordHash()==passwordHash)
            return &user;
    }
    return nullptr;
}

void DataBase::addUser(User user)
{
    userList.push_back(user);
}

void DataBase::removeUser(User user)
{
    for(int i=0;i<userList.size();++i)
        if(user.getUserName()==userList[i].getUserName())
            userList.remove(i);
}

QVector<Book> DataBase::getAvailableBooks()
{
    return availableBooks;
}

void DataBase::addAvailableBook(Book book)
{
    availableBooks.push_back(book);
}

void DataBase::removeBook(Book book)
{
    for(int i=0;i<availableBooks.size();i++)
        if(book.getISBN()==availableBooks[i].getISBN())
            availableBooks.remove(i);
}
int DataBase::levenshteinDistance(std::string draft, std::string original)
{

        std::vector<std::vector<int>> matrix;
        matrix.resize(original.size()+1, std::vector<int>(draft.size()+1,0));

        for (int index = 0; index < matrix.size(); index++)
            matrix[index][matrix[index].size() - 1] = matrix.size() - (index + 1);

        for (int index = matrix[0].size() - 1; index >= 0; index--)
            matrix[matrix.size() - 1][index] = matrix[index].size() - (index + 1);

        for (int index = matrix.size()-2; index >= 0; index--)
        {
            for (int index2 = matrix[index].size() - 2; index2 >= 0;  index2--)
            {
                matrix[index][index2] = std::min(matrix[index + 1][index2], std::min(matrix[index + 1][index2 + 1], matrix[index][index2 + 1]));

                if (draft[index2] != original[index])
                {
                    matrix[index][index2]++;
                }
            }
        }
        return matrix[0][0];
}
QVector<Book> DataBase::searchBooks(std::string& name,std::string& author,std::string& ISBN)
{
    std::vector<Book> searchResult;
    for( auto book:availableBooks)
    {
        searchResult.push_back(book);
    }
    class comparator
    {
        std::string name;
        std::string author;
        std::string ISBN;
    public:
        comparator(std::string name,std::string author,std::string ISBN)
        {
            this->name = name;
            this->author = author;
            this->ISBN = ISBN;
        }
        bool operator ()(Book book1, Book book2)
        {
            int nameDistance = levenshteinDistance(name,book1.getName().toStdString());
            int authorDistance = levenshteinDistance(author,book1.getAuthor().toStdString());
            int ISBNDistance = levenshteinDistance(ISBN,book1.getISBN().toStdString());
            if(name == "")
                nameDistance=0;
            if(author == "")
                authorDistance=0;
            if(ISBN == "")
                ISBNDistance=0;

            float meanDistance1 = (nameDistance + authorDistance + ISBNDistance)/3.f;

             nameDistance = levenshteinDistance(name,book2.getName().toStdString());
            authorDistance = levenshteinDistance(author,book2.getAuthor().toStdString());
             ISBNDistance = levenshteinDistance(ISBN,book2.getISBN().toStdString());
            if(name == "")
                nameDistance=0;
            if(author == "")
                authorDistance=0;
            if(ISBN == "")
                ISBNDistance=0;

            float meanDistance2 = (nameDistance + authorDistance + ISBNDistance)/3.f;

            return meanDistance1>meanDistance2;
        }
    };

    std::sort(searchResult.begin(),searchResult.end(),comparator(name,author,ISBN));

    return QVector<Book>(searchResult.begin(),searchResult.end());
}

DataBase::~DataBase()
{
    std::ofstream fout("../Biblioteca/dataBase.txt");
    fout<<userList.size()<<'\n';
    for(auto& user : userList)
    {
        fout<<user.getUserName().toStdString()<<' '<<user.getPasswordHash()<<'\n';
        fout<<user.getBorrowedBooks().size()<<'\n';
        for(auto& book : user.getBorrowedBooks())
        {
            fout<<book.getName().toStdString()<<'\n'<<book.getAuthor().toStdString()<<'\n'<<book.getISBN().toStdString() << '\n' << book.getRemaingDays() <<'\n';
        }

    }
    fout<<availableBooks.size()<<'\n';
    for(auto& book : availableBooks)
    {
        fout<<book.getName().toStdString()<<'\n'<<book.getAuthor().toStdString()<<'\n'<<book.getISBN().toStdString()<<'\n';
    }
}
