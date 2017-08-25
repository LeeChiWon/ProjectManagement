#ifndef USERREGISTRATION_FORM_H
#define USERREGISTRATION_FORM_H

#include <QWidget>
#include "define.h"
#include "tableprinter.h"

namespace Ui {
class UserRegistration_Form;
}

class UserRegistration_Form : public QWidget
{
    Q_OBJECT

public:
    explicit UserRegistration_Form(QWidget *parent = 0);
    ~UserRegistration_Form();

protected:
    void mouseReleaseEvent(QMouseEvent *event) override;

private slots:
    void on_pushButton_Add_clicked();
    void on_pushButton_Modify_clicked();
    void on_pushButton_Delete_clicked();
    void on_tableWidget_itemClicked(QTableWidgetItem *item);
    void on_pushButton_Search_clicked();
    void ExcelSave(bool clicked);
    void Print(bool clicked);
    void ContextMenuShow(const QPoint &pos);
    void TableWidgetPrint(QPrinter*);

private:
    Ui::UserRegistration_Form *ui;    
    QSettings *Setting;
    QMenu *ContextMenu;

    void DBInit();
    void SettingInit();
    void UserShow();
    void TableWidgetInit();
    bool UserAdd();
    bool UserModify();
    bool UserDelete();
    void UIInit();
    bool isEditEmpty();
    void ComboboxInit();
    void ContextMenuInit();
};

#endif // USERREGISTRATION_FORM_H
