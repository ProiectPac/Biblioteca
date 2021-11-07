#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(User& user, DataBase& database,QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->user=user;
    this->database=database;
    QMenuBar* userBar = new QMenuBar();
    QMenu* logOut = new QMenu();
    QAction* logOutAction = new QAction();
    logOutAction->setStatusTip("log out");
    connect(logOutAction,&QAction::triggered,this,&MainWindow::logOut);
    logOut->addAction(logOutAction);
    userBar->addMenu(logOut);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::addBorrowBook(Book book)
{
    database.removeBook(book);
    book.setRemaingDays(14);
    user.addBorrowedBook(book);
}

void MainWindow::deleteBorrowBook(Book book)
{
    user.removeBorrowedBook(book);
    book.setRemaingDays(-1);
    database.addAvailableBook(book);
}

void MainWindow::logOut()
{

    LoginDialog loginDialog;
    loginDialog.exec();
    DataBase dataBase;
    auto userCredentials = loginDialog.getUserCredentials();

    if(loginDialog.getAction()==LoginDialog::Actions::Login)
    {
        if(dataBase.existsUser(userCredentials.first,userCredentials.second))
        {
            auto foundUser = dataBase.findUser(userCredentials.first, userCredentials.second);
            this->close();
            MainWindow w(foundUser,dataBase);
            w.show();
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
        User user = User(userCredentials.first, userCredentials.second);
        dataBase.addUser(user);
        this->close();
        MainWindow w(user,dataBase);
        w.show();
    }
}


