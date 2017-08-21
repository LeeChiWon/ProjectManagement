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

    void closeEvent(QCloseEvent *event);
    void BgInit();
    void SettingInit();
    void SplashScreen();
};

#endif // LOGINDIALOG_H
