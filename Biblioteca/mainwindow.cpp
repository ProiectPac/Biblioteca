#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(User& user, DataBase& database,QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->user=user;
    this->database=database;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::addBorrowBook(Book book)
{
    database.removeBook(book);
    book.setRemaingDays(14);
    user.addBorrowedBook(book);
}

void MainWindow::deleteBorrowBook(Book book)
{
    user.removeBorrowedBook(book);
    book.setRemaingDays(-1);
    database.addAvailableBook(book);
}


