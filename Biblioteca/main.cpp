#include "mainwindow.h"

#include <QApplication>
#include <QErrorMessage>
#include "logindialog.h"
#include "database.h"
#include <memory>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    LoginDialog loginDialog;
    loginDialog.exec();
    std::shared_ptr<DataBase> dataBase(new DataBase());
    auto userCredentials = loginDialog.getUserCredentials();
    if(userCredentials.first=="" && userCredentials.second==0)
    {
        QString message= "UserName and Password can't be empty";
        QErrorMessage *errorMessage = QErrorMessage::qtHandler();
        errorMessage->showMessage(message);
        return a.exec();
    }
    if(userCredentials.first=="")
    {
        QString message= "UserName can't be empty";
        QErrorMessage *errorMessage = QErrorMessage::qtHandler();
        errorMessage->showMessage(message);
        return a.exec();
    }
    if(userCredentials.second==0)
    {
        QString message= "Password can't be empty";
        QErrorMessage *errorMessage = QErrorMessage::qtHandler();
        errorMessage->showMessage(message);
        return a.exec();
    }
    if(loginDialog.getAction()==LoginDialog::Actions::Login)
    {
        if(dataBase->findUser(userCredentials.first,userCredentials.second)!=nullptr)
        {
            auto foundUser = dataBase->findUser(userCredentials.first, userCredentials.second);
            MainWindow w(foundUser,dataBase);
            w.show();
            return a.exec();
        }
        else
        {
            QString message= "UserName/password combination is not correct.";
            QErrorMessage *errorMessage = QErrorMessage::qtHandler();
            errorMessage->showMessage(message);
            return a.exec();
        }
    }
    else if(loginDialog.getAction()==LoginDialog::Actions::Register)
    {
        User user = User(userCredentials.first, userCredentials.second);
        dataBase->addUser(user);
        MainWindow w(&user,dataBase);
        w.show();
        return a.exec();
    }

    return 0;
}
