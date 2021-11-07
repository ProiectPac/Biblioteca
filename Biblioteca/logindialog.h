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
    enum class Actions
    {
        Nothing,
        Login,
        Register
    } action;

    Actions getAction() const;

private:
    QLineEdit *nameLineEdit;
    QLineEdit *passwordLineEdit;
    unsigned int FNVHash(QString str);

private slots:
    void loginOnClick();
    void registerOnClick();
};

#endif // LOGINDIALOG_H
