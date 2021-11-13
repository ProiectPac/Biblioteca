#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <dataBase.h>

void MainWindow::setUpUserBar()
{
    QMenuBar* userBar = new QMenuBar(this);
    this->setMenuBar(userBar);
    QMenu* userMenu = new QMenu();
    userMenu->setTitle("User");
    QAction* logOutAction = new QAction();
    logOutAction->setText("Log out");
    connect(logOutAction,&QAction::triggered,this,&MainWindow::logOut);
    userMenu->addAction(logOutAction);
    userBar->addMenu(userMenu);
    QAction* deleteUserAction = new QAction();
    deleteUserAction->setText("Delete User");
    connect(deleteUserAction,&QAction::triggered,this,&MainWindow::deleteCurrentUser);
    userMenu->addAction(deleteUserAction);
}

const std::shared_ptr<DataBase> &MainWindow::getDataBase() const
{
    return dataBase;
}


MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setUser(User *newUser)
{
    user = newUser;
}

void MainWindow::setDataBase(const std::shared_ptr<DataBase> &newDataBase)
{
    dataBase = newDataBase;
}

MainWindow::MainWindow() : QMainWindow()
  , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setUpUserBar();
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
    if(userCredentials.first=="" && userCredentials.second==0)
    {
        QString message= "UserName and Password can't be empty";
        QErrorMessage *errorMessage = QErrorMessage::qtHandler();
        errorMessage->showMessage(message);
    }
    else
    {
        if(userCredentials.first=="")
        {
            QString message= "UserName can't be empty";
            QErrorMessage *errorMessage = QErrorMessage::qtHandler();
            errorMessage->showMessage(message);
        }
        if(userCredentials.second==0)
        {
            QString message= "Password can't be empty";
            QErrorMessage *errorMessage = QErrorMessage::qtHandler();
            errorMessage->showMessage(message);
        }
    }
    if(loginDialog.getAction()==LoginDialog::Actions::Login)
    {
        if(dataBase->findUser(userCredentials.first,userCredentials.second)!=nullptr)
        {
            auto foundUser = dataBase->findUser(userCredentials.first, userCredentials.second);
            this->setUser(foundUser);
        }
        else
        {
            QString message= "UserName/password combination is not correct.";
            QErrorMessage *errorMessage = QErrorMessage::qtHandler();
            errorMessage->showMessage(message);
        }
    }
    else if(loginDialog.getAction()==LoginDialog::Actions::Register)
    {
        dataBase->addUser(User(userCredentials.first, userCredentials.second));
        auto foundUser = dataBase->findUser(userCredentials.first, userCredentials.second);
        this->setUser(foundUser);
    }
}

void MainWindow::deleteCurrentUser()
{
    dataBase->removeUser(*user);
    logOut();
}


