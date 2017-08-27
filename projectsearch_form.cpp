#include "projectsearch_form.h"
#include "ui_projectsearch_form.h"

ProjectSearch_Form::ProjectSearch_Form(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ProjectSearch_Form)
{
    ui->setupUi(this);

    ui->dateEdit_AccountsCompleteDate->setDate(QDate::currentDate());
    ui->dateEdit_AccountsCompleteDate_2->setDate(QDate::currentDate());
    ui->dateEdit_AgreementStart->setDate(QDate::currentDate());
    ui->dateEdit_AgreementStart_2->setDate(QDate::currentDate());
    ui->dateEdit_ReceiptDate->setDate(QDate::currentDate());
    ui->dateEdit_ReceiptDate_2->setDate(QDate::currentDate());

    SettingInit();
    TableWidgetInit();
}

ProjectSearch_Form::~ProjectSearch_Form()
{
    delete ui;
}

void ProjectSearch_Form::on_pushButton_Search_clicked()
{

}

void ProjectSearch_Form::on_pushButton_Delete_clicked()
{

}

void ProjectSearch_Form::on_pushButton_Print_clicked()
{

}

void ProjectSearch_Form::on_pushButton_ExcelSave_clicked()
{

}

void ProjectSearch_Form::DBInit()
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

void ProjectSearch_Form::SettingInit()
{
    Setting=new QSettings("EachOne","ProjectManagement",this);
}

void ProjectSearch_Form::Search(QString QueryStr)
{
    ui->tableWidget->setSortingEnabled(false);
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

        query.exec(QueryStr);

        while(query.next())
        {
           /* ui->tableWidget->setRowCount(ui->tableWidget->rowCount()+1);
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
            }*/
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
    ui->tableWidget->setSortingEnabled(true);
}

void ProjectSearch_Form::TableWidgetInit()
{
    ui->tableWidget->setColumnCount(9);
    ui->tableWidget->setHorizontalHeaderLabels(QStringList()<<tr("BusinessType")<<tr("ProjectNumber")<<tr("Managementagency")<<tr("BusinessGroup1")<<tr("BusinessGroup2")<<tr("BusinessGroup3")
                                               <<tr("Organization")<<tr("Manager")<<tr("ProjectName"));
}
