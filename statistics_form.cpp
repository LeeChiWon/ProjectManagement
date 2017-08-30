#include "statistics_form.h"
#include "ui_statistics_form.h"

Statistics_Form::Statistics_Form(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Statistics_Form)
{
    ui->setupUi(this);
    for(int i=0; i<2; i++)
    {
        ComboboxInit(i);
    }
    SettingInit();
}

Statistics_Form::~Statistics_Form()
{
    delete ui;
}

void Statistics_Form::ComboboxInit(int Select)
{
    switch (Select) {
    case TAB_SUBJECT:
        ui->comboBox_SearchContent_SubjectNumber->clear();
        ui->comboBox_SearchContent_SubjectNumber->addItems(QStringList()<<tr("BusinessType")<<tr("ManagementAgency")<<tr("BusinessGroup1")<<tr("ProjectState"));
        break;
    default:
        ui->comboBox_SearchContent_Recognition->clear();
        ui->comboBox_SearchContent_Recognition->addItems(QStringList()<<tr("BusinessType")<<tr("ManagementAgency")<<tr("BusinessGroup1"));
        break;
    }
}

void Statistics_Form::TableWidgetInit(int Select,QString ColumnName)
{
    switch (Select) {
    case TAB_SUBJECT:
        ui->tableWidget_SubjectNumber->clear();
        ui->tableWidget_SubjectNumber->setRowCount(0);
        ui->tableWidget_SubjectNumber->setHorizontalHeaderLabels(QStringList()<<ColumnName<<tr("SubjectNumber"));
        break;
    default:
        ui->tableWidget_Recognition->clear();
        ui->tableWidget_Recognition->setRowCount(0);
        ui->tableWidget_Recognition->setHorizontalHeaderLabels(QStringList()<<ColumnName<<tr("Recognition"));
        break;
    }
}

void Statistics_Form::DBInit()
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

void Statistics_Form::DBShow(int Select, QString QueryStr)
{
    QSqlDatabase DB=QSqlDatabase::database("MainDB");
    ui->tableWidget_SubjectNumber->setSortingEnabled(false);
    ui->tableWidget_Recognition->setSortingEnabled(false);
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
            switch (Select) {
            case TAB_SUBJECT:
                ui->tableWidget_SubjectNumber->setRowCount(ui->tableWidget_SubjectNumber->rowCount()+1);
                ui->tableWidget_SubjectNumber->setItem(ui->tableWidget_SubjectNumber->rowCount()-1,0,new QTableWidgetItem(query.value(0).toString()));
                ui->tableWidget_SubjectNumber->setItem(ui->tableWidget_SubjectNumber->rowCount()-1,1,new QTableWidgetItem(query.value(1).toString()));
                break;
            default:
                ui->tableWidget_Recognition->setRowCount(ui->tableWidget_Recognition->rowCount()+1);
                ui->tableWidget_Recognition->setItem(ui->tableWidget_Recognition->rowCount()-1,0,new QTableWidgetItem(query.value(0).toString()));
                ui->tableWidget_Recognition->setItem(ui->tableWidget_Recognition->rowCount()-1,1,new QTableWidgetItem(query.value(1).toString()));
                break;
            }
        }

        switch (Select) {
        case TAB_SUBJECT:
            ui->tableWidget_SubjectNumber->resizeColumnsToContents();
            ui->tableWidget_SubjectNumber->resizeRowsToContents();
            break;
        default:
            ui->tableWidget_Recognition->resizeColumnsToContents();
            ui->tableWidget_Recognition->resizeRowsToContents();
            break;
        }

        DB.close();
    }

    catch(QException &e)
    {
        QMessageBox::warning(this,tr("Warning"),QString("%1\n%2").arg(tr("Database Error!"),e.what()),QMessageBox::Ok);
        QSqlDatabase::removeDatabase("MainDB");
    }
    ui->tableWidget_SubjectNumber->setSortingEnabled(true);
    ui->tableWidget_Recognition->setSortingEnabled(true);
}

void Statistics_Form::SettingInit()
{
    Setting=new QSettings("EachOne","ProjectManagement",this);
}

QString Statistics_Form::QueryString(int Select,int Cmd)
{
    QString Query;

    switch (Cmd) {
    case STATISTICS_BUSINESSTYPE:
        switch (Select) {
        case TAB_SUBJECT:
            Query=QString("select businesstype, count(businesstype) as subjectnumber from project_management group by businesstype having count(businesstype)>=1");
            break;
        default:
            Query=QString("select businesstype, sum(recognition) as recognitionmoney from project_management group by businesstype");//having sum(recognition)>=1
            break;
        }
        break;
    case STATISTICS_MANAGEMENTAGENCY:
        switch (Select) {
        case TAB_SUBJECT:
            Query=QString("select managementagency, count(managementagency) as subjectnumber from project_management group by managementagency having count(managementagency)>=1");
            break;
        default:
            Query=QString("select managementagency, sum(recognition) as recognitionmoney from project_management group by managementagency");
            break;
        }
        break;
    case STATISTICS_BUSINESSGROUP1:
        switch (Select) {
        case TAB_SUBJECT:
            Query=QString("select businessgroup1, count(businessgroup1) as subjectnumber from project_management group by businessgroup1 having count(businessgroup1)>=1");
            break;
        default:
            Query=QString("select businessgroup1, sum(recognition) as recognitionmoney from project_management group by businessgroup1");
            break;
        }
        break;
    case STATISTICS_PROJECTSTATE:
        switch (Select) {
        case TAB_SUBJECT:
            Query=QString("select projectstate, count(projectstate) as subjectnumber from project_management group by projectstate having count(projectstate)>=1");
            break;
        default:
            break;
        }
        break;
    }
    return Query;
}

void Statistics_Form::on_pushButton_Search_SubjectNumber_clicked()
{    
    TableWidgetInit(TAB_SUBJECT,ui->comboBox_SearchContent_SubjectNumber->currentText());
    DBShow(TAB_SUBJECT,QueryString(TAB_SUBJECT,ui->comboBox_SearchContent_SubjectNumber->currentIndex()));
}

void Statistics_Form::on_pushButton_Search_Recognition_clicked()
{
    TableWidgetInit(TAB_RECOGNITION,ui->comboBox_SearchContent_Recognition->currentText());
    DBShow(TAB_RECOGNITION,QueryString(TAB_RECOGNITION,ui->comboBox_SearchContent_Recognition->currentIndex()));
}
