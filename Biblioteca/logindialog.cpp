#include "logindialog.h"




LoginDialog::LoginDialog()
{
    this->resize(200,200);
    this->setWindowTitle("Login");



    QLineEdit *nameLineEdit = new QLineEdit(this);
    QLineEdit *passwordLineEdit = new QLineEdit(this);

    QPushButton *loginButton = new QPushButton("Login", this);
    QPushButton *registerButton = new QPushButton("Register", this);

    QLabel *nameLabel = new QLabel(tr("Name:"));
    nameLabel->setBuddy(nameLineEdit);

    QLabel *passwordLabel = new QLabel(tr("Password:"));
    passwordLabel->setBuddy(passwordLineEdit);

    QGridLayout *gridLayout = new QGridLayout;

    gridLayout->addWidget(nameLabel, 0, 0);
    gridLayout->addWidget(nameLineEdit, 0, 1);
    gridLayout->addWidget(passwordLabel, 1, 0);
    gridLayout->addWidget(passwordLineEdit, 1, 1);
    gridLayout->addWidget(loginButton, 2, 0);
    gridLayout->addWidget(registerButton, 2, 1);
    setLayout(gridLayout);
}
