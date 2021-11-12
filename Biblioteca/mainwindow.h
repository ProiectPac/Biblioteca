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
    MainWindow(User* user,std::shared_ptr<DataBase> database,QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
<<<<<<< HEAD
    User* user;
    std::shared_ptr<DataBase> dataBase;
=======
    User user;
    DataBase dataBase;
<<<<<<< Updated upstream
=======
>>>>>>> 7ad67e59a637ca93fc9d7a94b95fd98e79bb3f55
>>>>>>> Stashed changes
    void addBorrowBook(Book book);
    void deleteBorrowBook(Book book);
    void logOut();
};
#endif // MAINWINDOW_H
