#include "user.h"

User::User()
{

}

User::User(QString userName, QString passwordHash)
{
    this->userName=userName;
    this->passwordHash=passwordHash;
}
