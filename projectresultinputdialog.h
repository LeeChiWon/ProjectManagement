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

protected:
    void mouseDoubleClickEvent(QMouseEvent* event);

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
    void on_pushButton_Acttach1_clicked();
    void on_pushButton_Acttach2_clicked();
    void on_pushButton_Acttach3_clicked();
    void on_pushButton_Acttach4_clicked();
    void on_pushButton_Acttach5_clicked();

private:
    Ui::ProjectResultInputDialog *ui;
    QSettings *Setting;
    QString Acttach[5],InputActtach[5],ActtachPath;
    QString ProjectNumber;

    void ComboboxInit();
    void DBInit();
    QString QueryString(int);
    void DBShow(QString);
    void SettingInit();
    bool DBSave(QString);
};

#endif // PROJECTRESULTINPUTDIALOG_H
