#include "mainwindow.h"
#include "ui_mainwindow.h"

void MainWindow::setUpUserBar()
{
    TCPSocket::startUp();
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
        if(loginDialog->getAction()==LoginDialog::Actions::Login)
        {
            connectionSocket.Send("login "+userCredentials.first.toStdString()+" "+QString::number(userCredentials.second).toStdString());
            std::string response = connectionSocket.Receive();
            if(response=="Logged in succesfully")
            {
                auto foundUser = User(userCredentials.first, 0);
                setUser(foundUser);

                connectionSocket.Send("getAvailableBooks " + QString::number(availableBooksCurrentPage).toStdString());
                availableBooksModel = new TreeModel(stringToAvailableBooksVector(connectionSocket.Receive()),false);
                connectionSocket.Send("getBorrowedBooks " + QString::number(borrowedBooksCurrentPage).toStdString());
                borrowedBooksModel = new TreeModel(stringToBorrowedBooksVector(connectionSocket.Receive()),true);

                availableBooksList->setModel(availableBooksModel);
                borrowedBooksList->setModel(borrowedBooksModel);
            }
            else
            {
                QString message= QString::fromStdString(response);
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
            connectionSocket.Send("registerAcount "+userCredentials.first.toStdString()+" "+QString::number(userCredentials.second).toStdString());
            std::string response = connectionSocket.Receive();
            if(response=="Registration went succesfuly")
            {
                auto foundUser = User(userCredentials.first, 0);
                setUser(foundUser);

                connectionSocket.Send("getAvailableBooks " + QString::number(availableBooksCurrentPage).toStdString());
                availableBooksModel = new TreeModel(stringToAvailableBooksVector(connectionSocket.Receive()),false);
                connectionSocket.Send("getBorrowedBooks " + QString::number(borrowedBooksCurrentPage).toStdString());
                borrowedBooksModel = new TreeModel(stringToBorrowedBooksVector(connectionSocket.Receive()),true);

                availableBooksList->setModel(availableBooksModel);
                borrowedBooksList->setModel(borrowedBooksModel);
            }
            else
            {
                QString message= QString::fromStdString(response);
                QErrorMessage *errorMessage = QErrorMessage::qtHandler();

                delete loginDialog;
                loginDialog = new LoginDialog(this);
                connect(loginDialog,&LoginDialog::finished,this,&MainWindow::loginDialogFinished);
                connect(errorMessage,&QErrorMessage::finished, loginDialog,&LoginDialog::open);

                errorMessage->showMessage(message);
                hasError=true;
            }

        }

    }
    else
        this->close();
}

void MainWindow::deleteBookDialogFinished()
{
    int bookId = deleteBookDialog->getBookID();
    connectionSocket.Send("removeBook"+QString::number(bookId).toStdString());
    auto response = connectionSocket.Receive();
    QMessageBox messageBox;
    messageBox.setText(QString::fromStdString(response));
    messageBox.setWindowTitle("Delete book Response");
    messageBox.open();

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
    TCPSocket::startUp();
    connectionSocket.ConnectToServer("localhost", 27015);

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
        connectionSocket.Send("borrowBook " + string.toStdString());
        std::string response = connectionSocket.Receive();
        if(response!="Book borrowed succesfully!")
        {
            QString message= QString::fromStdString(response);
            QErrorMessage *errorMessage = QErrorMessage::qtHandler();

            errorMessage->showMessage(message);
        }
    }

    if(availableBooksNameLineEdit->text()=="" && availableBooksAuthorLineEdit->text()=="" && availableBooksISBNLineEdit->text()=="")
    {
        delete availableBooksModel;

        connectionSocket.Send("getAvailableBooks " + QString::number(availableBooksCurrentPage).toStdString());
        availableBooksModel = new TreeModel(stringToAvailableBooksVector(connectionSocket.Receive()),false);
        availableBooksList->setModel(availableBooksModel);
    }
    else
    {
        connectionSocket.Send("searchAvailableBooks ");
        availableBooksModel = new TreeModel(stringToAvailableBooksVector(connectionSocket.Receive()),false);


        delete availableBooksModel;
        connectionSocket.Send("searchAvailableBooks "+ availableBooksNameLineEdit->text().toStdString()+ availableBooksAuthorLineEdit->text().toStdString()+ availableBooksISBNLineEdit->text().toStdString()+ QString::number(availableBooksCurrentPage).toStdString());
        availableBooksModel = new TreeModel(stringToAvailableBooksVector(connectionSocket.Receive()),false);
        availableBooksList->setModel(availableBooksModel);
    }

    if(borrowedBooksNameLineEdit->text()=="" && borrowedBooksAuthorLineEdit->text()=="" && borrowedBooksISBNLineEdit->text()=="")
    {
        delete borrowedBooksModel;

        connectionSocket.Send("getBorrowedBooks " + QString::number(borrowedBooksCurrentPage).toStdString());
        borrowedBooksModel = new TreeModel(stringToBorrowedBooksVector(connectionSocket.Receive()),true);
        borrowedBooksList->setModel(borrowedBooksModel);
    }
    else
    {
        connectionSocket.Send("searchBorrowedBooks ");
        borrowedBooksModel = new TreeModel(stringToBorrowedBooksVector(connectionSocket.Receive()), true);
        delete borrowedBooksModel;
        connectionSocket.Send("searchBorrowedBooks "+ borrowedBooksNameLineEdit->text().toStdString()+ borrowedBooksAuthorLineEdit->text().toStdString()+ borrowedBooksISBNLineEdit->text().toStdString()+ QString::number(borrowedBooksCurrentPage).toStdString());
        borrowedBooksModel = new TreeModel(stringToBorrowedBooksVector(connectionSocket.Receive()), true);
        borrowedBooksList->setModel(borrowedBooksModel);
    }
}

