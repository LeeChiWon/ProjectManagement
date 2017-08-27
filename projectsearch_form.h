#ifndef PROJECTSEARCH_FORM_H
#define PROJECTSEARCH_FORM_H

#include <QWidget>
#include "define.h"

namespace Ui {
class ProjectSearch_Form;
}

class ProjectSearch_Form : public QWidget
{
    Q_OBJECT

public:
    explicit ProjectSearch_Form(QWidget *parent = 0);
    ~ProjectSearch_Form();

private slots:
    void on_pushButton_Search_clicked();
    void on_pushButton_Delete_clicked();
    void on_pushButton_Print_clicked();
    void on_pushButton_ExcelSave_clicked();


private:
    Ui::ProjectSearch_Form *ui;
    QSettings *Setting;

    void DBInit();
    void SettingInit();
    void Search(QString);
    void TableWidgetInit();
};

#endif // PROJECTSEARCH_FORM_H
