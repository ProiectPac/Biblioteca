#include "mainwindow.h"
#include <QApplication>
#include <QErrorMessage>
#include "logindialog.h"
#include <memory>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.showMaximized();
    return a.exec();
}
