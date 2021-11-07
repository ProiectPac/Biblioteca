#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "user.h"
#include "database.h"
#include "logindialog.h"
#include <QErrorMessage>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(User& user,DataBase& database,QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    User user;
    DataBase database;
    void addBorrowBook(Book book);
    void deleteBorrowBook(Book book);
    void logOut();
};
#endif // MAINWINDOW_H
