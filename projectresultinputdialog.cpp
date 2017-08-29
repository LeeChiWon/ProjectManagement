#include "projectresultinputdialog.h"
#include "ui_projectresultinputdialog.h"

ProjectResultInputDialog::ProjectResultInputDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ProjectResultInputDialog)
{    
    ui->setupUi(this);
    SettingInit();
    ComboboxInit();
    DBShow(QueryString(INIT));
}

ProjectResultInputDialog::~ProjectResultInputDialog()
{
    delete ui;
}

void ProjectResultInputDialog::on_pushButton_Close_clicked()
{
    this->close();
}

void ProjectResultInputDialog::ComboboxInit()
{
    ui->comboBox_ProjectState->clear();
    ui->comboBox_ProjectState->addItems(QStringList()<<tr("ReceiptDate")<<tr("AccountsNotifyDate")<<tr("AccountsCompleteDate"));
}

void ProjectResultInputDialog::InitString(QStringList List)
{
    ui->lineEdit_ManagementAgency->setText(List.at(0));
    ui->lineEdit_ProjectName->setText(List.at(1));
    ui->lineEdit_Organization->setText(List.at(2));
    ui->lineEdit_AccountPerson->setText(g_User);
}

void ProjectResultInputDialog::DBInit()
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

QString ProjectResultInputDialog::QueryString(int Cmd)
{
    QString Query;

    switch (Cmd) {
    case INIT:
        Query=QString("select * from project_management where projectname='%1'").arg(ui->lineEdit_ProjectName->text());
        break;
    case UPDATE:
        Query=QString("update project_management set projectstate='%1'").arg(ui->comboBox_ProjectState->currentText());

        if(ui->dateEdit_ReceiptDate->text()!="1900-01-01")
        {
            Query.append(QString(",receiptdate='%1'").arg(ui->dateEdit_ReceiptDate->text()));
        }

        if(ui->dateEdit_AccountsNotifyDate->text()!="1900-01-01")
        {
            Query.append(QString(",accountsnotifydate='%1'").arg(ui->dateEdit_AccountsNotifyDate->text()));
        }

        if(ui->dateEdit_AccountsCompleteDate->text()!="1900-01-01")
        {
            Query.append(QString(",accountscompletedate='%1'").arg(ui->dateEdit_AccountsCompleteDate->text()));
        }

        Query.append(QString(",excution_cash='%1',excution_goods='%2',excutionbalance='%3',appearinterest='%4',interestuse='%5',interestbalance='%6'"
                             ",carriedexecution='%7',carriedbusiness='%8',recognition='%9',governmentsubsidy='%10',return='%11',acttach1='%12'"
                             ",acttach2='%13',acttach3='%14',acttach4='%15',acttach5='%16' ").arg(ui->lineEdit_Excution_Cash->text(),ui->lineEdit_Excution_Goods->text())
                     .arg(ui->lineEdit_ExcutionBalance->text(),ui->lineEdit_Appearinterest->text(),ui->lineEdit_Interestuse->text(),ui->lineEdit_Interestbalance->text())
                     .arg(ui->lineEdit_Carriedexecution->text(),ui->lineEdit_Carriedbusiness->text(),ui->lineEdit_Recognition->text(),ui->lineEdit_Governmentsubsidy->text())
                     .arg(ui->lineEdit_Return->text(),ui->lineEdit_Acttach1->text(),ui->lineEdit_Acttach2->text(),ui->lineEdit_Acttach3->text(),ui->lineEdit_Acttach4->text()
                          ,ui->lineEdit_Acttach5->text()));

        Query.append("where projectname='%1'").arg(ui->lineEdit_ProjectName->text());
        break;

    }
    return Query;
}

void ProjectResultInputDialog::DBShow(QString QueryStr)
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
        query.exec(QueryStr);

        while(query.next())
        {
            for(int i=0; i<ui->comboBox_ProjectState->count(); i++)
            {
                if(ui->comboBox_ProjectState->itemText(i)==query.value("projectstate").toString())
                {
                    ui->comboBox_ProjectState->setCurrentIndex(i);
                    break;
                }
            }
            if(!query.value("receiptdate").toString().isEmpty())
            {
                ui->dateEdit_ReceiptDate->setDate(QDate::fromString(query.value("receiptdate").toString(),"yyyy-MM-dd"));
            }
            if(!query.value("accountsnotifydate").toString().isEmpty())
            {
                ui->dateEdit_AccountsNotifyDate->setDate(QDate::fromString(query.value("accountsnotifydate").toString(),"yyyy-MM-dd"));
            }
            if(!query.value("accountscompletedate").toString().isEmpty())
            {
                ui->dateEdit_AccountsCompleteDate->setDate(QDate::fromString(query.value("accountscompletedate").toString(),"yyyy-MM-dd"));
            }

            ui->lineEdit_Excution_Cash->setText(query.value("excution_cash").toString());
            ui->lineEdit_Excution_Goods->setText(query.value("excution_goods").toString());
            ui->lineEdit_ExcutionBalance->setText(query.value("excutionbalance").toString());
            ui->lineEdit_Appearinterest->setText(query.value("appearinterest").toString());
            ui->lineEdit_Interestuse->setText(query.value("interestuse").toString());
            ui->lineEdit_Interestbalance->setText(query.value("interestbalance").toString());
            ui->lineEdit_Carriedexecution->setText(query.value("carriedexecution").toString());
            ui->lineEdit_Carriedbusiness->setText(query.value("carriedbusiness").toString());
            ui->lineEdit_Recognition->setText(query.value("recognition").toString());
            ui->lineEdit_Governmentsubsidy->setText(query.value("governmentsubsidy").toString());
            ui->lineEdit_Return->setText(query.value("return").toString());
            ui->lineEdit_Acttach1->setText(query.value("Acttach1").toString());
            ui->lineEdit_Acttach2->setText(query.value("Acttach2").toString());
            ui->lineEdit_Acttach3->setText(query.value("Acttach3").toString());
            ui->lineEdit_Acttach4->setText(query.value("Acttach4").toString());
            ui->lineEdit_Acttach5->setText(query.value("Acttach5").toString());
            ui->lineEdit_Memo->setText(query.value("memo").toString());
        }
        DB.close();
    }

    catch(QException &e)
    {
        QMessageBox::warning(this,tr("Warning"),QString("%1\n%2").arg(tr("Database Error!"),e.what()),QMessageBox::Ok);
        QSqlDatabase::removeDatabase("MainDB");
    }
}

void ProjectResultInputDialog::SettingInit()
{
    Setting=new QSettings("EachOne","ProjectManagement",this);
}

bool ProjectResultInputDialog::DBSave(QString QueryStr)
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
        query.exec(QueryStr);
        DB.close();
    }

    catch(QException &e)
    {
        QMessageBox::warning(this,tr("Warning"),QString("%1\n%2").arg(tr("Database Error!"),e.what()),QMessageBox::Ok);
        QSqlDatabase::removeDatabase("MainDB");
        return false;
    }
    return true;
}

void ProjectResultInputDialog::on_pushButton_Save_clicked()
{
    if(DBSave(QueryString(UPDATE)))
    {
        QMessageBox::information(this,tr("Information"),tr("Update Complete."),QMessageBox::Ok);
    }
}

void ProjectResultInputDialog::on_pushButton_Print_clicked()
{

}
