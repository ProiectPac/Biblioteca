#include "newbookdialog.h"

void NewBookDialog::setupUI()
{
    QFormLayout *inputsLayout = new QFormLayout(this);
    titleLineEdit = new QLineEdit(this);
    ISBNLineEdit = new QLineEdit(this);
    authorsLineEdit = new QLineEdit(this);
    publicationYearLineEdit = new QLineEdit(this);
    languageLineEdit = new QLineEdit(this);
    ratingLineEdit = new QLineEdit(this);
    imageURLLineEdit = new QLineEdit(this);
    booksCountLineEdit = new QLineEdit(this);

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

Book NewBookDialog::getBook()
{
    QString title = titleLineEdit->text();
    QString author = authorsLineEdit->text();
    QString ISBN = ISBNLineEdit->text();
    int booksCount = booksCountLineEdit->text().toInt();
    int originalPublicationYear = publicationYearLineEdit->text().toInt();
    QString language = languageLineEdit->text();
    float averageRating = ratingLineEdit->text().toFloat();
    QString imageURL = imageURLLineEdit->text();

    Book newBook(0,ISBN,author,originalPublicationYear,title,language,averageRating, imageURL, "", booksCount);
    return newBook;

}
NewBookDialog::NewBookDialog(QWidget *parent):QDialog(parent)
{
    setupUI();
}



