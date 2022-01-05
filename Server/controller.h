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
        FindUser,
        AddUser,
        RemoveUser,
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

    Commands interpret(std::string const& message);
    TCPSocket *client=nullptr;
    SQLDataBase dataBase;
    void findUser(std::vector<std::string> message);
    void receiveComand();
    Controller() = default;
public:
    void static run(SOCKET client);
};

#endif // CONTROLLER_H
