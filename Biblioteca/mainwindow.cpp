#include "mainwindow.h"
#include "ui_mainwindow.h"

void MainWindow::setUpUserBar()
{
    userBar = new QMenuBar(this);
    this->setMenuBar(userBar);
    userMenu = new QMenu(this);
    userMenu->setTitle("User");
    logOutAction = new QAction(this);
    logOutAction->setText("Log out");
    connect(logOutAction,&QAction::triggered,this,&MainWindow::logOut);
    userMenu->addAction(logOutAction);
    userBar->addMenu(userMenu);
    deleteUserAction = new QAction(this);
    deleteUserAction->setText("Delete User");
    connect(deleteUserAction,&QAction::triggered,this,&MainWindow::deleteCurrentUser);
    userMenu->addAction(deleteUserAction);
    QAction* changeUserPasswordAction = new QAction(this);
    changeUserPasswordAction->setText("Change Password User");
    connect(changeUserPasswordAction,&QAction::triggered,this,&MainWindow::changeCurrentUserPassword);
    userMenu->addAction(changeUserPasswordAction);

    QAction *addBookAction = new QAction(this);
    userMenu->addAction(addBookAction);

    addBookAction->setText("Add book");
    connect(addBookAction,&QAction::triggered,this,&MainWindow::addNewBook);


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
                auto foundUser = dataBase.findUser(userCredentials.first, userCredentials.second);
                if(foundUser.getUserName()!="")
                {
                    setUser(foundUser);

                    availableBooksModel = new TreeModel(dataBase.getAvailableBooks(currentAvailableBooksPage),false);
                    borrowedBooksModel = new TreeModel(dataBase.getBorrowedBooks(currentBorrowedBooksPage,currentUser.getUserName()),true);

                    availableBooksList->setModel(availableBooksModel);
                    borrowedBooksList->setModel(borrowedBooksModel);
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
                dataBase.addUser(User(userCredentials.first, userCredentials.second));
                auto foundUser = dataBase.findUser(userCredentials.first, userCredentials.second);
                setUser(foundUser);                

                availableBooksModel = new TreeModel(dataBase.getAvailableBooks(currentAvailableBooksPage),false);
                borrowedBooksModel = new TreeModel(dataBase.getBorrowedBooks(currentBorrowedBooksPage,currentUser.getUserName()),true);

                availableBooksList->setModel(availableBooksModel);
                borrowedBooksList->setModel(borrowedBooksModel);
            }
        }
    }
    else
        this->close();
}

void MainWindow::setUpUI()
{
    resize(1920, 1080);

    bookContent = new QTextBrowser(this);
    bookContent->setPlainText("Book content");


    availableBooksList = new QTreeView(this);
    borrowedBooksList = new QTreeView(this);

    QGroupBox* availableBookBox = new QGroupBox(this);

    QGroupBox* availableBooksSearchBox = new QGroupBox(this);
    QVBoxLayout* availableBooksLayout = new QVBoxLayout();

    QFormLayout* availableBooksSearch = new QFormLayout();
    QLineEdit* availabeleBooksNameLineEdit = new QLineEdit(this);
    QLineEdit* availabeleBooksAuthorLineEdit = new QLineEdit(this);
    QLineEdit* availabeleBooksISBNLineEdit = new QLineEdit(this);

    availableBooksSearch->addRow("Name:",availabeleBooksNameLineEdit);
    availableBooksSearch->addRow("Author:",availabeleBooksAuthorLineEdit);
    availableBooksSearch->addRow("ISBN:",availabeleBooksISBNLineEdit);

    availableBooksSearchBox->setLayout(availableBooksSearch);

    availableBooksLayout->addWidget(availableBooksSearchBox);
    availableBooksLayout->addWidget(availableBooksList);

    QGroupBox* availableBooksNavigationBox = new QGroupBox(this);
    QHBoxLayout* availableBooksNavigation = new QHBoxLayout();

    QPushButton* previousAvailableBooksButton = new QPushButton(this);
    QPushButton* nextAvailableBooksButton = new QPushButton(this);

    previousAvailableBooksButton->setText("Previous");
    nextAvailableBooksButton->setText("Next");

    connect(previousAvailableBooksButton,&QPushButton::clicked,this,&MainWindow::previousAvailableBooksButtonOnClick);
    connect(nextAvailableBooksButton,&QPushButton::clicked,this,&MainWindow::nextAvailableBooksButtonOnClick);

    QLineEdit* currentPageLineEdit = new QLineEdit(this);

    availableBooksNavigation->addWidget(previousAvailableBooksButton);
    availableBooksNavigation->addWidget(currentPageLineEdit);
    availableBooksNavigation->addWidget(nextAvailableBooksButton);

    availableBooksNavigationBox->setLayout(availableBooksNavigation);

    availableBooksLayout->addWidget(availableBooksNavigationBox);



    availableBookBox->setLayout(availableBooksLayout);

    QGroupBox* borrowedBookBox = new QGroupBox(this);

    QGroupBox* borrowedBooksSearchBox = new QGroupBox(this);
    QVBoxLayout* borrowedBooksLayout = new QVBoxLayout();

    QFormLayout* borrowedBooksSearch = new QFormLayout();
    QLineEdit* borrowedBooksNameLineEdit = new QLineEdit(this);
    QLineEdit* borrowedBooksAuthorLineEdit = new QLineEdit(this);
    QLineEdit* borrowedBooksISBNLineEdit = new QLineEdit(this);

    borrowedBooksSearch->addRow("Name:",borrowedBooksNameLineEdit);
    borrowedBooksSearch->addRow("Author:",borrowedBooksAuthorLineEdit);
    borrowedBooksSearch->addRow("ISBN:",borrowedBooksISBNLineEdit);

    borrowedBooksSearchBox->setLayout(borrowedBooksSearch);

    borrowedBooksLayout->addWidget(borrowedBooksSearchBox);
    borrowedBooksLayout->addWidget(borrowedBooksList);

    QGroupBox* borrowedBooksNavigationBox = new QGroupBox(this);
    QHBoxLayout* borrowedBooksNavigation = new QHBoxLayout();

    QPushButton* previousBorrowedBooksButton = new QPushButton(this);
    QPushButton* nextBorrowedBooksButton = new QPushButton(this);

    previousBorrowedBooksButton->setText("Previous");
    nextBorrowedBooksButton->setText("Next");

    connect(previousBorrowedBooksButton,&QPushButton::clicked,this,&MainWindow::previousBorrowedBooksButtonOnClick);
    connect(nextBorrowedBooksButton,&QPushButton::clicked,this,&MainWindow::nextBorrowedBooksButtonOnClick);

    QLineEdit* borrowedCurrentPageLineEdit = new QLineEdit(this);

    borrowedBooksNavigation->addWidget(previousBorrowedBooksButton);
    borrowedBooksNavigation->addWidget(borrowedCurrentPageLineEdit);
    borrowedBooksNavigation->addWidget(nextBorrowedBooksButton);

    borrowedBooksNavigationBox->setLayout(borrowedBooksNavigation);

    borrowedBooksLayout->addWidget(borrowedBooksNavigationBox);

    borrowedBookBox->setLayout(borrowedBooksLayout);

    layout = new BorderLayout();
    layout->addWidget(bookContent, BorderLayout::Center);
    layout->addWidget(availableBookBox, BorderLayout::West);
    layout->addWidget(borrowedBookBox, BorderLayout::East);

    QWidget *window = new QWidget();
    window->setLayout(layout);

    setCentralWidget(window);
}


