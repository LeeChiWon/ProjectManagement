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
    void on_lineEdit_Excution_Cash_editingFinished();
    void on_lineEdit_Excution_Goods_editingFinished();
    void on_lineEdit_ExcutionBalance_editingFinished();
    void on_lineEdit_Appearinterest_editingFinished();
    void on_lineEdit_Interestuse_editingFinished();
    void on_lineEdit_Interestbalance_editingFinished();
    void on_lineEdit_Carriedexecution_editingFinished();
    void on_lineEdit_Carriedbusiness_editingFinished();
    void on_lineEdit_Recognition_editingFinished();
    void on_lineEdit_Return_editingFinished();
    void ViewPrint(QPrinter*);

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
