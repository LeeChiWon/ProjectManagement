#include "logindialog.h"
#include "ui_logindialog.h"

LoginDialog::LoginDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LoginDialog),
    isReject(false)
{
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint);//|Qt::WindowStaysOnTopHint);
    SettingInit();
    BgInit();
}

LoginDialog::~LoginDialog()
{
    delete ui;
}

void LoginDialog::reject()
{
    if(isReject)
    {
        QDialog::reject();
    }
}

void LoginDialog::BgInit()
{
    QPixmap Bg(":/img/login.png");
    Bg=Bg.scaled(this->size(),Qt::KeepAspectRatioByExpanding);//Qt::IgnoreAspectRatio);
    QPalette Palette;
    Palette.setBrush(QPalette::Background,Bg);
    this->setPalette(Palette);
}

void LoginDialog::SettingInit()
{
    Setting=new QSettings("EachOne","ProjectManagement",this);
    ui->checkBox_IDSave->setChecked(Setting->value("Login/LoginCheckBox").toBool());
    ui->lineEdit_ID->setText(Setting->value("Login/LoginID").toString());

}

void LoginDialog::SplashScreen()
{
    QSplashScreen *splashScreen=new QSplashScreen;
    splashScreen->setPixmap(QPixmap(":/img/login.png"));
    splashScreen->show();

    QTimer::singleShot(3000,splashScreen,SLOT(close()));
    QTimer::singleShot(3000,this,SLOT(LoginShow()));
}

void LoginDialog::on_pushButton_Login_clicked()
{
    if(ui->lineEdit_ID->text()=="admin" && ui->lineEdit_Password->text()=="admin")
    {
        SplashScreen();
    }

    LoginID=ui->lineEdit_ID->text();


}

void LoginDialog::closeEvent(QCloseEvent *event)
{
    event->ignore();
    if(ui->checkBox_IDSave->isChecked())
    {
        Setting->setValue("Login/LoginCheckBox",ui->checkBox_IDSave->isChecked());
        Setting->setValue("Login/LoginID",ui->lineEdit_ID->text());
    }

    else
    {
        Setting->remove("Login/LoginCheckBox");
        Setting->remove("Login/LoginID");
    }
    event->accept();
}

void LoginDialog::on_pushButton_Exit_clicked()
{
    this->close();
}

void LoginDialog::LoginShow()
{
    emit LoggedIn();
    LoginLevel=LOGIN_MASTER;
    this->close();
}
