#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include "user.h"

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
    QPair<QString, unsigned int> getUserCredentials();
private:
    QLineEdit *nameLineEdit;
    QLineEdit *passwordLineEdit;
    unsigned int FNVHash(QString str);
};

#endif // LOGINDIALOG_H
