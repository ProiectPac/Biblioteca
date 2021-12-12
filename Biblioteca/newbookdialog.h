#ifndef NEWBOOKDIALOG_H
#define NEWBOOKDIALOG_H
#include<QDialog>
#include <QFormLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QGroupBox>
#include "book.h"
class NewBookDialog: public QDialog
{
private:
    QLineEdit *titleLineEdit;
    QLineEdit *ISBNLineEdit;
    QLineEdit *authorsLineEdit;
    QLineEdit *publicationYearLineEdit;
    QLineEdit *languageLineEdit;
    QLineEdit *ratingLineEdit;
    QLineEdit *imageURLLineEdit;
    QLineEdit *booksCountLineEdit;

    void setupUI();

public:
    NewBookDialog(QWidget *parent = nullptr);
    Book getBook();
};

#endif // NEWBOOKDIALOG_H
