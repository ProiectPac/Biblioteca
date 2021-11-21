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
    static unsigned int FNVHash(QString str);
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
    QPushButton *loginButton = nullptr;
    QPushButton *registerButton = nullptr;
    QLabel *nameLabel = nullptr;
    QLabel *passwordLabel = nullptr;
    QGridLayout *gridLayout = nullptr;
    QLineEdit *nameLineEdit = nullptr;
    QLineEdit *passwordLineEdit = nullptr;
    unsigned int FNVHash(const QString& str);
private slots:
    void loginOnClick();
    void registerOnClick();
};

#endif // LOGINDIALOG_H
