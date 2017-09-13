#ifndef STATISTICS_FORM_H
#define STATISTICS_FORM_H

#include <QWidget>
#include "define.h"
#include "subjectlistdialog.h"

namespace Ui {
class Statistics_Form;
}

class Statistics_Form : public QWidget
{
    Q_OBJECT

public:
    explicit Statistics_Form(QWidget *parent = 0);
    ~Statistics_Form();
signals:
    void setTitle(QString);
    void DBShow(QString);
private slots: 
    void on_pushButton_Search_SubjectNumber_clicked();
    void on_pushButton_Search_Recognition_clicked();
    void on_tableWidget_SubjectNumber_doubleClicked(const QModelIndex &index);
    void on_tableWidget_Recognition_doubleClicked(const QModelIndex &index);

private:
    Ui::Statistics_Form *ui;
    QSettings *Setting;

    void ComboboxInit(int);
    void TableWidgetInit(int);
    void DBInit();
    void DBShow(int, QString);
    QString QueryString(int);
    void SettingInit();   
};

#endif // STATISTICS_FORM_H
