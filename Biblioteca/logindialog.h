#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QPushButton>
#include <QGridLayout>
#include <QLabel>
#include <QString>
#include <QPair>

class LoginDialog : public QDialog
{
public:
    LoginDialog();
    QPair<QString,QString> getUserCredentials();
private:
    QLineEdit *nameLineEdit;
    QLineEdit *passwordLineEdit;
};

#endif // LOGINDIALOG_H
