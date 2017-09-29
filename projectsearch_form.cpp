#include "projectsearch_form.h"
#include "ui_projectsearch_form.h"

ProjectSearch_Form::ProjectSearch_Form(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ProjectSearch_Form)
{
    ui->setupUi(this);

    ui->dateEdit_AccountsCompleteDate->setDate(QDate::fromString("1900-01-01","yyyy-MM-dd"));
    ui->dateEdit_AccountsCompleteDate_2->setDate(QDate::fromString("1900-01-01","yyyy-MM-dd"));
    ui->dateEdit_AgreementStart->setDate(QDate::fromString("1900-01-01","yyyy-MM-dd"));
    ui->dateEdit_AgreementStart_2->setDate(QDate::fromString("1900-01-01","yyyy-MM-dd"));
    ui->dateEdit_ReceiptDate->setDate(QDate::fromString("1900-01-01","yyyy-MM-dd"));
    ui->dateEdit_ReceiptDate_2->setDate(QDate::fromString("1900-01-01","yyyy-MM-dd"));

    SettingInit();
    TableWidgetInit();
    Search(QueryString(INIT));
}

ProjectSearch_Form::~ProjectSearch_Form()
{
    delete ui;
}

void ProjectSearch_Form::on_pushButton_Search_clicked()
{
    EnabledItem(false);
    Search(QueryString(SEARCH));
    EnabledItem(true);
}

void ProjectSearch_Form::on_pushButton_Delete_clicked()
{
    EnabledItem(false);
    Delete(QueryString(DEL));
    on_pushButton_Search_clicked();
    EnabledItem(true);
}

void ProjectSearch_Form::on_pushButton_Print_clicked()
{
    QPrintPreviewDialog dialog;
    connect(&dialog, SIGNAL(paintRequested(QPrinter*)), this, SLOT(TableWidgetPrint(QPrinter*)));
    dialog.exec();
}

void ProjectSearch_Form::TableWidgetPrint(QPrinter *printer)
{
    QPainter painter;
    if(!painter.begin(printer)) {
        QMessageBox::warning(this,tr("Warning"),tr("can't start printer"),QMessageBox::Ok);
        return;
    }
    // print table
    TablePrinter tablePrinter(&painter, printer);
    QVector<int> columnStretch = QVector<int>() << 5 << 5 << 5 << 5 << 5<< 5 <<5<<2<<8;
    if(!tablePrinter.printTable(ui->tableWidget->model(), columnStretch)) {
        QMessageBox::warning(this,tr("Error"),tablePrinter.lastError(),QMessageBox::Ok);
    }
    painter.end();
}

void ProjectSearch_Form::on_pushButton_ExcelSave_clicked()
{
    EnabledItem(false);
    if(ui->tableWidget->rowCount()>0)
    {
        ExcelSave();
    }
    EnabledItem(true);
}

