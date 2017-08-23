#ifndef USERREGISTRATION_FORM_H
#define USERREGISTRATION_FORM_H

#include <QWidget>
#include "define.h"

namespace Ui {
class UserRegistration_Form;
}

class UserRegistration_Form : public QWidget
{
    Q_OBJECT

public:
    explicit UserRegistration_Form(QWidget *parent = 0);
    ~UserRegistration_Form();

private slots:
    void on_pushButton_Add_clicked();
    void on_pushButton_Modify_clicked();
    void on_pushButton_Delete_clicked();

private:
    Ui::UserRegistration_Form *ui;    
    QSettings *Setting;

    void DBInit();
    void SettingInit();
    void UserShow();
    void TableWidgetInit();
    bool UserAdd();
    void UIInit();
};

#endif // USERREGISTRATION_FORM_H
