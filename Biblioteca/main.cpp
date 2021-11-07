#include "mainwindow.h"

#include <QApplication>
#include <QErrorMessage>
#include "logindialog.h"
#include "database.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    LoginDialog loginDialog;
    loginDialog.exec();
    DataBase dataBase;
    auto userCredentials = loginDialog.getUserCredentials();

    if(loginDialog.getAction()==LoginDialog::Actions::Login)
    {
        auto foundUser = dataBase.findUser(userCredentials.first, userCredentials.second);
        if(foundUser.getUserName()!="")
        {
            MainWindow w;
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
        dataBase.addUser(User(userCredentials.first, userCredentials.second));
        MainWindow w;
        w.show();
        return a.exec();
    }

    return 0;
}
