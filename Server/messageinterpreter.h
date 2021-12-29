#ifndef MESSAGEINTERPRETER_H
#define MESSAGEINTERPRETER_H
#include <string>

class MessageInterpreter
{
public:
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

public:
    MessageInterpreter();
    Commands interpret(std::string const& message);
};

#endif // MESSAGEINTERPRETER_H
