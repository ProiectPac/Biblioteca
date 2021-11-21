#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <database.h>

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
    QAction* changeUserPasswordAction = new QAction();
    changeUserPasswordAction->setText("Change Password User");
    //connect(deleteUserAction,&QAction::triggered,this,&MainWindow::deleteCurrentUser);
    userMenu->addAction(changeUserPasswordAction);
}

void MainWindow::loginDialogFinished()
{
    QErrorMessage* errorMessage = QErrorMessage::qtHandler();

    auto userCredentials = loginDialog->getUserCredentials();

    hasError=false;

    if(loginDialog->getAction()!=LoginDialog::Actions::Nothing)
    {
        if(userCredentials.first=="" && userCredentials.second==0)
        {
            QString message= "UserName and Password can't be empty";
            errorMessage->showMessage(message);

            delete loginDialog;
            loginDialog = new LoginDialog(this);
            connect(loginDialog,&LoginDialog::finished,this,&MainWindow::loginDialogFinished);
            loginDialog->open();

            hasError=true;

        }
        else
        {
            if(userCredentials.first=="")
            {
                QString message= "UserName can't be empty";
                errorMessage->showMessage(message);

                delete loginDialog;
                loginDialog = new LoginDialog(this);
                connect(loginDialog,&LoginDialog::finished,this,&MainWindow::loginDialogFinished);
                loginDialog->open();

                hasError=true;
            }
            if(userCredentials.second==0)
            {
                QString message= "Password can't be empty";

                delete loginDialog;
                loginDialog = new LoginDialog(this);
                connect(loginDialog,&LoginDialog::finished,this,&MainWindow::loginDialogFinished);
                loginDialog->open();

                errorMessage->showMessage(message);
                hasError=true;
            }
        }
        if(hasError==false)
        {
            if(loginDialog->getAction()==LoginDialog::Actions::Login)
            {
                if(dataBase->findUser(userCredentials.first,userCredentials.second)!=nullptr)
                {
                    auto foundUser = dataBase->findUser(userCredentials.first, userCredentials.second);
                    setUser(foundUser);
                }
                else
                {
                    QString message= "UserName/password combination is not correct.";
                    QErrorMessage *errorMessage = QErrorMessage::qtHandler();

                    delete loginDialog;
                    loginDialog = new LoginDialog(this);
                    connect(loginDialog,&LoginDialog::finished,this,&MainWindow::loginDialogFinished);
                    loginDialog->open();

                    errorMessage->showMessage(message);
                    hasError=true;
                }
            }
            else if(loginDialog->getAction()==LoginDialog::Actions::Register)
            {
                dataBase->addUser(User(userCredentials.first, userCredentials.second));
                auto foundUser = dataBase->findUser(userCredentials.first, userCredentials.second);
                setUser(foundUser);
            }
        }
    }
    else
        this->close();
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

MainWindow::MainWindow() : QMainWindow()
  , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setUpUserBar();
    dataBase = std::shared_ptr<DataBase>(new DataBase());

    delete loginDialog;
    loginDialog = new LoginDialog(this);
    connect(loginDialog,&LoginDialog::finished,this,&MainWindow::loginDialogFinished);
    loginDialog->open();

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
    delete loginDialog;
    loginDialog = new LoginDialog(this);
    connect(loginDialog,&LoginDialog::finished,this,&MainWindow::loginDialogFinished);
    loginDialog->open();
}

void MainWindow::deleteCurrentUser()
{
    dataBase->removeUser(*user);
    logOut();
}


