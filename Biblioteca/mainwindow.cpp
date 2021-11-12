#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <dataBase.h>

MainWindow::MainWindow(User* user, std::shared_ptr<DataBase> database ,QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->user=user;
    this->dataBase=database;
    QMenuBar* userBar = new QMenuBar(this);
    QMenu* logOut = new QMenu();
    logOut->setTitle("User");
    QAction* logOutAction = new QAction();
    logOutAction->setText("Log out");
    connect(logOutAction,&QAction::triggered,this,&MainWindow::logOut);
    logOut->addAction(logOutAction);
    userBar->addMenu(logOut);
    this->setMenuBar(userBar);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::addBorrowBook(Book book)
{
    dataBase->removeBook(book);
    book.setRemaingDays(14);
    user->addBorrowedBook(book);
}

void MainWindow::deleteBorrowBook(Book book)
{
    user->removeBorrowedBook(book);
    book.setRemaingDays(-1);
    dataBase->addAvailableBook(book);
}

void MainWindow::logOut()
{

    LoginDialog loginDialog;
    loginDialog.exec();
    auto userCredentials = loginDialog.getUserCredentials();

    if(loginDialog.getAction()==LoginDialog::Actions::Login)
    {
        if(dataBase->findUser(userCredentials.first,userCredentials.second)!=nullptr)
        {
            auto foundUser = dataBase->findUser(userCredentials.first, userCredentials.second);
            //this->close();
            MainWindow w(foundUser,dataBase);
            w.show();
        }
        else
        {
            //this->close();
            QString message= "UserName/password combination is not correct.";
            QErrorMessage *errorMessage = QErrorMessage::qtHandler();
            errorMessage->showMessage(message);            
        }
    }
    else if(loginDialog.getAction()==LoginDialog::Actions::Register)
    {
        User user = User(userCredentials.first, userCredentials.second);
        dataBase->addUser(user);
        //this->close();
        MainWindow w(&user,dataBase);
        w.show();
    }
}


