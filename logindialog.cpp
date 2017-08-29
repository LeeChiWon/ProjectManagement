#include "logindialog.h"
#include "ui_logindialog.h"

LoginDialog::LoginDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LoginDialog),
    isReject(false),
    LoginLevel(0)
{
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint);//|Qt::WindowStaysOnTopHint);
    SettingInit();
    BgInit();
    DBInit();
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
    this->hide();
    QSplashScreen *splashScreen=new QSplashScreen;
    splashScreen->setPixmap(QPixmap(":/img/login.png"));
    splashScreen->show();

    QTimer::singleShot(2000,splashScreen,SLOT(close()));
    QTimer::singleShot(2000,this,SLOT(LoginShow()));
}

void LoginDialog::on_pushButton_Login_clicked()
{
    if(ui->lineEdit_ID->text().isEmpty() || ui->lineEdit_ID->text().isNull() ||
            ui->lineEdit_Password->text().isEmpty() || ui->lineEdit_Password->text().isNull())
    {
         QMessageBox::information(this,tr("Information"),tr("LineEdit is empty."),QMessageBox::Ok);
         return;
    }

    if(ui->lineEdit_ID->text()=="admin" && ui->lineEdit_Password->text()=="admin")
    {
        LoginName=tr("Master");
        LoginLevel=LOGIN_MASTER;
    }

    else
    {
        if(!IsCheckLogin())
        {
            QMessageBox::warning(this,tr("Warning"),tr("Check your ID/Password."),QMessageBox::Ok);
            return;
        }
        if(!ResignDate.isEmpty() || !ResignDate.isNull())
        {
            QMessageBox::warning(this,tr("Warning"),tr("Resign ID."),QMessageBox::Ok);
            return;
        }
    }
    SplashScreen();
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
    emit UserInfo(ui->lineEdit_ID->text(),LoginName,LoginLevel);
    this->close();
}

void LoginDialog::DBInit()
{
    QSqlDatabase DB=QSqlDatabase::addDatabase("QSQLITE","MainDB");
    if(Setting->value("Configuration/DBPath").toString().isEmpty()
            || Setting->value("Configuration/DBPath").toString().isNull())
    {
        DB.setDatabaseName(QApplication::applicationDirPath()+"/ProjectManagement.db");
    }

    else
    {
        DB.setDatabaseName(Setting->value("Configuration/DBPath").toString());
    }

    try
    {
        if(!DB.open())
        {
            switch(DB.lastError().number())
            {
            case -1:
                QMessageBox::warning(this,tr("Warning"),QString("%1\n%2").arg(tr("Database open error!"),tr("Database is not exist!")),QMessageBox::Ok);
                break;
            default:
                QMessageBox::warning(this,tr("Warning"),QString("%1\n%2").arg(tr("Database open error!"),DB.lastError().text()),QMessageBox::Ok);
                break;
            }
            exit(0);
            QSqlDatabase::removeDatabase("MainDB");            
            return;
        }

        // CREATE TABLE "main_tb" ( `idx` INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, `date` TEXT, `content` TEXT, `separation` TEXT, `writer` TEXT )

        QSqlQuery query(DB);
        query.exec(QString("create table if not exists user_management (companynumber text not null primary key, name text not null, password text not null,"
                           " joindate date not null, resigndate date, admin integer)"));
        query.exec(QString("create table if not exists project_management (businesstype text, projectnumber text, managementagency text, businessgroup1 text, businessgroup2 text,businessgroup3 text,"
                           "organization text, manager text, projectname text not null primary key, agreementstartdate date, agreementenddate date, contribution text,lardaceousspleen text,"
                           "deductible_cash text, deductible_goods text, projectcost text, totalprojectcost text, carriedcost text,"
                           "projectstate text, receiptdate date, accountsnotifydate date, accountscompletedate date, execution_cash text, execution_goods text, executionbalance text, appearinterest text,interestuse text,"
                           "interestbalance text, carriedexecution text, carriedbusiness text, recognition text, governmentsubsidy text, return text,accountperson text,"
                           "acttach1 text,acttach2 text,acttach3 text,acttach4 text,acttach5 text,memo text,acttachpath text)"));
        DB.close();
    }
    catch(QException &e)
    {
        QMessageBox::warning(this,tr("Warning"),QString("%1\n%2").arg(tr("Database Error!"),e.what()),QMessageBox::Ok);
        QSqlDatabase::removeDatabase("MainDB");
        return;
    }   
}

bool LoginDialog::IsCheckLogin()
{
    QSqlDatabase DB=QSqlDatabase::database("MainDB");

    try
    {
        if(!DB.isOpen())
        {
            //QMessageBox::warning(this,tr("Warning"),QString("%1\n%2").arg(tr("Database open error!"),DB.lastError().text()),QMessageBox::Ok);
            QSqlDatabase::removeDatabase("MainDB");
            DBInit();
           // return false;
        }

        QSqlQuery query(DB);
        query.exec(QString("select * from user_management where companynumber='%1' and password='%2'").arg(ui->lineEdit_ID->text(),ui->lineEdit_Password->text()));

        if(query.next())
        {
            LoginName=query.value("name").toString();
            LoginLevel=query.value("admin").toInt();
            ResignDate=query.value("resigndate").toString();
            DB.close();
        }
        else
        {
            DB.close();
            return false;
        }
    }
    catch(QException &e)
    {
        QMessageBox::warning(this,tr("Warning"),QString("%1\n%2").arg(tr("Database Error!"),e.what()),QMessageBox::Ok);
        QSqlDatabase::removeDatabase("MainDB");
        return false;
    }
    return true;
}
