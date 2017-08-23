#include "userregistration_form.h"
#include "ui_userregistration_form.h"

UserRegistration_Form::UserRegistration_Form(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::UserRegistration_Form)
{
    ui->setupUi(this);
    SettingInit();
    UserShow();
}

UserRegistration_Form::~UserRegistration_Form()
{
    delete ui;
}

void UserRegistration_Form::on_pushButton_Add_clicked()
{
    if(ui->lineEdit_CompanyNum->text().isEmpty() || ui->lineEdit_JoinDate->text().isEmpty()
            ||ui->lineEdit_Name->text().isEmpty() || ui->lineEdit_Password->text().isEmpty())
    {
        QMessageBox::information(this,tr("Information"),tr("LineEdit is Empty."),QMessageBox::Ok);
        return;
    }
    if(UserAdd())
    {
        UserShow();
        UIInit();
        QMessageBox::information(this,tr("Information"),tr("User Add"),QMessageBox::Ok);
    }
}

void UserRegistration_Form::on_pushButton_Modify_clicked()
{

}

void UserRegistration_Form::on_pushButton_Delete_clicked()
{

}

void UserRegistration_Form::DBInit()
{
    QSqlDatabase DB=QSqlDatabase::addDatabase("QSQLITE","MainDB");
    DB.setDatabaseName(Setting->value("Configuration/DBPath").toString());

    try
    {
        if(!DB.open())
        {
            QMessageBox::warning(this,tr("Warning"),QString("%1\n%2").arg(tr("Database open error!"),DB.lastError().text()),QMessageBox::Ok);
            QSqlDatabase::removeDatabase("MainDB");
            return;
        }
    }
    catch(QException &e)
    {
        QMessageBox::warning(this,tr("Warning"),QString("%1\n%2").arg(tr("Database Error!"),e.what()),QMessageBox::Ok);
        QSqlDatabase::removeDatabase("MainDB");
    }
}

void UserRegistration_Form::SettingInit()
{
    Setting=new QSettings("EachOne","ProjectManagement",this);
}

void UserRegistration_Form::UserShow()
{
    QSqlDatabase DB=QSqlDatabase::database("MainDB");

    try
    {
        if(!DB.isOpen())
        {
            QSqlDatabase::removeDatabase("MainDB");
            DBInit();
        }

        ui->tableWidget->clear();
        ui->tableWidget->setRowCount(0);
        TableWidgetInit();

        QSqlQuery query(DB);

        query.exec(QString("select * from user_management"));

        while(query.next())
        {
            ui->tableWidget->setRowCount(ui->tableWidget->rowCount()+1);
            ui->tableWidget->setItem(ui->tableWidget->rowCount()-1,COLUMN_COMPANYNUMBER,new QTableWidgetItem(query.value("companynumber").toString()));
            ui->tableWidget->setItem(ui->tableWidget->rowCount()-1,COLUMN_NAME,new QTableWidgetItem(query.value("name").toString()));
            ui->tableWidget->setItem(ui->tableWidget->rowCount()-1,COLUMN_JOINDATE,new QTableWidgetItem(query.value("joindate").toString()));
            ui->tableWidget->setItem(ui->tableWidget->rowCount()-1,COLUMN_RESIGNDATE,new QTableWidgetItem(query.value("resigndate").toString()));
            ui->tableWidget->setItem(ui->tableWidget->rowCount()-1,COLUMN_ADMIN,new QTableWidgetItem(query.value("admin").toString()));
            if(ui->tableWidget->rowCount()%2==0)
            {
                for(int i=0; i<ui->tableWidget->columnCount(); i++)
                {
                    ui->tableWidget->item(ui->tableWidget->rowCount()-1,i)->setBackgroundColor(qRgb(255,255,127));
                }
            }
        }

        ui->tableWidget->resizeColumnsToContents();
        ui->tableWidget->resizeRowsToContents();
        DB.close();
    }

    catch(QException &e)
    {
        QMessageBox::warning(this,tr("Warning"),QString("%1\n%2").arg(tr("Database Error!"),e.what()),QMessageBox::Ok);
        QSqlDatabase::removeDatabase("MainDB");
    }
}

void UserRegistration_Form::TableWidgetInit()
{
    ui->tableWidget->setColumnCount(5);
    ui->tableWidget->setHorizontalHeaderLabels(QStringList()<<tr("Companynumber")<<tr("Name")<<tr("JoinDate")<<tr("ResignDate")<<tr("Admin"));
}

bool UserRegistration_Form::UserAdd()
{
    QSqlDatabase DB=QSqlDatabase::database("MainDB");

    try
    {
        if(!DB.isOpen())
        {
            QSqlDatabase::removeDatabase("MainDB");
            DBInit();
        }
        QSqlQuery query(DB);

        query.exec(QString("insert into user_management(companynumber, name, password, joindate, resigndate, admin) values ('%1','%2','%3','%4','%5',%6)")
                   .arg(ui->lineEdit_CompanyNum->text(),ui->lineEdit_Name->text(),ui->lineEdit_Password->text(),ui->lineEdit_JoinDate->text(),ui->lineEdit_ResignDate->text())
                   .arg(ui->checkBox_Admin->isChecked()));

        if(query.lastError().number()>0)
        {
            switch(query.lastError().number())
            {
            case DB_ERROR_INSERT_PRIMARYKEY:
                QMessageBox::warning(this,tr("Warning"),tr("CompanyNumber is depulicate."),QMessageBox::Ok);
                break;
            default:
                QMessageBox::warning(this,tr("Warning"),QString("%1\n%2").arg(tr("Database Error!"),query.lastError().text()),QMessageBox::Ok);
                break;
            }
            return false;
        }
        DB.close();
    }

    catch(QException &e)
    {
        QMessageBox::warning(this,tr("Warning"),QString("%1\n%2").arg(tr("Database Error!"),e.what()),QMessageBox::Ok);
        QSqlDatabase::removeDatabase("MainDB");
    }
    return true;
}

void UserRegistration_Form::UIInit()
{
    ui->lineEdit_CompanyNum->setText("");
    ui->lineEdit_JoinDate->setText("");
    ui->lineEdit_Name->setText("");
    ui->lineEdit_Password->setText("");
    ui->lineEdit_ResignDate->setText("");
    ui->checkBox_Admin->setChecked(false);
}
