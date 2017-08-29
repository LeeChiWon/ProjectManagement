#ifndef CONFIGURATION_FORM_H
#define CONFIGURATION_FORM_H

#include <QWidget>
#include "define.h"

namespace Ui {
class Configuration_form;
}

class Configuration_form : public QWidget
{
    Q_OBJECT

public:
    explicit Configuration_form(QWidget *parent = 0);
    ~Configuration_form();

private slots:
    void on_pushButton_DBPath_clicked();
    void on_pushButton_Apply_clicked();
    void on_treeWidget_itemClicked(QTreeWidgetItem *item, int column);

    void on_pushButton_PasswordChange_clicked();

private:
    Ui::Configuration_form *ui;
    QSettings *Setting;

    void TabWidgetInit();
    void TreeWidgetInit();
    void SettingInit();
    void AutoStartChecked(bool);
    void DBInit();
    bool ChangePassword();
};

#endif // CONFIGURATION_FORM_H
