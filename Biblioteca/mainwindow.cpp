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
<<<<<<< Updated upstream
    QMenuBar* userBar = new QMenuBar(this);
=======
<<<<<<< HEAD
    QMenuBar* userBar = new QMenuBar();
=======
    QMenuBar* userBar = new QMenuBar(this);
>>>>>>> 7ad67e59a637ca93fc9d7a94b95fd98e79bb3f55
>>>>>>> Stashed changes
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
<<<<<<< Updated upstream
    dataBase.removeBook(book);
=======
<<<<<<< HEAD
    dataBase->removeBook(book);
=======
    dataBase.removeBook(book);
>>>>>>> 7ad67e59a637ca93fc9d7a94b95fd98e79bb3f55
>>>>>>> Stashed changes
    book.setRemaingDays(14);
    user->addBorrowedBook(book);
}

void MainWindow::deleteBorrowBook(Book book)
{
    user->removeBorrowedBook(book);
    book.setRemaingDays(-1);
<<<<<<< Updated upstream
    dataBase.addAvailableBook(book);
=======
<<<<<<< HEAD
    dataBase->addAvailableBook(book);
=======
    dataBase.addAvailableBook(book);
>>>>>>> 7ad67e59a637ca93fc9d7a94b95fd98e79bb3f55
>>>>>>> Stashed changes
}

void MainWindow::logOut()
{

    LoginDialog loginDialog;
    loginDialog.exec();
<<<<<<< Updated upstream

=======
<<<<<<< HEAD
=======

>>>>>>> 7ad67e59a637ca93fc9d7a94b95fd98e79bb3f55
>>>>>>> Stashed changes
    auto userCredentials = loginDialog.getUserCredentials();

    if(loginDialog.getAction()==LoginDialog::Actions::Login)
    {
        if(dataBase->findUser(userCredentials.first,userCredentials.second)!=nullptr)
        {
            auto foundUser = dataBase->findUser(userCredentials.first, userCredentials.second);
            this->close();
            MainWindow w(foundUser,dataBase);
            w.show();
        }
        else
        {
            this->close();
            QString message= "UserName/password combination is not correct.";
            QErrorMessage *errorMessage = QErrorMessage::qtHandler();
            errorMessage->showMessage(message);            
        }
    }
    else if(loginDialog.getAction()==LoginDialog::Actions::Register)
    {
        User user = User(userCredentials.first, userCredentials.second);
        dataBase->addUser(user);
        this->close();
        MainWindow w(&user,dataBase);
        w.show();
    }
}


