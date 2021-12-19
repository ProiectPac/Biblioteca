#ifndef DELETEBOOKDIALOG_H
#define DELETEBOOKDIALOG_H
#include <QLineEdit>
#include <QFormLayout>
#include <QPushButton>
#include <QInputDialog>
#include <QGroupBox>

class DeleteBookDialog : public QDialog
{
    QLineEdit *titleLineEdit;
    QLineEdit *ISBNLineEdit;
    QLineEdit *idLineEdit;

    void setupUI();

public:
    DeleteBookDialog(QWidget *parent = nullptr);
    int getBookID();
};

#endif // DELETEBOOKDIALOG_H