void MainWindow::returnBook(const QModelIndex &index)
{
    auto idNode = ((TreeItem *)index.internalPointer())->child(0);
    if(idNode!=nullptr)
    {
        auto string = idNode->data(0).toString();
        string.erase(string.begin(),string.begin()+4);
        connectionSocket.Send("returnBook " + string.toStdString());
        std::string response = connectionSocket.Receive();
        if(response!="Book returned succesfully!")
        {
            QString message= QString::fromStdString(response);
            QErrorMessage *errorMessage = QErrorMessage::qtHandler();

            errorMessage->showMessage(message);
        }
    }


    if(availableBooksNameLineEdit->text()=="" && availableBooksAuthorLineEdit->text()=="" && availableBooksISBNLineEdit->text()=="")
    {
        delete availableBooksModel;

        connectionSocket.Send("getAvailableBooks " + QString::number(availableBooksCurrentPage).toStdString());
        availableBooksModel = new TreeModel(stringToAvailableBooksVector(connectionSocket.Receive()),false);

        availableBooksList->setModel(availableBooksModel);
    }
    else
    {
        connectionSocket.Send("searchAvailableBooks ");
        connectionSocket.Receive();

        delete availableBooksModel;
        connectionSocket.Send("searchAvailableBooks "+ availableBooksNameLineEdit->text().toStdString()+ availableBooksAuthorLineEdit->text().toStdString()+ availableBooksISBNLineEdit->text().toStdString()+ QString::number(availableBooksCurrentPage).toStdString());
        availableBooksModel = new TreeModel(stringToAvailableBooksVector(connectionSocket.Receive()),false);
        availableBooksList->setModel(availableBooksModel);
    }

    if(borrowedBooksNameLineEdit->text()=="" && borrowedBooksAuthorLineEdit->text()=="" && borrowedBooksISBNLineEdit->text()=="")
    {
        delete borrowedBooksModel;

        connectionSocket.Send("getBorrowedBooks " + QString::number(borrowedBooksCurrentPage).toStdString());
        borrowedBooksModel = new TreeModel(stringToBorrowedBooksVector(connectionSocket.Receive()),true);
        borrowedBooksList->setModel(borrowedBooksModel);
    }
    else
    {
        connectionSocket.Send("searchBorrowedBooks ");
        borrowedBooksModel = new TreeModel(stringToBorrowedBooksVector(connectionSocket.Receive()), true);
        delete borrowedBooksModel;
        connectionSocket.Send("searchBorrowedBooks "+ borrowedBooksNameLineEdit->text().toStdString()+ borrowedBooksAuthorLineEdit->text().toStdString()+ borrowedBooksISBNLineEdit->text().toStdString()+ QString::number(borrowedBooksCurrentPage).toStdString());
        borrowedBooksModel = new TreeModel(stringToBorrowedBooksVector(connectionSocket.Receive()), true);
        borrowedBooksList->setModel(borrowedBooksModel);
    }
}


void MainWindow::logOut()
{
    availableBooksCurrentPage=0;
    borrowedBooksCurrentPage=0;
    connectionSocket.Send("logOut");
    connectionSocket.Receive();
    delete loginDialog;
    loginDialog = new LoginDialog(this);
    connect(loginDialog,&LoginDialog::finished,this,&MainWindow::loginDialogFinished);
    loginDialog->open();
}

