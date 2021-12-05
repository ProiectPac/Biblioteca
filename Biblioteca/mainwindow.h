#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "user.h"
#include "database.h"
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


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    ~MainWindow();


    void setUser(User *newUser);

    void setDataBase(const std::shared_ptr<DataBase> &newDataBase);
    MainWindow();

    const std::shared_ptr<DataBase> getDataBase() const;

private:
    QMenuBar* userBar = nullptr;
    QMenu* userMenu = nullptr;
    QAction* logOutAction = nullptr;
    QAction* deleteUserAction = nullptr;
    QTextBrowser *bookContent = nullptr;
    BorderLayout *layout = nullptr;
    LoginDialog* loginDialog = nullptr;
    Ui::MainWindow *ui = nullptr;
    User* user = nullptr;
    QTreeView* availableBooksList=nullptr;
    QTreeView* borrowedBooksList=nullptr;
    TreeModel* availableBooksModel=nullptr;
    TreeModel* borrowedBooksModel=nullptr;

    bool hasError=false;

    SQLDataBase sqlDataBase;
    std::shared_ptr<DataBase> dataBase;

    void setUpUserBar();
    void loginDialogFinished();
    void setUpUI();
    void addBorrowBook(Book& book);
    void deleteBorrowBook(Book& book);
    void logOut();
    void deleteCurrentUser();
    void changeCurrentUserPassword();
    void nextAvailableBooksButtonOnClick();
    void previousAvailableBooksButtonOnClick();
    void nextBorrowedBooksButtonOnClick();
    void previousBorrowedBooksButtonOnClick();

    int currentAvailableBooksPage=0;
    int currentBorrowedBooksPage=0;
};
#endif // MAINWINDOW_H
