#include "mainwindow.h"
#include <QApplication>
#include <QErrorMessage>
#include "logindialog.h"
#include "database.h"
#include <memory>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
