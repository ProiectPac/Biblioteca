#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "user.h"
#include "database.h"
#include "logindialog.h"
#include <QErrorMessage>
#include <memory>

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

    const std::shared_ptr<DataBase> &getDataBase() const;

private:
    void setUpUserBar();
    Ui::MainWindow *ui;
    User* user;
    std::shared_ptr<DataBase> dataBase;
    void addBorrowBook(Book book);
    void deleteBorrowBook(Book book);
    void logOut();
    void deleteCurrentUser();
};
#endif // MAINWINDOW_H
