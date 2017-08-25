#ifndef PROJECTINPUT_FORM_H
#define PROJECTINPUT_FORM_H

#include <QWidget>
#include "define.h"

namespace Ui {
class ProjectInput_Form;
}

class ProjectInput_Form : public QWidget
{
    Q_OBJECT

public:
    explicit ProjectInput_Form(QWidget *parent = 0);
    ~ProjectInput_Form();

private slots:
    void on_pushButton_Save_clicked();
    void on_pushButton_Upload_clicked();
    void on_lineEdit_TotalProjectCost_editingFinished();
    void on_lineEdit_LardaceousSpleen_editingFinished();
    void on_lineEdit_Deductible_Cash_editingFinished();
    void on_lineEdit_Deductible_Goods_editingFinished();
    void on_lineEdit_ProjectCost_editingFinished();
    void on_lineEdit_CarriedCost_editingFinished();
    void on_lineEdit_Contribution_editingFinished();

private:
    Ui::ProjectInput_Form *ui;
    QSettings *Setting;

    void UIInit();
    void DBInit();
    void SettingInit();
    bool Input(QString);
    bool isEditEmpty();
    bool ExcelUpload(QString);
};

#endif // PROJECTINPUT_FORM_H
