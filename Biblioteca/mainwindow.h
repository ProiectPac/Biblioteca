#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
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

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    ~MainWindow();



    MainWindow();

private:
    QMenuBar* userBar = nullptr;
    QMenu* userMenu = nullptr;
    QAction* logOutAction = nullptr;
    QAction* deleteUserAction = nullptr;
    QTextBrowser *bookContent = nullptr;
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

    bool hasError=false;

    User currentUser;

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

    int availableBooksCurrentPage=0;
    int borrowedBooksCurrentPage=0;
    int availableBooksLastPage=0;
    int borrowedBooksLastPage=0;
};
#endif // MAINWINDOW_H
