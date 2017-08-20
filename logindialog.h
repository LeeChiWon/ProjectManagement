#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QDialog>

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

private slots:
    void on_pushButton_Login_clicked();

private:
    Ui::LoginDialog *ui;

    void BgInit();
};

#endif // LOGINDIALOG_H
