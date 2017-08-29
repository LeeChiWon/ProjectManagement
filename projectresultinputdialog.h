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
    void InitString(QStringList);

private slots:
    void on_pushButton_Close_clicked();
    void on_pushButton_Save_clicked();
    void on_pushButton_Print_clicked();

private:
    Ui::ProjectResultInputDialog *ui;
    QSettings *Setting;

    void ComboboxInit();
    void DBInit();
    QString QueryString(int);
    void DBShow(QString);
    void SettingInit();
    bool DBSave(QString);
};

#endif // PROJECTRESULTINPUTDIALOG_H
