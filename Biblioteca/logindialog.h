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
    LoginDialog(QWidget *parent = nullptr);
    QPair<QString, unsigned int> getUserCredentials();
    static unsigned int FNVHash(const QString& str);
    enum class Actions
    {
        Nothing,
        Login,
        Register
    } action;

    Actions getAction() const;

    ~LoginDialog();
public slots:
    void open() override;
private:
    QLineEdit *nameLineEdit = nullptr;
    QLineEdit *passwordLineEdit = nullptr;
private slots:
    void loginOnClick();
    void registerOnClick();
};

#endif // LOGINDIALOG_H
