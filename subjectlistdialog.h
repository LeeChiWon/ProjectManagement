#ifndef SUBJECTLISTDIALOG_H
#define SUBJECTLISTDIALOG_H

#include <QDialog>
#include "define.h"

namespace Ui {
class SubjectListDialog;
}

class SubjectListDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SubjectListDialog(QWidget *parent = 0);
    ~SubjectListDialog();

private slots:
    void SetTitle(QString);
    void DBShow(QString);
    void ExcelSave(bool clicked);
    void on_listWidget_customContextMenuRequested(const QPoint &pos);

private:
    Ui::SubjectListDialog *ui;
    QSettings *Setting;
    QMenu *ContextMenu;

    void DBInit();
    void SettingInit();
    void ContextMenuInit();
};

#endif // SUBJECTLISTDIALOG_H
