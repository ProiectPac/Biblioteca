#include "mainwindow.h"
#include <QApplication>
#include <QErrorMessage>
#include "logindialog.h"
#include "database.h"
#include <memory>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    std::shared_ptr<DataBase> dataBase(new DataBase());
    w.setDataBase(dataBase);
    LoginDialog loginDialog;
    QErrorMessage *errorMessage = QErrorMessage::qtHandler();
    //QObject::connect(errorMessage,&QDialog::finished,nullptr,);
    bool hasError;
    do
    {
        loginDialog.exec();
        hasError=false;
        auto userCredentials = loginDialog.getUserCredentials();
        if(userCredentials.first=="" && userCredentials.second==0)
        {
            QString message= "UserName and Password can't be empty";
            errorMessage->showMessage(message);
            hasError=true;
        }
        else
        {
            if(userCredentials.first=="")
            {
                QString message= "UserName can't be empty";
                errorMessage->showMessage(message);
                hasError=true;
            }
            if(userCredentials.second==0)
            {
                QString message= "Password can't be empty";
                errorMessage->showMessage(message);
                hasError=true;
            }
        }
        if(hasError==false)
        {
            if(loginDialog.getAction()==LoginDialog::Actions::Login)
            {
                if(dataBase->findUser(userCredentials.first,userCredentials.second)!=nullptr)
                {
                    auto foundUser = dataBase->findUser(userCredentials.first, userCredentials.second);
                    w.setUser(foundUser);
                    w.show();
                }
                else
                {
                    QString message= "UserName/password combination is not correct.";
                    QErrorMessage *errorMessage = QErrorMessage::qtHandler();
                    errorMessage->showMessage(message);
                    hasError=true;
                }
            }
            else if(loginDialog.getAction()==LoginDialog::Actions::Register)
            {
                dataBase->addUser(User(userCredentials.first, userCredentials.second));
                auto foundUser = dataBase->findUser(userCredentials.first, userCredentials.second);
                w.setUser(foundUser);
                w.show();
            }
        }
    } while (hasError==true && loginDialog.getAction()!=LoginDialog::Actions::Nothing) ;
    return a.exec();
}