void MainWindow::deleteCurrentUser()
{
    connectionSocket.Send("deleteAccount");
    connectionSocket.Receive();
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
            connectionSocket.Send("updateUserPassword" + currentUser.getPasswordHash());
            connectionSocket.Receive();
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

        connectionSocket.Send("getAvailableBooks " + QString::number(availableBooksCurrentPage).toStdString());
        availableBooksModel = new TreeModel(stringToAvailableBooksVector(connectionSocket.Receive()),false);

        availableBooksList->setModel(availableBooksModel);
    }
    else
    {
        availableBooksCurrentPage++;
        availableBooksCurrentPageLineEdit->setText(QString::number(availableBooksCurrentPage));
        delete availableBooksModel;

        connectionSocket.Send("searchAvailableBooks "+ availableBooksNameLineEdit->text().toStdString()+ availableBooksAuthorLineEdit->text().toStdString()+ availableBooksISBNLineEdit->text().toStdString()+ QString::number(availableBooksCurrentPage).toStdString());
        availableBooksModel = new TreeModel(stringToAvailableBooksVector(connectionSocket.Receive()),false);
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

        connectionSocket.Send("getAvailableBooks " + QString::number(availableBooksCurrentPage).toStdString());
        availableBooksModel = new TreeModel(stringToAvailableBooksVector(connectionSocket.Receive()),false);
        availableBooksList->setModel(availableBooksModel);
    }
    else
    {
        availableBooksCurrentPage--;
        availableBooksCurrentPageLineEdit->setText(QString::number(availableBooksCurrentPage));
        delete availableBooksModel;

        connectionSocket.Send("searchAvailableBooks "+ availableBooksNameLineEdit->text().toStdString()+ availableBooksAuthorLineEdit->text().toStdString()+ availableBooksISBNLineEdit->text().toStdString()+ QString::number(availableBooksCurrentPage).toStdString());
        availableBooksModel = new TreeModel(stringToAvailableBooksVector(connectionSocket.Receive()),false);
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

        connectionSocket.Send("getBorrowedBooks " + QString::number(borrowedBooksCurrentPage).toStdString());
        borrowedBooksModel = new TreeModel(stringToBorrowedBooksVector(connectionSocket.Receive()),true);
        borrowedBooksList->setModel(borrowedBooksModel);
    }
    else
    {
        borrowedBooksCurrentPage++;
        borrowedBooksCurrentPageLineEdit->setText(QString::number(borrowedBooksCurrentPage));
        delete borrowedBooksModel;

        connectionSocket.Send("searchBorrowedBooks "+ borrowedBooksNameLineEdit->text().toStdString()+ borrowedBooksAuthorLineEdit->text().toStdString()+ borrowedBooksISBNLineEdit->text().toStdString()+ QString::number(borrowedBooksCurrentPage).toStdString());
        borrowedBooksModel = new TreeModel(stringToBorrowedBooksVector(connectionSocket.Receive()), true);
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

        connectionSocket.Send("getBorrowedBooks " + QString::number(borrowedBooksCurrentPage).toStdString());
        borrowedBooksModel = new TreeModel(stringToBorrowedBooksVector(connectionSocket.Receive()),true);
        borrowedBooksList->setModel(borrowedBooksModel);
    }
    else
    {
        borrowedBooksCurrentPage--;
        borrowedBooksCurrentPageLineEdit->setText(QString::number(borrowedBooksCurrentPage));
        delete borrowedBooksModel;

        connectionSocket.Send("searchBorrowedBooks "+ borrowedBooksNameLineEdit->text().toStdString()+ borrowedBooksAuthorLineEdit->text().toStdString()+ borrowedBooksISBNLineEdit->text().toStdString()+ QString::number(borrowedBooksCurrentPage).toStdString());
        borrowedBooksModel = new TreeModel(stringToBorrowedBooksVector(connectionSocket.Receive()), true);
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

            connectionSocket.Send("getAvailableBooks " + QString::number(availableBooksCurrentPage).toStdString());
            availableBooksModel = new TreeModel(stringToAvailableBooksVector(connectionSocket.Receive()),false);

            availableBooksList->setModel(availableBooksModel);
        }
        else
        {
            connectionSocket.Send("searchAvailableBooks ");
            connectionSocket.Receive();
            availableBooksModel = new TreeModel(stringToAvailableBooksVector(connectionSocket.Receive()),false);
            delete availableBooksModel;
            availableBooksCurrentPage=currentPage;
            if(availableBooksLastPage<currentPage)
            {
                availableBooksLastPage=currentPage;
            }
            connectionSocket.Send("searchAvailableBooks "+ availableBooksNameLineEdit->text().toStdString()+ availableBooksAuthorLineEdit->text().toStdString()+ availableBooksISBNLineEdit->text().toStdString()+ QString::number(availableBooksCurrentPage).toStdString());
            availableBooksModel = new TreeModel(stringToAvailableBooksVector(connectionSocket.Receive()),false);
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

            connectionSocket.Send("getBorrowedBooks " + QString::number(borrowedBooksCurrentPage).toStdString());
            borrowedBooksModel = new TreeModel(stringToBorrowedBooksVector(connectionSocket.Receive()),true);
            borrowedBooksList->setModel(borrowedBooksModel);
        }
        else
        {

            delete borrowedBooksModel;
            borrowedBooksCurrentPage = currentPage;
            if(borrowedBooksLastPage< borrowedBooksCurrentPage)
            {
                borrowedBooksLastPage = borrowedBooksCurrentPage;
            }

            connectionSocket.Send("searchBorrowedBooks "+ borrowedBooksNameLineEdit->text().toStdString()+ borrowedBooksAuthorLineEdit->text().toStdString()+ borrowedBooksISBNLineEdit->text().toStdString()+ QString::number(borrowedBooksCurrentPage).toStdString());
            borrowedBooksModel = new TreeModel(stringToBorrowedBooksVector(connectionSocket.Receive()), true);
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
        std::string bookString = newBook.getISBN().toStdString() + " " + newBook.getAuthor().toStdString() + " " + QString::number(newBook.getOriginalPublicationYear()).toStdString() + " " + newBook.getTitle().toStdString() + " " + newBook.getLanguage().toStdString() + " " + QString::number(newBook.getAverageRating()).toStdString() + " " + newBook.getImageURL().toStdString() + " " + newBook.getSmallImageURL().toStdString() + " " + QString::number(newBook.getBooksCount()).toStdString();
        connectionSocket.Send(bookString);
        std::string response = connectionSocket.Receive();
        if(response!="Book has been added succesfully!")
        {
            QString message= QString::fromStdString(response);
            QErrorMessage *errorMessage = QErrorMessage::qtHandler();

            errorMessage->showMessage(message);
        }
    }

}

