#include "mainwindow.h"
#include "ui_mainwindow.h"

void MainWindow::setUpUserBar()
{
    userBar = new QMenuBar(this);
    this->setMenuBar(userBar);

    userMenu = new QMenu(this);
    userBar->addMenu(userMenu);
    userMenu->setTitle("User");

    logOutAction = new QAction(this);
    logOutAction->setText("Log out");
    connect(logOutAction,&QAction::triggered,this,&MainWindow::logOut);
    userMenu->addAction(logOutAction);

    deleteUserAction = new QAction(this);
    deleteUserAction->setText("Delete User");
    connect(deleteUserAction,&QAction::triggered,this,&MainWindow::deleteCurrentUser);
    userMenu->addAction(deleteUserAction);

    QAction* changeUserPasswordAction = new QAction(this);
    changeUserPasswordAction->setText("Change Password User");
    connect(changeUserPasswordAction,&QAction::triggered,this,&MainWindow::changeCurrentUserPassword);
    userMenu->addAction(changeUserPasswordAction);

    QMenu *bookMenu = new QMenu(this);
    userBar->addMenu(bookMenu);
    bookMenu->setTitle("Book");

    QAction *addBookAction = new QAction(this);
    addBookAction->setText("Add book");
    connect(addBookAction,&QAction::triggered,this,&MainWindow::addNewBook);
    bookMenu->addAction(addBookAction);

    QAction *deleteBookAction = new QAction(this);
    deleteBookAction->setText("Delete book");
    connect(deleteBookAction,&QAction::triggered,this,&MainWindow::deleteBook);
    bookMenu->addAction(deleteBookAction);
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

                    availableBooksModel = new TreeModel(dataBase.getAvailableBooks(availableBooksCurrentPage),false);
                    borrowedBooksModel = new TreeModel(dataBase.getBorrowedBooks(borrowedBooksCurrentPage,currentUser.getUserName()),true);

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

                availableBooksModel = new TreeModel(dataBase.getAvailableBooks(availableBooksCurrentPage),false);
                borrowedBooksModel = new TreeModel(dataBase.getBorrowedBooks(borrowedBooksCurrentPage,currentUser.getUserName()),true);

                availableBooksList->setModel(availableBooksModel);
                borrowedBooksList->setModel(borrowedBooksModel);
            }
        }
    }
    else
        this->close();
}

void MainWindow::deleteBookDialogFinished()
{
    int bookId = deleteBookDialog->getBookID();
    dataBase.removeBook(bookId);
}

