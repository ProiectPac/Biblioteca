#ifndef NEWBOOKDIALOG_H
#define NEWBOOKDIALOG_H
#include<QDialog>
#include <QFormLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QGroupBox>
class NewBookDialog: public QDialog
{
private:
    void setupUI();
public:
    NewBookDialog(QWidget *parent = nullptr);
};

#endif // NEWBOOKDIALOG_H
