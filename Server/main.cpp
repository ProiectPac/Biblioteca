#include <QCoreApplication>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QTextStream(stdout)<<"da";
    return a.exec();
}