void MainWindow::availableBooksNameLineEditReturnPressed()
{
    availableBooksCurrentPage = 0;
    availableBooksLastPage=0;
    availableBooksCurrentPageLineEdit->setText("0");
    delete availableBooksModel;

    connectionSocket.Send("searchAvailableBooks "+ availableBooksNameLineEdit->text().toStdString()+ availableBooksAuthorLineEdit->text().toStdString()+ availableBooksISBNLineEdit->text().toStdString()+ QString::number(availableBooksCurrentPage).toStdString());
    availableBooksModel = new TreeModel(stringToAvailableBooksVector(connectionSocket.Receive()),false);
    availableBooksList->setModel(availableBooksModel);
}

void MainWindow::availableBooksISBNLineEditReturnPressed()
{
    availableBooksCurrentPage = 0;
    availableBooksLastPage=0;
    availableBooksCurrentPageLineEdit->setText("0");
    delete availableBooksModel;

    connectionSocket.Send("searchAvailableBooks "+ availableBooksNameLineEdit->text().toStdString()+ availableBooksAuthorLineEdit->text().toStdString()+ availableBooksISBNLineEdit->text().toStdString()+ QString::number(availableBooksCurrentPage).toStdString());
    availableBooksModel = new TreeModel(stringToAvailableBooksVector(connectionSocket.Receive()),false);
    availableBooksList->setModel(availableBooksModel);
}

void MainWindow::availableBooksAuthorLineEditReturnPressed()
{
    availableBooksCurrentPage = 0;
    availableBooksLastPage=0;
    availableBooksCurrentPageLineEdit->setText("0");
    delete availableBooksModel;

    connectionSocket.Send("searchAvailableBooks "+ availableBooksNameLineEdit->text().toStdString()+ availableBooksAuthorLineEdit->text().toStdString()+ availableBooksISBNLineEdit->text().toStdString()+ QString::number(availableBooksCurrentPage).toStdString());
    availableBooksModel = new TreeModel(stringToAvailableBooksVector(connectionSocket.Receive()),false);
    availableBooksList->setModel(availableBooksModel);
}

