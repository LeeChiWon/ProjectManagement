#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QDialog>
#include "define.h"

namespace Ui {
class LoginDialog;
}

class LoginDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LoginDialog(QWidget *parent = 0);
    ~LoginDialog();

signals:
    void LoggedIn();
    void UserInfo(QString,QString,int);

public slots:
    void reject();

private slots:
    void on_pushButton_Login_clicked();
    void on_pushButton_Exit_clicked();
    void LoginShow();

private:
    Ui::LoginDialog *ui;
    QSettings *Setting;
    bool isReject;
    QTranslator Translator;
    QString LoginName;
    int LoginLevel;
    QString ResignDate;

    void closeEvent(QCloseEvent *event);
    void BgInit();
    void SettingInit();
    void SplashScreen();
    void DBInit();
    bool IsCheckLogin();
};

#endif // LOGINDIALOG_H