MainWindow::~MainWindow()
{
    delete loginDialog;
    delete userBar;
    delete userMenu;
    delete logOutAction;
    delete deleteUserAction;
    delete bookContent;
    delete availableBooksList;
    delete borrowedBooksList;
    delete layout;
}

void MainWindow::setUser(User newUser)
{
    currentUser = newUser;
}

MainWindow::MainWindow() : QMainWindow()
{
    setUpUI();
    setUpUserBar();

    delete loginDialog;
    loginDialog = new LoginDialog(this);
    connect(loginDialog,&LoginDialog::finished,this,&MainWindow::loginDialogFinished);
    loginDialog->open();    

}

void MainWindow::borrowBook(int bookID)
{
    dataBase.borrowBook(currentUser.getUserName(), bookID);
}

void MainWindow::returnBook(int bookID)
{
    dataBase.returnBook(currentUser.getUserName(), bookID);
}

void MainWindow::logOut()
{
    currentAvailableBooksPage=0;
    currentBorrowedBooksPage=0;
    delete loginDialog;
    loginDialog = new LoginDialog(this);
    connect(loginDialog,&LoginDialog::finished,this,&MainWindow::loginDialogFinished);
    loginDialog->open();
}

void MainWindow::deleteCurrentUser()
{
    dataBase.removeUser(currentUser.getUserName());
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
            currentUser.setPasswordHash(LoginDialog::FNVHash(newPassword));
            dataBase.updateUserPassword(currentUser.getUserName(), currentUser.getPasswordHash());
        }
    }
    delete passwordDialog;
    delete repeatPasswordDialog;
}

void MainWindow::nextAvailableBooksButtonOnClick()
{
    currentAvailableBooksPage++;
    delete availableBooksModel;

    availableBooksModel = new TreeModel(dataBase.getAvailableBooks(currentAvailableBooksPage),false);
    availableBooksList->setModel(availableBooksModel);
}

void MainWindow::previousAvailableBooksButtonOnClick()
{
    currentAvailableBooksPage--;
    delete availableBooksModel;

    availableBooksModel = new TreeModel(dataBase.getAvailableBooks(currentAvailableBooksPage),false);
    availableBooksList->setModel(availableBooksModel);
}

void MainWindow::nextBorrowedBooksButtonOnClick()
{
    currentBorrowedBooksPage++;
    delete borrowedBooksModel;

    borrowedBooksModel = new TreeModel(dataBase.getBorrowedBooks(currentBorrowedBooksPage,currentUser.getUserName()),true);
    borrowedBooksList->setModel(borrowedBooksModel);
}

void MainWindow::previousBorrowedBooksButtonOnClick()
{
    currentBorrowedBooksPage--;
    delete borrowedBooksModel;

    borrowedBooksModel = new TreeModel(dataBase.getBorrowedBooks(currentBorrowedBooksPage,currentUser.getUserName()),true);
    borrowedBooksList->setModel(borrowedBooksModel);
}

void MainWindow::addNewBook()
{
    delete newBookDialog;
    newBookDialog = new NewBookDialog(this);

    connect(newBookDialog,&QDialog::finished,this,&MainWindow::addBookFinished);

    newBookDialog->show();

}
 void MainWindow::addBookFinished()
 {
     if(newBookDialog->result() == QDialog::Accepted)
     {
         Book newBook = newBookDialog->getBook();
         dataBase.addBook(newBook);
     }

 }

