#include "deletebookdialog.h"

void DeleteBookDialog::setupUI()
{
    QFormLayout *inputsLayout = new QFormLayout(this);
    idLineEdit = new QLineEdit(this);
    inputsLayout->addRow("ID: ",idLineEdit);

    QGroupBox *inputsBox = new QGroupBox(this);
    inputsBox->setLayout(inputsLayout);

    QPushButton *deleteButton = new QPushButton(this);
    deleteButton->setText("Delete book");
    connect(deleteButton,&QPushButton::clicked,this,&QDialog::accept);

    QVBoxLayout *dialogLayout = new QVBoxLayout(this);
    dialogLayout->addWidget(inputsBox);
    dialogLayout->addWidget(deleteButton);

    setLayout(dialogLayout);
}

DeleteBookDialog::DeleteBookDialog(QWidget *parent) : QDialog(parent)
{
    setupUI();
}

int DeleteBookDialog::getBookID()
{
    return idLineEdit->text().toInt();
}
