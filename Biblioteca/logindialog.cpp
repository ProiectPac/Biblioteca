#include "logindialog.h"




LoginDialog::LoginDialog()
{
    this->resize(200,200);
    this->setWindowTitle("Login");

    nameLineEdit = new QLineEdit(this);
    passwordLineEdit = new QLineEdit(this);

    QPushButton *loginButton = new QPushButton("Login", this);
    QObject::connect(loginButton,&QPushButton::clicked,this,&LoginDialog::accept);
    QPushButton *registerButton = new QPushButton("Register", this);
    QObject::connect(loginButton,&QPushButton::clicked,this,&LoginDialog::reject);

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

User LoginDialog::getUserCredentials()
{
    return User(nameLineEdit->text(),passwordLineEdit->text());
}
