#ifndef SUBJECTLISTDIALOG_H
#define SUBJECTLISTDIALOG_H

#include <QDialog>
#include "define.h"

namespace Ui {
class SubjectListDialog;
}

class SubjectListDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SubjectListDialog(QWidget *parent = 0);
    ~SubjectListDialog();

private slots:
    void SetTitle(QString);
    void DBShow(QString);

private:
    Ui::SubjectListDialog *ui;
    QSettings *Setting;

    void DBInit();
    void SettingInit();
};

#endif // SUBJECTLISTDIALOG_H
