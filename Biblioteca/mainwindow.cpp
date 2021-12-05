#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <database.h>

void MainWindow::setUpUserBar()
{
    userBar = new QMenuBar(this);
    this->setMenuBar(userBar);
    userMenu = new QMenu();
    userMenu->setTitle("User");
    logOutAction = new QAction();
    logOutAction->setText("Log out");
    connect(logOutAction,&QAction::triggered,this,&MainWindow::logOut);
    userMenu->addAction(logOutAction);
    userBar->addMenu(userMenu);
    deleteUserAction = new QAction();
    deleteUserAction->setText("Delete User");
    connect(deleteUserAction,&QAction::triggered,this,&MainWindow::deleteCurrentUser);
    userMenu->addAction(deleteUserAction);
    QAction* changeUserPasswordAction = new QAction();
    changeUserPasswordAction->setText("Change Password User");
    connect(changeUserPasswordAction,&QAction::triggered,this,&MainWindow::changeCurrentUserPassword);
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
            connect(errorMessage,&QErrorMessage::finished, loginDialog,&LoginDialog::open);
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
                connect(errorMessage,&QErrorMessage::finished, loginDialog,&LoginDialog::open);

                hasError=true;
            }
            if(userCredentials.second==0)
            {
                QString message= "Password can't be empty";

                delete loginDialog;
                loginDialog = new LoginDialog(this);
                connect(loginDialog,&LoginDialog::finished,this,&MainWindow::loginDialogFinished);
                connect(errorMessage,&QErrorMessage::finished, loginDialog,&LoginDialog::open);

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
                    connect(errorMessage,&QErrorMessage::finished, loginDialog,&LoginDialog::open);

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

void MainWindow::setUpUI()
{
    resize(1920, 1080);

    bookContent = new QTextBrowser;
    bookContent->setPlainText("Book content");


    availableBooksList = new QListView;
    borrowedBooksList = new QListView;

    QGroupBox* availableBookBox = new QGroupBox;

    QGroupBox* availableBooksSearchBox = new QGroupBox;
    QVBoxLayout* availableBooksLayout = new QVBoxLayout;

    QFormLayout* availableBooksSearch = new QFormLayout;
    QLineEdit* availabeleBooksNameLineEdit = new QLineEdit;
    QLineEdit* availabeleBooksAuthorLineEdit = new QLineEdit;
    QLineEdit* availabeleBooksISBNLineEdit = new QLineEdit;

    availableBooksSearch->addRow("Name:",availabeleBooksNameLineEdit);
    availableBooksSearch->addRow("Author:",availabeleBooksAuthorLineEdit);
    availableBooksSearch->addRow("ISBN:",availabeleBooksISBNLineEdit);

    availableBooksSearchBox->setLayout(availableBooksSearch);

    availableBooksLayout->addWidget(availableBooksSearchBox);
    availableBooksLayout->addWidget(availableBooksList);

    QGroupBox* availableBooksNavigationBox = new QGroupBox;
    QHBoxLayout* availableBooksNavigation = new QHBoxLayout;

    QPushButton* previousAvailableBooksButton = new QPushButton;
    QPushButton* nextAvailableBooksButton = new QPushButton;

    previousAvailableBooksButton->setText("Previous");
    nextAvailableBooksButton->setText("Next");

    QLineEdit* currentPageLineEdit = new QLineEdit;

    availableBooksNavigation->addWidget(previousAvailableBooksButton);
    availableBooksNavigation->addWidget(currentPageLineEdit);
    availableBooksNavigation->addWidget(nextAvailableBooksButton);

    availableBooksNavigationBox->setLayout(availableBooksNavigation);

    availableBooksLayout->addWidget(availableBooksNavigationBox);



    availableBookBox->setLayout(availableBooksLayout);

    QGroupBox* borrowedBookBox = new QGroupBox;

    QGroupBox* borrowedBooksSearchBox = new QGroupBox;
    QVBoxLayout* borrowedBooksLayout = new QVBoxLayout;

    QFormLayout* borrowedBooksSearch = new QFormLayout;
    QLineEdit* borrowedBooksNameLineEdit = new QLineEdit;
    QLineEdit* borrowedBooksAuthorLineEdit = new QLineEdit;
    QLineEdit* borrowedBooksISBNLineEdit = new QLineEdit;

    borrowedBooksSearch->addRow("Name:",borrowedBooksNameLineEdit);
    borrowedBooksSearch->addRow("Author:",borrowedBooksAuthorLineEdit);
    borrowedBooksSearch->addRow("ISBN:",borrowedBooksISBNLineEdit);

    borrowedBooksSearchBox->setLayout(borrowedBooksSearch);

    borrowedBooksLayout->addWidget(borrowedBooksSearchBox);
    borrowedBooksLayout->addWidget(borrowedBooksList);

    borrowedBookBox->setLayout(borrowedBooksLayout);

    layout = new BorderLayout;
    layout->addWidget(bookContent, BorderLayout::Center);
    layout->addWidget(availableBookBox, BorderLayout::West);
    layout->addWidget(borrowedBookBox, BorderLayout::East);

    QWidget *window = new QWidget();
    window->setLayout(layout);

    setCentralWidget(window);
}

const std::shared_ptr<DataBase> MainWindow::getDataBase() const
{
    return dataBase;
}


MainWindow::~MainWindow()
{ 
    delete userBar;
    delete userMenu;
    delete logOutAction;
    delete deleteUserAction;
    delete loginDialog;
    delete bookContent;
    delete availableBooksList;
    delete borrowedBooksList;
    delete layout;
    delete ui;
}

void MainWindow::setUser( User *newUser)
{
    user = newUser;
}

MainWindow::MainWindow() : QMainWindow()
{
    setUpUI();
    setUpUserBar();
    dataBase = std::shared_ptr<DataBase>(new DataBase());
    delete loginDialog;
    loginDialog = new LoginDialog(this);
    connect(loginDialog,&LoginDialog::finished,this,&MainWindow::loginDialogFinished);
    loginDialog->open();
}

void MainWindow::addBorrowBook(Book& book)
{
    dataBase->removeAvailableBook(book);
    book.setRemainingDays(14);
    user->addBorrowedBook(book);
}

void MainWindow::deleteBorrowBook(Book& book)
{
    user->removeBorrowedBook(book);
    book.setRemainingDays(-1);
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

void MainWindow::changeCurrentUserPassword()
{
    QInputDialog *passwordDialog = new QInputDialog;
    passwordDialog->setWindowTitle("Change Password");
    passwordDialog->setLabelText("New Password");
    QInputDialog *repeatPasswordDialog = new QInputDialog;
    repeatPasswordDialog->setWindowTitle("Change Password");
    repeatPasswordDialog->setLabelText("Repeat Password");
    passwordDialog->exec();
    repeatPasswordDialog->exec();
    QString newPassword=passwordDialog->textValue();
    QString repeatPassword=repeatPasswordDialog->textValue();
    if(passwordDialog->result()==QDialog::Accepted && repeatPasswordDialog->result()==QDialog::Accepted)
    {
        if(newPassword==""||repeatPassword=="")
        {
            QString message= "Password can't be empty";
            QErrorMessage *errorMessage = QErrorMessage::qtHandler();
            errorMessage->showMessage(message);
        }
        else if(newPassword!=repeatPassword)
        {
            QString message= "Passwords do not match";
            QErrorMessage *errorMessage = QErrorMessage::qtHandler();
            errorMessage->showMessage(message);
        }
        else
        {
            user->setPasswordHash(LoginDialog::FNVHash(newPassword));
        }
    }
    delete passwordDialog;
    delete repeatPasswordDialog;
}


