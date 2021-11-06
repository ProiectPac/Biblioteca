#include "user.h"

User::User()
{

}

User::User(QString userName, unsigned int passwordHash)
{
    this->userName=userName;
    this->passwordHash=passwordHash;
}
