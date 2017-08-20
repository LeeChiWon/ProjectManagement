#include "logindialog.h"
#include "ui_logindialog.h"

LoginDialog::LoginDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LoginDialog)
{
    ui->setupUi(this);

    BgInit();
}

LoginDialog::~LoginDialog()
{
    delete ui;
}

void LoginDialog::BgInit()
{
     QPixmap Bg(":/img/login.png");
     Bg=Bg.scaled(this->size(),Qt::KeepAspectRatioByExpanding);//Qt::IgnoreAspectRatio);
     QPalette Palette;
     Palette.setBrush(QPalette::Background,Bg);
     this->setPalette(Palette);
}

void LoginDialog::on_pushButton_Login_clicked()
{
    if(ui->lineEdit_ID->text()=="admin" && ui->lineEdit_Password->text()=="admin")
    {
        emit LoggedIn();
        this->close();
    }


}
