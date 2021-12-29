#include "user.h"

const QString &User::getUserName() const
{
    return userName;
}

unsigned int User::getPasswordHash() const
{
    return passwordHash;
}

void User::setUserName(const QString &newUserName)
{
    userName = newUserName;
}

void User::setPasswordHash(const unsigned int& newPasswordHash)
{
    passwordHash = newPasswordHash;
}

User::User()
{
    userName="";
    passwordHash=0;
}

User::~User()
{

}

User::User(const QString& userName, const unsigned int& passwordHash)
{
    this->userName=userName;
    this->passwordHash=passwordHash;
}

