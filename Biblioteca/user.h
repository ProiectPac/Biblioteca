#ifndef USER_H
#define USER_H

#include <QString>



class User
{
private:
    QString userName;
    unsigned int passwordHash;
public:
    User();
    User(QString userName, unsigned int passwordHash);

};

#endif // USER_H
