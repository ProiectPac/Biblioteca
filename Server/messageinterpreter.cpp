#include "messageinterpreter.h"

MessageInterpreter::MessageInterpreter()
{

}


MessageInterpreter::Commands MessageInterpreter::interpret(const std::string &message)
{
    if(message=="findUser")
        return Commands::FindUser;

    if(message=="addUser")
        return Commands::AddUser;

    if(message=="removeUser")
        return Commands::RemoveUser;

    if(message=="updateUserPassword")
        return Commands::UpdateUserPassword;

    if(message=="addBook")
        return Commands::AddBook;

    if(message=="removeBook")
        return Commands::RemoveBook;

    if(message=="getAvailableBooks")
        return Commands::GetAvailableBooks;

    if(message=="getBorrowedBooks")
        return Commands::GetBorrowedBooks;

    if(message=="getBook")
        return Commands::GetBook;

    if(message=="getBorrowedBook")
        return Commands::GetBorrowedBook;

    if(message=="borrowBook")
        return Commands::BorrowBook;

    if(message=="returnBook")
        return Commands::ReturnBook;

    if(message=="searchAvailableBooks")
        return Commands::SearchAvailableBooks;

    if(message=="searchBorrowedBooks")
        return Commands::SearchBorrowedBooks;

    if(message=="logOut")
        return Commands::LogOut;

    return Commands::None;
}
