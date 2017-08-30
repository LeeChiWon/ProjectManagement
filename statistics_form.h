#ifndef STATISTICS_FORM_H
#define STATISTICS_FORM_H

#include <QWidget>
#include "define.h"

namespace Ui {
class Statistics_Form;
}

class Statistics_Form : public QWidget
{
    Q_OBJECT

public:
    explicit Statistics_Form(QWidget *parent = 0);
    ~Statistics_Form();

private slots: 
    void on_pushButton_Search_SubjectNumber_clicked();

    void on_pushButton_Search_Recognition_clicked();

private:
    Ui::Statistics_Form *ui;
    QSettings *Setting;

    void ComboboxInit(int);
    void TableWidgetInit(int,QString);
    void DBInit();
    void DBShow(int, QString);
    QString QueryString(int,int);
    void SettingInit();
};

#endif // STATISTICS_FORM_H
