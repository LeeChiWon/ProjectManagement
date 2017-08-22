#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtWidgets>
#include "configuration_form.h"
#include "main_form.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_pushButton_Logout_clicked();
    void on_actionConfiguration_triggered(bool checked);
    void onSystemTryIconClicked(QSystemTrayIcon::ActivationReason);
    void on_actionExit_triggered();
    void TrayIconInit();
    void UserInfo(QString,QString,int);
    void on_tabWidget_tabCloseRequested(int index);

protected:
    void hideEvent(QHideEvent *event);

private:
    Ui::MainWindow *ui;
    QSystemTrayIcon *TrayIcon;
    QMenu *TrayIconMenu;
    int CloseMode;
    QString LoginID;
    QString User;
    int LoginLevel;

    void closeEvent(QCloseEvent *event);   
    bool TabWidgetCompare(QString);
    void TabWidgetInit();

};

#endif // MAINWINDOW_H