void MainWindow::setUpUI()
{
    resize(1920, 1080);

    bookContent = new QTextBrowser(this);
    bookContent->setPlainText("Book content");


    availableBooksList = new QTreeView(this);
    borrowedBooksList = new QTreeView(this);

    connect(availableBooksList,&QAbstractItemView::doubleClicked,this,&MainWindow::borrowBook);
    connect(borrowedBooksList,&QAbstractItemView::doubleClicked,this,&MainWindow::returnBook);


    QGroupBox* availableBookBox = new QGroupBox(this);

    QGroupBox* availableBooksSearchBox = new QGroupBox(this);
    QVBoxLayout* availableBooksLayout = new QVBoxLayout();

    QFormLayout* availableBooksSearch = new QFormLayout();
    availableBooksNameLineEdit = new QLineEdit(this);
    availableBooksAuthorLineEdit = new QLineEdit(this);
    availableBooksISBNLineEdit = new QLineEdit(this);

    connect(availableBooksNameLineEdit,&QLineEdit::returnPressed,this,&MainWindow::availableBooksNameLineEditReturnPressed);
    connect(availableBooksAuthorLineEdit,&QLineEdit::returnPressed,this,&MainWindow::availableBooksAuthorLineEditReturnPressed);
    connect(availableBooksISBNLineEdit,&QLineEdit::returnPressed,this,&MainWindow::availableBooksISBNLineEditReturnPressed);

    availableBooksSearch->addRow("Name:",availableBooksNameLineEdit);
    availableBooksSearch->addRow("Author:",availableBooksAuthorLineEdit);
    availableBooksSearch->addRow("ISBN:",availableBooksISBNLineEdit);

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

    availableBooksCurrentPageLineEdit = new QLineEdit(this);
    availableBooksCurrentPageLineEdit->setText("0");
    availableBooksCurrentPageLineEdit->setAlignment(Qt::AlignHCenter);
    connect(availableBooksCurrentPageLineEdit, &QLineEdit::textChanged, this, &MainWindow::availableBooksCurrentPageChanged);

    availableBooksNavigation->addWidget(previousAvailableBooksButton);
    availableBooksNavigation->addWidget(availableBooksCurrentPageLineEdit);
    availableBooksNavigation->addWidget(nextAvailableBooksButton);

    availableBooksNavigationBox->setLayout(availableBooksNavigation);

    availableBooksLayout->addWidget(availableBooksNavigationBox);



    availableBookBox->setLayout(availableBooksLayout);

    QGroupBox* borrowedBookBox = new QGroupBox(this);

    QGroupBox* borrowedBooksSearchBox = new QGroupBox(this);
    QVBoxLayout* borrowedBooksLayout = new QVBoxLayout();

    QFormLayout* borrowedBooksSearch = new QFormLayout();
    borrowedBooksNameLineEdit = new QLineEdit(this);
    borrowedBooksAuthorLineEdit = new QLineEdit(this);
    borrowedBooksISBNLineEdit = new QLineEdit(this);

    connect(borrowedBooksNameLineEdit,&QLineEdit::returnPressed,this,&MainWindow::borrowedBooksNameLineEditReturnPressed);
    connect(borrowedBooksAuthorLineEdit,&QLineEdit::returnPressed,this,&MainWindow::borrowedBooksAuthorLineEditReturnPressed);
    connect(borrowedBooksISBNLineEdit,&QLineEdit::returnPressed,this,&MainWindow::borrowedBooksISBNLineEditReturnPressed);

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

    borrowedBooksCurrentPageLineEdit = new QLineEdit(this);
    borrowedBooksCurrentPageLineEdit->setAlignment(Qt::AlignHCenter);
    borrowedBooksCurrentPageLineEdit->setText("0");
    connect(borrowedBooksCurrentPageLineEdit, &QLineEdit::textChanged, this, &MainWindow::borrowedBooksCurrentPageChanged);


    borrowedBooksNavigation->addWidget(previousBorrowedBooksButton);
    borrowedBooksNavigation->addWidget(borrowedBooksCurrentPageLineEdit);
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

void MainWindow::borrowBook(const QModelIndex &index)
{
    auto idNode = ((TreeItem *)index.internalPointer())->child(0);
    if(idNode!=nullptr)
    {
        auto string = idNode->data(0).toString();
        string.erase(string.begin(),string.begin()+4);

        dataBase.borrowBook(currentUser.getUserName(), string.toInt());
    }

    delete borrowedBooksModel;

    borrowedBooksModel = new TreeModel(dataBase.getBorrowedBooks(borrowedBooksCurrentPage,currentUser.getUserName()),true);
    borrowedBooksList->setModel(borrowedBooksModel);

    availableBooksModel = new TreeModel(dataBase.getAvailableBooks(availableBooksCurrentPage),false);
    availableBooksList->setModel(availableBooksModel);
}

void MainWindow::returnBook(const QModelIndex &index)
{
    auto idNode = ((TreeItem *)index.internalPointer())->child(0);
    if(idNode!=nullptr)
    {
        auto string = idNode->data(0).toString();
        string.erase(string.begin(),string.begin()+4);

        dataBase.returnBook(currentUser.getUserName(), string.toInt());
    }

    delete borrowedBooksModel;

    borrowedBooksModel = new TreeModel(dataBase.getBorrowedBooks(borrowedBooksCurrentPage,currentUser.getUserName()),true);
    borrowedBooksList->setModel(borrowedBooksModel);

    availableBooksModel = new TreeModel(dataBase.getAvailableBooks(availableBooksCurrentPage),false);
    availableBooksList->setModel(availableBooksModel);
}


void MainWindow::logOut()
{
    availableBooksCurrentPage=0;
    borrowedBooksCurrentPage=0;
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

void MainWindow::deleteBook()
{
    delete deleteBookDialog;
    deleteBookDialog = new DeleteBookDialog(this);
    connect(deleteBookDialog,&QInputDialog::finished,this,&MainWindow::deleteBookDialogFinished);
    deleteBookDialog->show();
}

void MainWindow::nextAvailableBooksButtonOnClick()
{
    if(availableBooksNameLineEdit->text()=="" && availableBooksAuthorLineEdit->text()=="" && availableBooksISBNLineEdit->text()=="")
    {
        availableBooksCurrentPage++;
        availableBooksCurrentPageLineEdit->setText(QString::number(availableBooksCurrentPage));
        delete availableBooksModel;

        availableBooksModel = new TreeModel(dataBase.getAvailableBooks(availableBooksCurrentPage),false);
        availableBooksList->setModel(availableBooksModel);
    }
    else
    {
        availableBooksCurrentPage++;
        availableBooksCurrentPageLineEdit->setText(QString::number(availableBooksCurrentPage));
        delete availableBooksModel;

        availableBooksModel = new TreeModel(dataBase.searchAvailableBooks(availableBooksNameLineEdit->text(),availableBooksAuthorLineEdit->text(), availableBooksISBNLineEdit->text(), availableBooksCurrentPage),false);
        availableBooksList->setModel(availableBooksModel);
    }
}

void MainWindow::previousAvailableBooksButtonOnClick()
{
    if(availableBooksCurrentPage==0)
        return;
    if(availableBooksNameLineEdit->text()=="" && availableBooksAuthorLineEdit->text()=="" && availableBooksISBNLineEdit->text()=="")
    {
        availableBooksCurrentPage--;

        availableBooksCurrentPageLineEdit->setText(QString::number(availableBooksCurrentPage));
        delete availableBooksModel;

        availableBooksModel = new TreeModel(dataBase.getAvailableBooks(availableBooksCurrentPage),false);
        availableBooksList->setModel(availableBooksModel);
    }
    else
    {
        availableBooksCurrentPage--;
        availableBooksCurrentPageLineEdit->setText(QString::number(availableBooksCurrentPage));
        delete availableBooksModel;

        availableBooksModel = new TreeModel(dataBase.searchAvailableBooks(availableBooksNameLineEdit->text(),availableBooksAuthorLineEdit->text(), availableBooksISBNLineEdit->text(), availableBooksCurrentPage),false);
        availableBooksList->setModel(availableBooksModel);
    }
}

void MainWindow::nextBorrowedBooksButtonOnClick()
{
    if(borrowedBooksNameLineEdit->text()=="" && borrowedBooksAuthorLineEdit->text()=="" && borrowedBooksISBNLineEdit->text()=="")
    {
        borrowedBooksCurrentPage++;
        borrowedBooksCurrentPageLineEdit->setText(QString::number(borrowedBooksCurrentPage));
        delete borrowedBooksModel;

        borrowedBooksModel = new TreeModel(dataBase.getBorrowedBooks(borrowedBooksCurrentPage,currentUser.getUserName()),true);
        borrowedBooksList->setModel(borrowedBooksModel);
    }
    else
    {
        borrowedBooksCurrentPage++;
        borrowedBooksCurrentPageLineEdit->setText(QString::number(borrowedBooksCurrentPage));
        delete borrowedBooksModel;

        borrowedBooksModel = new TreeModel(dataBase.searchBorrowedBooks(borrowedBooksNameLineEdit->text(), borrowedBooksAuthorLineEdit->text(), borrowedBooksISBNLineEdit->text(), borrowedBooksCurrentPage, currentUser.getUserName()), false);
        borrowedBooksList->setModel(borrowedBooksModel);
    }
}

void MainWindow::previousBorrowedBooksButtonOnClick()
{
    if(borrowedBooksCurrentPage == 0)
        return;
    if(borrowedBooksNameLineEdit->text()=="" && borrowedBooksAuthorLineEdit->text()=="" && borrowedBooksISBNLineEdit->text()=="")
    {
        borrowedBooksCurrentPage--;
        borrowedBooksCurrentPageLineEdit->setText(QString::number(borrowedBooksCurrentPage));
        delete borrowedBooksModel;

        borrowedBooksModel = new TreeModel(dataBase.getBorrowedBooks(borrowedBooksCurrentPage,currentUser.getUserName()),true);
        borrowedBooksList->setModel(borrowedBooksModel);
    }
    else
    {
        borrowedBooksCurrentPage--;
        borrowedBooksCurrentPageLineEdit->setText(QString::number(borrowedBooksCurrentPage));
        delete borrowedBooksModel;

        borrowedBooksModel = new TreeModel(dataBase.searchBorrowedBooks(borrowedBooksNameLineEdit->text(), borrowedBooksAuthorLineEdit->text(), borrowedBooksISBNLineEdit->text(), borrowedBooksCurrentPage, currentUser.getUserName()), false);
        borrowedBooksList->setModel(borrowedBooksModel);
    }
}

void MainWindow::availableBooksCurrentPageChanged(QString text)
{
    bool *ok = new bool(true);
    int currentPage = text.toInt(ok,10);

    if(*ok)
    {
        if(availableBooksNameLineEdit->text()=="" && availableBooksAuthorLineEdit->text()=="" && availableBooksISBNLineEdit->text()=="")
        {
            availableBooksCurrentPage = currentPage;
            delete availableBooksModel;

            availableBooksModel = new TreeModel(dataBase.getAvailableBooks(availableBooksCurrentPage),false);
            availableBooksList->setModel(availableBooksModel);
        }
        else
        {
            for(int index=availableBooksLastPage+1;index<=currentPage;++index)
            {
                dataBase.searchAvailableBooks(availableBooksNameLineEdit->text(),availableBooksAuthorLineEdit->text(), availableBooksISBNLineEdit->text(), index);
            }
            delete availableBooksModel;
            availableBooksCurrentPage=currentPage;
            if(availableBooksLastPage<currentPage)
            {
                availableBooksLastPage=currentPage;
            }
            availableBooksModel = new TreeModel(dataBase.searchAvailableBooks(availableBooksNameLineEdit->text(),availableBooksAuthorLineEdit->text(), availableBooksISBNLineEdit->text(), availableBooksCurrentPage),false);
            availableBooksList->setModel(availableBooksModel);
        }
    }
}

void MainWindow::borrowedBooksCurrentPageChanged(QString text)
{
    bool *ok = new bool(true);
    int currentPage = text.toInt(ok,10);
    if(*ok)
    {
        if(borrowedBooksNameLineEdit->text()=="" && borrowedBooksAuthorLineEdit->text()=="" && borrowedBooksISBNLineEdit->text()=="")
        {
            borrowedBooksCurrentPage=currentPage;
            delete borrowedBooksModel;

            borrowedBooksModel = new TreeModel(dataBase.getBorrowedBooks(borrowedBooksCurrentPage,currentUser.getUserName()),true);
            borrowedBooksList->setModel(borrowedBooksModel);
        }
        else
        {
            for(int index=borrowedBooksLastPage;index<=currentPage;++index)
            {
                dataBase.searchBorrowedBooks(borrowedBooksNameLineEdit->text(), borrowedBooksAuthorLineEdit->text(), borrowedBooksISBNLineEdit->text(), index, currentUser.getUserName());
            }
            delete borrowedBooksModel;
            borrowedBooksCurrentPage = currentPage;
            if(borrowedBooksLastPage< borrowedBooksCurrentPage)
            {
                borrowedBooksLastPage = borrowedBooksCurrentPage;
            }

            borrowedBooksModel = new TreeModel(dataBase.searchBorrowedBooks(borrowedBooksNameLineEdit->text(), borrowedBooksAuthorLineEdit->text(), borrowedBooksISBNLineEdit->text(), borrowedBooksCurrentPage, currentUser.getUserName()), false);
            borrowedBooksList->setModel(borrowedBooksModel);
        }
    }
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

void MainWindow::availableBooksNameLineEditReturnPressed()
{
    availableBooksCurrentPage = 0;
    availableBooksLastPage=0;
    availableBooksCurrentPageLineEdit->setText("0");
    delete availableBooksModel;

    availableBooksModel = new TreeModel(dataBase.searchAvailableBooks(availableBooksNameLineEdit->text(),availableBooksAuthorLineEdit->text(), availableBooksISBNLineEdit->text(), availableBooksCurrentPage),false);
    availableBooksList->setModel(availableBooksModel);
}

void MainWindow::availableBooksISBNLineEditReturnPressed()
{
    availableBooksCurrentPage = 0;
    availableBooksLastPage=0;
    availableBooksCurrentPageLineEdit->setText("0");
    delete availableBooksModel;

    availableBooksModel = new TreeModel(dataBase.searchAvailableBooks(availableBooksNameLineEdit->text(), availableBooksAuthorLineEdit->text(), availableBooksISBNLineEdit->text(), availableBooksCurrentPage),false);
    availableBooksList->setModel(availableBooksModel);
}

void MainWindow::availableBooksAuthorLineEditReturnPressed()
{
    availableBooksCurrentPage = 0;
    availableBooksLastPage=0;
    availableBooksCurrentPageLineEdit->setText("0");
    delete availableBooksModel;

    availableBooksModel = new TreeModel(dataBase.searchAvailableBooks(availableBooksNameLineEdit->text(), availableBooksAuthorLineEdit->text(), availableBooksISBNLineEdit->text(), availableBooksCurrentPage),false);
    availableBooksList->setModel(availableBooksModel);
}

void MainWindow::borrowedBooksNameLineEditReturnPressed()
{
    borrowedBooksCurrentPage = 0;
    borrowedBooksLastPage=0;
    borrowedBooksCurrentPageLineEdit->setText("0");
    delete borrowedBooksModel;

    borrowedBooksModel = new TreeModel(dataBase.searchBorrowedBooks(borrowedBooksNameLineEdit->text(), borrowedBooksAuthorLineEdit->text(), borrowedBooksISBNLineEdit->text(), borrowedBooksCurrentPage, currentUser.getUserName()), false);
    borrowedBooksList->setModel(borrowedBooksModel);
}

void MainWindow::borrowedBooksAuthorLineEditReturnPressed()
{
    borrowedBooksCurrentPage = 0;
    borrowedBooksLastPage=0;
    borrowedBooksCurrentPageLineEdit->setText("0");
    delete borrowedBooksModel;

    borrowedBooksModel = new TreeModel(dataBase.searchBorrowedBooks(borrowedBooksNameLineEdit->text(), borrowedBooksAuthorLineEdit->text(), borrowedBooksISBNLineEdit->text(), borrowedBooksCurrentPage, currentUser.getUserName()), false);
    borrowedBooksList->setModel(borrowedBooksModel);
}

void MainWindow::borrowedBooksISBNLineEditReturnPressed()
{
    borrowedBooksCurrentPage = 0;
    borrowedBooksLastPage=0;
    borrowedBooksCurrentPageLineEdit->setText("0");
    delete borrowedBooksModel;

    borrowedBooksModel = new TreeModel(dataBase.searchBorrowedBooks(borrowedBooksNameLineEdit->text(), borrowedBooksAuthorLineEdit->text(), borrowedBooksISBNLineEdit->text(), borrowedBooksCurrentPage, currentUser.getUserName()), false);
    borrowedBooksList->setModel(borrowedBooksModel);
}
