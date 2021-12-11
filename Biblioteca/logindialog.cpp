#include "logindialog.h"




LoginDialog::LoginDialog(QWidget *parent) : QDialog(parent)
{

    action = LoginDialog::Actions::Nothing;

    this->resize(200,200);
    this->setWindowTitle("Login");

    nameLineEdit = new QLineEdit(this);
    passwordLineEdit = new QLineEdit(this);

    QPushButton *loginButton = new QPushButton("Login", this);
    QObject::connect(loginButton,&QPushButton::clicked,this,&LoginDialog::loginOnClick);
    QPushButton *registerButton = new QPushButton("Register", this);
    QObject::connect(registerButton,&QPushButton::clicked,this,&LoginDialog::registerOnClick);

    QLabel *nameLabel = new QLabel(tr("Name:"), this);
    nameLabel->setBuddy(nameLineEdit);

    QLabel *passwordLabel = new QLabel(tr("Password:"), this);
    passwordLabel->setBuddy(passwordLineEdit);

    QGridLayout *gridLayout = new QGridLayout(this);

    gridLayout->addWidget(nameLabel, 0, 0);
    gridLayout->addWidget(nameLineEdit, 0, 1);
    gridLayout->addWidget(passwordLabel, 1, 0);
    gridLayout->addWidget(passwordLineEdit, 1, 1);
    gridLayout->addWidget(loginButton, 2, 0);
    gridLayout->addWidget(registerButton, 2, 1);
    setLayout(gridLayout);
}

QPair<QString, unsigned int> LoginDialog::getUserCredentials()
{
    return QPair<QString, unsigned int>(nameLineEdit->text(),FNVHash(passwordLineEdit->text()));
}

LoginDialog::Actions LoginDialog::getAction() const
{
    return action;
}

LoginDialog::~LoginDialog()
{

}

void LoginDialog::open()
{
    action=LoginDialog::Actions::Nothing;
    QDialog::open();
}

unsigned int LoginDialog::FNVHash(const QString& str)
{
    const unsigned int fnv_prime = 0x811C9DC5;
    unsigned int hash = 0;
    unsigned int i = 0;
    unsigned int len = str.length();

    for (i = 0; i < len; i++)
    {
        hash *= fnv_prime;
        hash ^= (str.toStdString()[i]);
    }
    return hash;
}

void LoginDialog::loginOnClick()
{
    action=LoginDialog::Actions::Login;
    QDialog::accept();
}

void LoginDialog::registerOnClick()
{
    action=LoginDialog::Actions::Register;
    QDialog::reject();
}
