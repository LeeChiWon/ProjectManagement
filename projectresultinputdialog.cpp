#include "projectresultinputdialog.h"
#include "ui_projectresultinputdialog.h"

ProjectResultInputDialog::ProjectResultInputDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ProjectResultInputDialog)
{    
    ui->setupUi(this);
    SettingInit();
    ComboboxInit();
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
    DBShow(QueryString(INIT));
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

        Query.append(QString(",execution_cash='%1',execution_goods='%2',executionbalance='%3',appearinterest='%4',interestuse='%5',interestbalance='%6'"
                             ",carriedexecution='%7',carriedbusiness='%8',recognition='%9',governmentsubsidy='%10',return='%11',acttach1='%12'"
                             ",acttach2='%13',acttach3='%14',acttach4='%15',acttach5='%16',accountperson='%17',memo='%18' ").arg(ui->lineEdit_Excution_Cash->text(),ui->lineEdit_Excution_Goods->text())
                     .arg(ui->lineEdit_ExcutionBalance->text(),ui->lineEdit_Appearinterest->text(),ui->lineEdit_Interestuse->text(),ui->lineEdit_Interestbalance->text())
                     .arg(ui->lineEdit_Carriedexecution->text(),ui->lineEdit_Carriedbusiness->text(),ui->lineEdit_Recognition->text(),ui->lineEdit_Governmentsubsidy->text())
                     .arg(ui->lineEdit_Return->text(),ui->lineEdit_Acttach1->text(),ui->lineEdit_Acttach2->text(),ui->lineEdit_Acttach3->text(),ui->lineEdit_Acttach4->text()
                          ,ui->lineEdit_Acttach5->text(),ui->lineEdit_AccountPerson->text(),ui->lineEdit_Memo->text()));

        Query.append(QString("where projectname='%1'").arg(ui->lineEdit_ProjectName->text()));
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

            if(!query.value("accountperson").toString().isEmpty())
            {
                ui->lineEdit_AccountPerson->setText(query.value("accountperson").toString());
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

            ui->lineEdit_Excution_Cash->setText(query.value("execution_cash").toString());
            ui->lineEdit_Excution_Goods->setText(query.value("execution_goods").toString());
            ui->lineEdit_ExcutionBalance->setText(query.value("executionbalance").toString());
            ui->lineEdit_Appearinterest->setText(query.value("appearinterest").toString());
            ui->lineEdit_Interestuse->setText(query.value("interestuse").toString());
            ui->lineEdit_Interestbalance->setText(query.value("interestbalance").toString());
            ui->lineEdit_Carriedexecution->setText(query.value("carriedexecution").toString());
            ui->lineEdit_Carriedbusiness->setText(query.value("carriedbusiness").toString());
            ui->lineEdit_Recognition->setText(query.value("recognition").toString());
            ui->lineEdit_Governmentsubsidy->setText(query.value("governmentsubsidy").toString());
            ui->lineEdit_Return->setText(query.value("return").toString());
            ui->lineEdit_Acttach1->setText(query.value("acttach1").toString());
            ui->lineEdit_Acttach2->setText(query.value("acttach2").toString());
            ui->lineEdit_Acttach3->setText(query.value("acttach3").toString());
            ui->lineEdit_Acttach4->setText(query.value("acttach4").toString());
            ui->lineEdit_Acttach5->setText(query.value("acttach5").toString());
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
    QPrintPreviewDialog dialog;
    connect(&dialog, SIGNAL(paintRequested(QPrinter*)), this, SLOT(ViewPrint(QPrinter*)));
    dialog.exec();
}

void ProjectResultInputDialog::on_lineEdit_Excution_Cash_editingFinished()
{
    ui->lineEdit_Excution_Cash->setText(QString("%L1").arg(ui->lineEdit_Excution_Cash->text().replace(",","").toInt()));
}

void ProjectResultInputDialog::on_lineEdit_Excution_Goods_editingFinished()
{
    ui->lineEdit_Excution_Goods->setText(QString("%L1").arg(ui->lineEdit_Excution_Goods->text().replace(",","").toInt()));
}

void ProjectResultInputDialog::on_lineEdit_ExcutionBalance_editingFinished()
{
    ui->lineEdit_ExcutionBalance->setText(QString("%L1").arg(ui->lineEdit_ExcutionBalance->text().replace(",","").toInt()));
}

void ProjectResultInputDialog::on_lineEdit_Appearinterest_editingFinished()
{
    ui->lineEdit_Appearinterest->setText(QString("%L1").arg(ui->lineEdit_Appearinterest->text().replace(",","").toInt()));
}

void ProjectResultInputDialog::on_lineEdit_Interestuse_editingFinished()
{
    ui->lineEdit_Interestuse->setText(QString("%L1").arg(ui->lineEdit_Interestuse->text().replace(",","").toInt()));
}

void ProjectResultInputDialog::on_lineEdit_Interestbalance_editingFinished()
{
    ui->lineEdit_Interestbalance->setText(QString("%L1").arg(ui->lineEdit_Interestbalance->text().replace(",","").toInt()));
}

void ProjectResultInputDialog::on_lineEdit_Carriedexecution_editingFinished()
{
    ui->lineEdit_Carriedexecution->setText(QString("%L1").arg(ui->lineEdit_Carriedexecution->text().replace(",","").toInt()));
}

void ProjectResultInputDialog::on_lineEdit_Carriedbusiness_editingFinished()
{
    ui->lineEdit_Carriedbusiness->setText(QString("%L1").arg(ui->lineEdit_Carriedbusiness->text().replace(",","").toInt()));
}

void ProjectResultInputDialog::on_lineEdit_Recognition_editingFinished()
{
    ui->lineEdit_Recognition->setText(QString("%L1").arg(ui->lineEdit_Recognition->text().replace(",","").toInt()));
}

void ProjectResultInputDialog::on_lineEdit_Return_editingFinished()
{
    ui->lineEdit_Return->setText(QString("%L1").arg(ui->lineEdit_Return->text().replace(",","").toInt()));
}

void ProjectResultInputDialog::ViewPrint(QPrinter *printer)
{
    QPainter painter;
    painter.begin(printer);
    double xscale = printer->pageRect().width()/double(this->width());
    double yscale = printer->pageRect().height()/double(this->height());
    double scale = qMin(xscale, yscale);
    painter.translate(printer->paperRect().x() + printer->pageRect().width()/2,
                      printer->paperRect().y() + printer->pageRect().height()/2);
    painter.scale(scale, scale);
    painter.translate(-width()/2, -height()/2);

    this->render(&painter);
}
