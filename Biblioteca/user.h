#ifndef USER_H
#define USER_H

#include <QString>



class User
{
private:
    QString userName;
    QString passwordHash;
public:
    User();
    User(QString userName, QString passwordHash);
};

#endif // USER_H
