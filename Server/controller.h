#ifndef CONTROLLER_H
#define CONTROLLER_H
#include <string>
#include <string.h>
#include <vector>
#include "tcpsocket.h"
#include "sqldatabase.h"
#include "book.h"
#include "user.h"
#include <string.h>
#include <QString>

class Controller
{
private:
    enum class Commands
    {
        Login,
        RegisterAccount,
        DeleteAccount,
        UpdateUserPassword,
        AddBook,
        RemoveBook,
        GetAvailableBooks,
        GetBorrowedBooks,
        GetBook,
        GetBorrowedBook,
        BorrowBook,
        ReturnBook,
        SearchAvailableBooks,
        SearchBorrowedBooks,
        LogOut,
        None
    };
    User loggedUser;

    Commands interpret(std::string const& message);
    TCPSocket *client=nullptr;
    SQLDataBase dataBase;
    void login(std::vector<std::string> message);
    void registerAccount(std::vector<std::string> message);
    void deleteAccount(std::vector<std::string>message);
    void updateUserPassword(std::vector<std::string> message);
    void addBook(std::vector<std::string>message);
    void removeBook(std::vector<std::string>message);
    void getBook(std::vector<std::string>message);
    void getBorrowedBook(std::vector<std::string>message);
    void borrowBook(std::vector<std::string>message);
    void returnBook(std::vector<std::string>message);
    void receiveComand();
    Controller() = default;
public:
    void static run(SOCKET client);
};

#endif // CONTROLLER_H
