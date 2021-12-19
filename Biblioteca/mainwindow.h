#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "user.h"
#include "logindialog.h"
#include <QErrorMessage>
#include <memory>
#include "sqldatabase.h"
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

    QLineEdit* availableBooksCurrentPageLineEdit = nullptr;

    QLineEdit* availableBooksNameLineEdit = nullptr;
    QLineEdit* availableBooksAuthorLineEdit = nullptr;
    QLineEdit* availableBooksISBNLineEdit = nullptr;

    QLineEdit* borrowedBooksNameLineEdit = nullptr;
    QLineEdit* borrowedBooksAuthorLineEdit = nullptr;
    QLineEdit* borrowedBooksISBNLineEdit = nullptr;

    QLineEdit* borrowedCurrentPageLineEdit = nullptr;

    bool hasError=false;

    User currentUser;
    SQLDataBase dataBase;

    void setUser(User newUser);
    void setUpUserBar();
    void loginDialogFinished();
    void setUpUI();
    void borrowBook(const QModelIndex &index);
    void returnBook(const QModelIndex &index);
    void logOut();
    void deleteCurrentUser();
    void changeCurrentUserPassword();
    void nextAvailableBooksButtonOnClick();
    void previousAvailableBooksButtonOnClick();
    void nextBorrowedBooksButtonOnClick();
    void previousBorrowedBooksButtonOnClick();

    void availableBooksCurrentPageChanged(QString text);
    void borrowedBooksCurrentPageChanged(QString text);

    void addNewBook();
    void addBookFinished();
    void availableBooksNameLineEditTextChanged(QString text);
    void availableBooksISBNLineEditTextChanged(QString text);
    void availableBooksAuthorLineEditTextChanged(QString text);

    void borrowedBooksNameLineEditTextChanged(QString text);
    void borrowedBooksISBNLineEditTextChanged(QString text);
    void borrowedBooksAuthorLineEditTextChanged(QString text);

    int availableBooksCurrentPage=0;
    int borrowedBooksCurrentPage=0;
    int lastPage=0;
};
#endif // MAINWINDOW_H
