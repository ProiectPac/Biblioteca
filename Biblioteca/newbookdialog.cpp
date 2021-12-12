#include "newbookdialog.h"

void NewBookDialog::setupUI()
{
    QFormLayout *inputsLayout = new QFormLayout(this);
    QLineEdit *titleLineEdit = new QLineEdit(this);
    QLineEdit *ISBNLineEdit = new QLineEdit(this);
    QLineEdit *authorsLineEdit = new QLineEdit(this);
    QLineEdit *publicationYearLineEdit = new QLineEdit(this);
    QLineEdit *languageLineEdit = new QLineEdit(this);
    QLineEdit *ratingLineEdit = new QLineEdit(this);
    QLineEdit *imageURLLineEdit = new QLineEdit(this);
    QLineEdit *booksCountLineEdit = new QLineEdit(this);

    inputsLayout->addRow("Title: ",titleLineEdit);
    inputsLayout->addRow("ISBN: ",ISBNLineEdit);
    inputsLayout->addRow("Authors: ",authorsLineEdit);
    inputsLayout->addRow("Publication year: ",publicationYearLineEdit);
    inputsLayout->addRow("Rating: ",ratingLineEdit);
    inputsLayout->addRow("Language: ",languageLineEdit);
    inputsLayout->addRow("Number of copies: ",booksCountLineEdit);
    inputsLayout->addRow("image URL: ",imageURLLineEdit);

    QGroupBox *inputsBox = new QGroupBox(this);
    inputsBox->setLayout(inputsLayout);

    QPushButton *sendButton = new QPushButton(this);
    sendButton->setText("Send book");

    QVBoxLayout *dialogLayout = new QVBoxLayout(this);
    dialogLayout->addWidget(inputsBox);
    dialogLayout->addWidget(sendButton);

}

NewBookDialog::NewBookDialog(QWidget *parent):QDialog(parent)
{
    setupUI();
}