void MainWindow::borrowedBooksNameLineEditReturnPressed()
{
    borrowedBooksCurrentPage = 0;
    borrowedBooksLastPage=0;
    borrowedBooksCurrentPageLineEdit->setText("0");
    delete borrowedBooksModel;

    connectionSocket.Send("searchBorrowedBooks "+ borrowedBooksNameLineEdit->text().toStdString()+ borrowedBooksAuthorLineEdit->text().toStdString()+ borrowedBooksISBNLineEdit->text().toStdString()+ QString::number(borrowedBooksCurrentPage).toStdString());
    borrowedBooksModel = new TreeModel(stringToBorrowedBooksVector(connectionSocket.Receive()), true);
    borrowedBooksList->setModel(borrowedBooksModel);
}

void MainWindow::borrowedBooksAuthorLineEditReturnPressed()
{
    borrowedBooksCurrentPage = 0;
    borrowedBooksLastPage=0;
    borrowedBooksCurrentPageLineEdit->setText("0");
    delete borrowedBooksModel;

    connectionSocket.Send("searchBorrowedBooks "+ borrowedBooksNameLineEdit->text().toStdString()+ borrowedBooksAuthorLineEdit->text().toStdString()+ borrowedBooksISBNLineEdit->text().toStdString()+ QString::number(borrowedBooksCurrentPage).toStdString());
    borrowedBooksModel = new TreeModel(stringToBorrowedBooksVector(connectionSocket.Receive()), true);
    borrowedBooksList->setModel(borrowedBooksModel);
}

std::vector<std::string> MainWindow::separate(const std::string &message, char separator)
{
    std::vector<std::string> result;

    int lastPosition = 0;
    int position = message.find(separator);

    while(position !=std::string::npos)
    {
        result.push_back(message.substr(lastPosition,position - lastPosition));
        lastPosition = position + 1;
        position = message.find(separator,lastPosition);
    }

    result.push_back(message.substr(lastPosition,message.size() - lastPosition));

    return result;
}

std::vector<Book> MainWindow::stringToAvailableBooksVector(std::string string)
{
    std::vector<std::string> lines = separate(string, '\n');
    std::vector<Book> bookVector;
    for(int index =1;index<lines.size();index++)
    {
        std::vector<std::string> words = separate(lines[index], ' ');
        Book newBook(QString::fromStdString(words[0]).toInt(),QString::fromStdString(words[1]),QString::fromStdString(words[2]),QString::fromStdString(words[3]).toInt(),QString::fromStdString(words[4]),QString::fromStdString(words[5]),QString::fromStdString(words[6]).toFloat(),QString::fromStdString(words[7]),QString::fromStdString(words[8]),QString::fromStdString(words[9]).toInt());
        bookVector.push_back(newBook);
    }
    return bookVector;
}

std::vector<Book> MainWindow::stringToBorrowedBooksVector(std::string string)
{
    std::vector<std::string> lines = separate(string, '\n');
    std::vector<Book> bookVector;
    for(int index =1;index<lines.size();index++)
    {
        std::vector<std::string> words = separate(lines[index], ' ');
        Book newBook(QString::fromStdString(words[0]).toInt(),QString::fromStdString(words[1]),QString::fromStdString(words[2]),QString::fromStdString(words[3]).toInt(),QString::fromStdString(words[4]),QString::fromStdString(words[5]),QString::fromStdString(words[6]).toFloat(),QString::fromStdString(words[7]),QString::fromStdString(words[8]),QString::fromStdString(words[9]).toInt(),QString::fromStdString(words[10]).toInt());
        bookVector.push_back(newBook);
    }
    return bookVector;
}

void MainWindow::borrowedBooksISBNLineEditReturnPressed()
{
    borrowedBooksCurrentPage = 0;
    borrowedBooksLastPage=0;
    borrowedBooksCurrentPageLineEdit->setText("0");
    delete borrowedBooksModel;

    connectionSocket.Send("searchBorrowedBooks "+ borrowedBooksNameLineEdit->text().toStdString()+ borrowedBooksAuthorLineEdit->text().toStdString()+ borrowedBooksISBNLineEdit->text().toStdString()+ QString::number(borrowedBooksCurrentPage).toStdString());
    borrowedBooksModel = new TreeModel(stringToBorrowedBooksVector(connectionSocket.Receive()), true);
    borrowedBooksList->setModel(borrowedBooksModel);
}
