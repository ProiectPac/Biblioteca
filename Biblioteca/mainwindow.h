#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include "user.h"
#include "logindialog.h"
#include <QErrorMessage>
#include <memory>
#include <QInputDialog>
#include <QTextBrowser>
#include "borderlayout.h"
#include <QTreeView>
#include <QGroupBox>
#include <QVBoxLayout>
#include <QFormLayout>
#include <QHBoxLayout>
#include "treemodel.h"
#include "newbookdialog.h"
#include "treeitem.h"
#include "deletebookdialog.h"
#include "tcpsocket.h"
#include <QtNetwork>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    ~MainWindow();



    MainWindow();
public slots:
    void slot_netwManagerFinished1(QNetworkReply *reply);
    void slot_netwManagerFinished2(QNetworkReply *reply);

private:
    QNetworkAccessManager *m_netwManager1 = nullptr;
    QNetworkAccessManager *m_netwManager2 = nullptr;
    QMenuBar* userBar = nullptr;
    QMenu* userMenu = nullptr;
    QAction* logOutAction = nullptr;
    QAction* deleteUserAction = nullptr;
    QLabel *availableBookContent = nullptr;
    QLabel *borrowedBookContent =nullptr;
    BorderLayout *layout = nullptr;
    QTreeView* availableBooksList=nullptr;
    QTreeView* borrowedBooksList=nullptr;
    TreeModel* availableBooksModel=nullptr;
    TreeModel* borrowedBooksModel=nullptr;

    LoginDialog* loginDialog = nullptr;
    NewBookDialog *newBookDialog = nullptr;
    DeleteBookDialog *deleteBookDialog = nullptr;

    QLineEdit* availableBooksCurrentPageLineEdit = nullptr;

    QLineEdit* availableBooksNameLineEdit = nullptr;
    QLineEdit* availableBooksAuthorLineEdit = nullptr;
    QLineEdit* availableBooksISBNLineEdit = nullptr;

    QLineEdit* borrowedBooksNameLineEdit = nullptr;
    QLineEdit* borrowedBooksAuthorLineEdit = nullptr;
    QLineEdit* borrowedBooksISBNLineEdit = nullptr;

    QLineEdit* borrowedBooksCurrentPageLineEdit = nullptr;

    TCPSocket* connectionSocket = nullptr;

    bool hasError=false;

    User currentUser;


    void displayAvailableBook(const QModelIndex &index);
    void displayBorrowedBook(const QModelIndex &index);
    void setUser(User newUser);
    void setUpUserBar();
    void loginDialogFinished();
    void deleteBookDialogFinished();
    void setUpUI();
    void borrowBook(const QModelIndex &index);
    void returnBook(const QModelIndex &index);
    void logOut();
    void deleteCurrentUser();
    void changeCurrentUserPassword();
    void deleteBook();

    void nextAvailableBooksButtonOnClick();
    void previousAvailableBooksButtonOnClick();

    void nextBorrowedBooksButtonOnClick();
    void previousBorrowedBooksButtonOnClick();

    void availableBooksCurrentPageChanged(QString text);
    void borrowedBooksCurrentPageChanged(QString text);

    void addNewBook();
    void addBookFinished();

    void availableBooksNameLineEditReturnPressed();
    void availableBooksISBNLineEditReturnPressed();
    void availableBooksAuthorLineEditReturnPressed();

    void borrowedBooksNameLineEditReturnPressed();
    void borrowedBooksISBNLineEditReturnPressed();
    void borrowedBooksAuthorLineEditReturnPressed();

    std::vector<std::string> separate(std::string const& message, char separator);
    std::vector<Book> stringToAvailableBooksVector(std::string string);
    std::vector<Book> stringToBorrowedBooksVector(std::string string);

    void eliminateUnderline(std::string& message);
    std::string addUnderline(std::string message);

    int availableBooksCurrentPage=0;
    int borrowedBooksCurrentPage=0;
    int availableBooksLastPage=0;
    int borrowedBooksLastPage=0;
};
#endif // MAINWINDOW_H