void ProjectSearch_Form::ExcelSave()
{
    QFileDialog FileDialog;
    QString FileName=FileDialog.getSaveFileName(this,tr("Save File"),"c://",tr("Excel (*.xlsx)"));

    if(!FileName.isEmpty() || !FileName.isNull())
    {
        QXlsx::Document xlsx(FileName);
        QStringList ColumnTitle=QStringList()<<tr("BusinessType")<<tr("ProjectNumber")<<tr("Managementagency")<<tr("BusinessGroup1")
                                            <<tr("BusinessGroup2")<<tr("BusinessGroup3")<<tr("Organization")<<tr("Manager")<<tr("ProjectName");
        int ColumnWidth[9]={17,25,25,35,35,35,40,10,70};
        QXlsx::Format format;

        format.setBorderStyle(QXlsx::Format::BorderThin);
        format.setFontSize(10);
        format.setFontBold(false);

        for(int i=0; i<ui->tableWidget->rowCount(); i++)
        {
            for(int j=0; j<9; j++)
            {
                xlsx.write(i+2,j+1,ui->tableWidget->item(i,j)->text(),format);
            }
        }

        format.setBottomBorderStyle(QXlsx::Format::BorderDouble);
        format.setFontBold(true);
        format.setFontSize(13);

        for(int i=1; i<10; i++)
        {
            xlsx.setColumnWidth(i,ColumnWidth[i-1]);
            xlsx.write(1,i,ColumnTitle.at(i-1),format);
        }

        if(xlsx.save())
        {
            QMessageBox::information(this,tr("Save"),tr("Excel File is Saved."),QMessageBox::Ok);
        }

        xlsx.deleteLater();
    }
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
        DB.transaction();
        QSqlQuery query(DB);

        query.exec(QueryStr);

        while(query.next())
        {
            ui->tableWidget->setRowCount(ui->tableWidget->rowCount()+1);
            ui->tableWidget->setItem(ui->tableWidget->rowCount()-1,BUSINESSTYPE,new QTableWidgetItem(query.value("businesstype").toString()));
            ui->tableWidget->setItem(ui->tableWidget->rowCount()-1,PROJECTNUMBER,new QTableWidgetItem(query.value("projectnumber").toString()));
            ui->tableWidget->setItem(ui->tableWidget->rowCount()-1,MANAGEMENTAGENCY,new QTableWidgetItem(query.value("managementagency").toString()));
            ui->tableWidget->setItem(ui->tableWidget->rowCount()-1,BUSINESSGROUP1,new QTableWidgetItem(query.value("businessgroup1").toString()));
            ui->tableWidget->setItem(ui->tableWidget->rowCount()-1,BUSINESSGROUP2,new QTableWidgetItem(query.value("businessgroup2").toString()));
            ui->tableWidget->setItem(ui->tableWidget->rowCount()-1,BUSINESSGROUP3,new QTableWidgetItem(query.value("businessgroup3").toString()));
            ui->tableWidget->setItem(ui->tableWidget->rowCount()-1,ORGANIZATION,new QTableWidgetItem(query.value("organization").toString()));
            ui->tableWidget->setItem(ui->tableWidget->rowCount()-1,MANAGER,new QTableWidgetItem(query.value("manager").toString()));
            ui->tableWidget->setItem(ui->tableWidget->rowCount()-1,PROJECTNAME,new QTableWidgetItem(query.value("projectname").toString()));
            /*if(ui->tableWidget->rowCount()%2==0)
            {
                for(int i=0; i<ui->tableWidget->columnCount(); i++)
                {
                    ui->tableWidget->item(ui->tableWidget->rowCount()-1,i)->setBackgroundColor(qRgb(255,255,127));
                }
            }*/
        }

        ui->tableWidget->resizeColumnsToContents();
        ui->tableWidget->resizeRowsToContents();
        DB.commit();
        DB.close();
    }

    catch(QException &e)
    {
        QMessageBox::warning(this,tr("Warning"),QString("%1\n%2").arg(tr("Database Error!"),e.what()),QMessageBox::Ok);
        QSqlDatabase::removeDatabase("MainDB");
    }
    ui->tableWidget->setSortingEnabled(true);
}

void ProjectSearch_Form::Delete(QString QueryStr)
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
        DB.transaction();
        QSqlQuery query(DB);

        query.exec(QueryStr);

        ui->tableWidget->resizeColumnsToContents();
        ui->tableWidget->resizeRowsToContents();
        DB.commit();
        DB.close();
    }

    catch(QException &e)
    {
        QMessageBox::warning(this,tr("Warning"),QString("%1\n%2").arg(tr("Database Error!"),e.what()),QMessageBox::Ok);
        QSqlDatabase::removeDatabase("MainDB");
    }
    ui->pushButton_Delete->setEnabled(false);
    ui->tableWidget->setSortingEnabled(true);
}

void ProjectSearch_Form::TableWidgetInit()
{
    ui->tableWidget->setColumnCount(9);
    ui->tableWidget->setHorizontalHeaderLabels(QStringList()<<tr("BusinessType")<<tr("ProjectNumber")<<tr("Managementagency")<<tr("BusinessGroup1")<<tr("BusinessGroup2")<<tr("BusinessGroup3")
                                               <<tr("Organization")<<tr("Manager")<<tr("ProjectName"));
}

void ProjectSearch_Form::EnabledItem(bool bCheck)
{
    ui->pushButton_Delete->setEnabled(bCheck);
    ui->pushButton_ExcelSave->setEnabled(bCheck);
    ui->pushButton_Print->setEnabled(bCheck);
    ui->pushButton_Search->setEnabled(bCheck);
}

