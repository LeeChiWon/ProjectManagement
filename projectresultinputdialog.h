#ifndef PROJECTRESULTINPUTDIALOG_H
#define PROJECTRESULTINPUTDIALOG_H

#include <QDialog>
#include "define.h"

namespace Ui {
class ProjectResultInputDialog;
}

class ProjectResultInputDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ProjectResultInputDialog(QWidget *parent = 0);
    ~ProjectResultInputDialog();

private slots:
    void on_pushButton_Close_clicked();

private:
    Ui::ProjectResultInputDialog *ui;
};

#endif // PROJECTRESULTINPUTDIALOG_H