QString ProjectSearch_Form::QueryString(int Cmd)
{
    QString Query;
    bool Check=false;
    switch (Cmd) {
    case INIT:
        Query=QString("select * from project_management order by projectnumber desc limit 100");
        break;
    case SEARCH:
        Query=QString("select * from project_management where ");
        if(!ui->lineEdit_BusinessType->text().isEmpty())
        {
            Query.append(QString("businesstype like '\%%1\%'").arg(ui->lineEdit_BusinessType->text()));
            Check=true;
        }
        if(!ui->lineEdit_ProjectNumber->text().isEmpty())
        {
            if(Check)
            {
                Query.append("and ");
            }
            Query.append(QString("projectnumber like '\%%1\%'").arg(ui->lineEdit_ProjectNumber->text()));
            Check=true;
        }
        if(!ui->lineEdit_ManagementAgency->text().isEmpty())
        {
            if(Check)
            {
                Query.append("and ");
            }
            Query.append(QString("managementagency like '\%%1\%'").arg(ui->lineEdit_ManagementAgency->text()));
            Check=true;
        }
        if(!ui->lineEdit_Organization->text().isEmpty())
        {
            if(Check)
            {
                Query.append("and ");
            }
            Query.append(QString("organization like '\%%1\%'").arg(ui->lineEdit_Organization->text()));
            Check=true;
        }
        if(!ui->lineEdit_Manager->text().isEmpty())
        {
            if(Check)
            {
                Query.append("and ");
            }
            Query.append(QString("manager like '\%%1\%'").arg(ui->lineEdit_Manager->text()));
            Check=true;
        }
        if(!ui->lineEdit_ProjectName->text().isEmpty())
        {
            if(Check)
            {
                Query.append("and ");
            }
            Query.append(QString("projectname like '\%%1\%'").arg(ui->lineEdit_ProjectName->text()));
            Check=true;
        }

        if(ui->dateEdit_AgreementStart_2->text()!="1900-01-01")
        {
            if(Check)
            {
                Query.append("and ");
            }
            Query.append(QString("agreementstartdate between '%1' and '%2'").arg(ui->dateEdit_AgreementStart->text(),ui->dateEdit_AgreementStart_2->text()));
            Check=true;
        }

        if(ui->dateEdit_ReceiptDate_2->text()!="1900-01-01")
        {
            if(Check)
            {
                Query.append("and ");
            }
            Query.append(QString("receiptdate between '%1' and '%2'").arg(ui->dateEdit_ReceiptDate->text(),ui->dateEdit_ReceiptDate_2->text()));
            Check=true;
        }

        if(ui->dateEdit_AccountsCompleteDate_2->text()!="1900-01-01")
        {
            if(Check)
            {
                Query.append("and ");
            }
            Query.append(QString("accountscompletedate between '%1' and '%2'").arg(ui->dateEdit_AccountsCompleteDate->text(),ui->dateEdit_AccountsCompleteDate_2->text()));
            Check=true;
        }

        if(!Check)
        {
            Query.replace("where ","");
        }
        break;
    case DEL:
        Query.append(QString("delete from project_management where projectname='%1'").arg(ui->tableWidget->item(ui->tableWidget->currentRow(),PROJECTNAME)->text()));
        break;
    }
    return Query;
}

void ProjectSearch_Form::on_tableWidget_itemClicked(QTableWidgetItem *item)
{
    if(g_LoginLevel==LOGIN_MANAGER || g_LoginLevel==LOGIN_MASTER)
    {
        ui->pushButton_Delete->setEnabled(true);
    }
}

void ProjectSearch_Form::on_tableWidget_itemDoubleClicked(QTableWidgetItem *item)
{
    ProjectResultInputDialog dialog;
    dialog.InitString(QStringList()<<ui->tableWidget->item(item->row(),MANAGEMENTAGENCY)->text()<<ui->tableWidget->item(item->row(),PROJECTNAME)->text()
                                    <<ui->tableWidget->item(item->row(),ORGANIZATION)->text()<<ui->tableWidget->item(item->row(),PROJECTNUMBER)->text());
    dialog.exec();
}
