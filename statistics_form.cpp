#include "statistics_form.h"
#include "ui_statistics_form.h"

Statistics_Form::Statistics_Form(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Statistics_Form)
{
    ui->setupUi(this);
    ui->dateEdit_SubjectNumber_FixedDate->setDate(QDate::currentDate());
    ui->dateEdit_SubjectNumber_ReceiptStartDate->setDate(QDate::currentDate());
    ui->dateEdit_SubjectNumber_ReceiptEndDate->setDate(QDate::currentDate());
    ui->dateEdit_Recognition_FixedDate->setDate(QDate::currentDate());
    ui->dateEdit_Recognition_ReceiptEndDate->setDate(QDate::currentDate());
    ui->dateEdit_Recognition_ReceiptStartDate->setDate(QDate::currentDate());

    SettingInit();
}

Statistics_Form::~Statistics_Form()
{
    delete ui;
}

void Statistics_Form::TableWidgetInit(int Select)
{
    switch (Select) {
    case TAB_SUBJECT:
        ui->tableWidget_SubjectNumber->clear();
        ui->tableWidget_SubjectNumber->setRowCount(0);
        ui->tableWidget_SubjectNumber->setColumnCount(8);
        ui->tableWidget_SubjectNumber->setHorizontalHeaderLabels(QStringList()<<tr("Managementagency")<<tr("BusinessGroup1")<<tr("TotalReceiptSubject")<<tr("Before\nCompleteSubject")<<tr("Before\nNonSubject")<<tr("After\nCompleteSubject")<<tr("After\nNonSubject")<<tr("Note"));
        break;
    default:
        ui->tableWidget_Recognition->clear();
        ui->tableWidget_Recognition->setRowCount(0);
        ui->tableWidget_Recognition->setColumnCount(7);
        ui->tableWidget_Recognition->setHorizontalHeaderLabels(QStringList()<<tr("Managementagency")<<tr("BusinessGroup1")<<tr("TotalReceiptSubjectNumber")
                                                               <<tr("AccountsCompleteSubject")<<tr("RecognitionSubjectNumber")<<tr("RecognitionMoney")<<tr("Note"));
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

    TableWidgetInit(Select);

    int TotalSubjectCount=0;
    int BeforeCompleteCount=0;
    int BeforeNonCount=0;
    int AfterCompleteCount=0;
    int AfterNonCount=0;

    try
    {
        if(!DB.isOpen())
        {
            QSqlDatabase::removeDatabase("MainDB");
            DBInit();
        }

        QSqlQuery query(DB);

        switch (Select)
        {
        case TAB_SUBJECT:
            query.exec(QueryString(TAB_SUBJECT));

            while(query.next())
            {
                ui->tableWidget_SubjectNumber->setRowCount(ui->tableWidget_SubjectNumber->rowCount()+1);
                ui->tableWidget_SubjectNumber->setItem(ui->tableWidget_SubjectNumber->rowCount()-1,0,new QTableWidgetItem(query.value(0).toString()));
                ui->tableWidget_SubjectNumber->setItem(ui->tableWidget_SubjectNumber->rowCount()-1,1,new QTableWidgetItem(query.value(1).toString()));

                for(int i=2; i<7; i++)
                {
                    if(query.value(i).toInt()<=0)
                    {
                        ui->tableWidget_SubjectNumber->setItem(ui->tableWidget_SubjectNumber->rowCount()-1,i,new QTableWidgetItem("0"));
                    }
                    else
                    {
                        ui->tableWidget_SubjectNumber->setItem(ui->tableWidget_SubjectNumber->rowCount()-1,i,new QTableWidgetItem(query.value(i).toString()));

                        switch(i)
                        {
                        case STATISTICS_ALL:
                            TotalSubjectCount+=query.value(i).toInt();
                            break;
                        case STATISTICS_BEFORECOMPLETE:
                            BeforeCompleteCount+=query.value(i).toInt();
                            break;
                        case STATISTICS_BEFORENON:
                            BeforeNonCount+=query.value(i).toInt();
                            break;
                        case STATISTICS_AFTERCOMPLETE:
                            AfterCompleteCount+=query.value(i).toInt();
                            break;
                        case STATISTICS_AFTERNON:
                            AfterNonCount+=query.value(i).toInt();
                            break;
                        }
                    }
                }
            }
            ui->tableWidget_SubjectNumber->setRowCount(ui->tableWidget_SubjectNumber->rowCount()+1);
            ui->tableWidget_SubjectNumber->setItem(ui->tableWidget_SubjectNumber->rowCount()-1,0,new QTableWidgetItem(tr("Sum")));
            ui->tableWidget_SubjectNumber->setItem(ui->tableWidget_SubjectNumber->rowCount()-1,2,new QTableWidgetItem(QString::number(TotalSubjectCount)));
            ui->tableWidget_SubjectNumber->setItem(ui->tableWidget_SubjectNumber->rowCount()-1,3,new QTableWidgetItem(QString::number(BeforeCompleteCount)));
            ui->tableWidget_SubjectNumber->setItem(ui->tableWidget_SubjectNumber->rowCount()-1,4,new QTableWidgetItem(QString::number(BeforeNonCount)));
            ui->tableWidget_SubjectNumber->setItem(ui->tableWidget_SubjectNumber->rowCount()-1,5,new QTableWidgetItem(QString::number(AfterCompleteCount)));
            ui->tableWidget_SubjectNumber->setItem(ui->tableWidget_SubjectNumber->rowCount()-1,6,new QTableWidgetItem(QString::number(AfterNonCount)));

            ui->tableWidget_SubjectNumber->resizeColumnsToContents();
            ui->tableWidget_SubjectNumber->resizeRowsToContents();
            break;

        default:
            query.exec(QueryString(Select));

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


QString Statistics_Form::QueryString(int Select)
{
    QString Query;

    switch (Select)
    {
    case TAB_SUBJECT:
        Query=QString("select managementagency, businessgroup1,count(*) totalcount,"
                      "sum(case when projectstate='%1' and accountscompletedate<='%2' then 1 end) beforecompletecount,"
                      "sum(case when (projectstate='%3' or projectstate='%4') and accountscompletedate<='%2' then 1 end) beforenoncount,"
                      "sum(case when projectstate='%1' and accountscompletedate>'%2' then 1 end) aftercompletecount,"
                      "sum(case when (projectstate='%3' or projectstate='%4') and accountscompletedate>'%2' then 1 end) afternoncount "
                      "from project_management where receiptdate between '%5' and '%6' group by managementagency, businessgroup1;")
                .arg(tr("AccountsComplete"),ui->dateEdit_SubjectNumber_FixedDate->date().toString("yyyy-MM-dd"),tr("Receipt"),tr("AccountsNotify"))
                .arg(ui->dateEdit_SubjectNumber_ReceiptStartDate->date().toString("yyyy-MM-dd"),ui->dateEdit_SubjectNumber_ReceiptEndDate->date().toString("yyyy-MM-dd"));
    case TAB_RECOGNITION:
        /* Query=QString("select managementagency,businessgroup1,count(*) as totalsubjectcount from project_management where receiptdate between"
                          "'%1' and '%2' group by managementagency,businessgroup1").arg(ui->dateEdit_Recognition_ReceiptStartDate->date().toString("yyyy-MM-dd"))
                    .arg(ui->dateEdit_Recognition_ReceiptEndDate->date().toString("yyyy-MM-dd"));*/
        break;
    }
    return Query;
}

void Statistics_Form::on_pushButton_Search_SubjectNumber_clicked()
{    
    TableWidgetInit(TAB_SUBJECT);
    DBShow(TAB_SUBJECT,QueryString(TAB_SUBJECT));
}

void Statistics_Form::on_pushButton_Search_Recognition_clicked()
{
    TableWidgetInit(TAB_RECOGNITION);
    DBShow(TAB_RECOGNITION,QueryString(TAB_RECOGNITION));
}

void Statistics_Form::on_tableWidget_SubjectNumber_doubleClicked(const QModelIndex &index)
{
    SubjectListDialog Dialog;
    connect(this,SIGNAL(setTitle(QString)),&Dialog,SLOT(SetTitle(QString)));
    connect(this,SIGNAL(DBShow(QString)),&Dialog,SLOT(DBShow(QString)));
    if(index.column()>=2 && index.column()<=6)
    {
        if(ui->tableWidget_SubjectNumber->item(index.row(),index.column())->text()=="0" &&
                ui->tableWidget_SubjectNumber->item(index.row(),index.column())->text().isEmpty())
        {
            return;
        }

        if(index.row()==ui->tableWidget_SubjectNumber->rowCount()-1)
        {
            emit setTitle(QString("%1 & %2").arg(tr("Sum"),ui->tableWidget_SubjectNumber->horizontalHeaderItem(index.column())->text()));
        }
        else
        {
            emit setTitle(QString("%1 & %2 & %3").arg(ui->tableWidget_SubjectNumber->item(index.row(),0)->text(),
                                                      ui->tableWidget_SubjectNumber->item(index.row(),1)->text(),
                                                      ui->tableWidget_SubjectNumber->horizontalHeaderItem(index.column())->text()));
        }
    }

    switch(index.column())
    {
    case STATISTICS_ALL:
        if(index.row()==ui->tableWidget_SubjectNumber->rowCount()-1)
        {
            emit DBShow(QString("select projectname from project_management where receiptdate between '%1' and '%2'")
                        .arg(ui->dateEdit_SubjectNumber_ReceiptStartDate->date().toString("yyyy-MM-dd"),ui->dateEdit_SubjectNumber_ReceiptEndDate->date().toString("yyyy-MM-dd")));
        }

        else
        {
            emit DBShow(QString("select projectname from project_management where managementagency='%1' and businessgroup1='%2' and receiptdate between '%3' and '%4'")
                        .arg(ui->tableWidget_SubjectNumber->item(index.row(),0)->text(),ui->tableWidget_SubjectNumber->item(index.row(),1)->text())
                        .arg(ui->dateEdit_SubjectNumber_ReceiptStartDate->date().toString("yyyy-MM-dd"),ui->dateEdit_SubjectNumber_ReceiptEndDate->date().toString("yyyy-MM-dd")));
        }
        Dialog.exec();
        break;
    case STATISTICS_BEFORECOMPLETE:
        if(index.row()==ui->tableWidget_SubjectNumber->rowCount()-1)
        {
            emit DBShow(QString("select projectname from project_management where receiptdate between '%1' and '%2' and accountscompletedate<='%3' and projectstate='%4'")
                        .arg(ui->dateEdit_SubjectNumber_ReceiptStartDate->date().toString("yyyy-MM-dd"),ui->dateEdit_SubjectNumber_ReceiptEndDate->date().toString("yyyy-MM-dd"))
                        .arg(ui->dateEdit_SubjectNumber_FixedDate->date().toString("yyyy-MM-dd"),tr("AccountsComplete")));
        }
        else
        {
            emit DBShow(QString("select projectname from project_management where managementagency='%1' and businessgroup1='%2' and receiptdate between '%3' and '%4'"
                                " and accountscompletedate<='%5' and projectstate='%6'")
                        .arg(ui->tableWidget_SubjectNumber->item(index.row(),0)->text(),ui->tableWidget_SubjectNumber->item(index.row(),1)->text())
                        .arg(ui->dateEdit_SubjectNumber_ReceiptStartDate->date().toString("yyyy-MM-dd"),ui->dateEdit_SubjectNumber_ReceiptEndDate->date().toString("yyyy-MM-dd"))
                        .arg(ui->dateEdit_SubjectNumber_FixedDate->date().toString("yyyy-MM-dd"),tr("AccountsComplete")));
        }
        Dialog.exec();
        break;
    case STATISTICS_BEFORENON:
        if(index.row()==ui->tableWidget_SubjectNumber->rowCount()-1)
        {
           emit DBShow(QString("select projectname from project_management where receiptdate between '%1' and '%2' and accountscompletedate between '%1' and '%5' and (projectstate='%4' or projectstate='%5')")
                        .arg(ui->dateEdit_SubjectNumber_ReceiptStartDate->date().toString("yyyy-MM-dd"),ui->dateEdit_SubjectNumber_ReceiptEndDate->date().toString("yyyy-MM-dd"))
                        .arg(ui->dateEdit_SubjectNumber_FixedDate->date().toString("yyyy-MM-dd"),tr("Receipt"),tr("AccountsNotify")));
        }
        else
        {
            emit DBShow(QString("select projectname from project_management where managementagency='%1' and businessgroup1='%2' and receiptdate between '%3' and '%4'"
                                " and accountscompletedate between '%3' and '%5' and (projectstate='%6' or projectstate='%7')")
                        .arg(ui->tableWidget_SubjectNumber->item(index.row(),0)->text(),ui->tableWidget_SubjectNumber->item(index.row(),1)->text())
                        .arg(ui->dateEdit_SubjectNumber_ReceiptStartDate->date().toString("yyyy-MM-dd"),ui->dateEdit_SubjectNumber_ReceiptEndDate->date().toString("yyyy-MM-dd"))
                        .arg(ui->dateEdit_SubjectNumber_FixedDate->date().toString("yyyy-MM-dd"),tr("Receipt"),tr("AccountsNotify")));
        }
        Dialog.exec();
        break;
    case STATISTICS_AFTERCOMPLETE:
        if(index.row()==ui->tableWidget_SubjectNumber->rowCount()-1)
        {
            emit DBShow(QString("select projectname from project_management where receiptdate between '%1' and '%2' and accountscompletedate>'%3' and projectstate='%4'")
                        .arg(ui->dateEdit_SubjectNumber_ReceiptStartDate->date().toString("yyyy-MM-dd"),ui->dateEdit_SubjectNumber_ReceiptEndDate->date().toString("yyyy-MM-dd"))
                        .arg(ui->dateEdit_SubjectNumber_FixedDate->date().toString("yyyy-MM-dd"),tr("AccountsComplete")));
        }
        else
        {
            emit DBShow(QString("select projectname from project_management where managementagency='%1' and businessgroup1='%2' and receiptdate between '%3' and '%4'"
                                " and accountscompletedate>'%5' and projectstate='%6'")
                        .arg(ui->tableWidget_SubjectNumber->item(index.row(),0)->text(),ui->tableWidget_SubjectNumber->item(index.row(),1)->text())
                        .arg(ui->dateEdit_SubjectNumber_ReceiptStartDate->date().toString("yyyy-MM-dd"),ui->dateEdit_SubjectNumber_ReceiptEndDate->date().toString("yyyy-MM-dd"))
                        .arg(ui->dateEdit_SubjectNumber_FixedDate->date().toString("yyyy-MM-dd"),tr("AccountsComplete")));
        }
        Dialog.exec();
        break;
    case STATISTICS_AFTERNON:
        if(index.row()==ui->tableWidget_SubjectNumber->rowCount()-1)
        {
           emit DBShow(QString("select projectname from project_management where receiptdate between '%1' and '%2' and (projectstate='%3' or projectstate='%4')")
                        .arg(ui->dateEdit_SubjectNumber_ReceiptStartDate->date().toString("yyyy-MM-dd"),ui->dateEdit_SubjectNumber_ReceiptEndDate->date().toString("yyyy-MM-dd"))
                        .arg(tr("Receipt"),tr("AccountsNotify")));
        }
        else
        {
            emit DBShow(QString("select projectname from project_management where managementagency='%1' and businessgroup1='%2' and receiptdate between '%3' and '%4'"
                                " and (projectstate='%5' or projectstate='%6')")
                        .arg(ui->tableWidget_SubjectNumber->item(index.row(),0)->text(),ui->tableWidget_SubjectNumber->item(index.row(),1)->text())
                        .arg(ui->dateEdit_SubjectNumber_ReceiptStartDate->date().toString("yyyy-MM-dd"),ui->dateEdit_SubjectNumber_ReceiptEndDate->date().toString("yyyy-MM-dd"))
                        .arg(tr("Receipt"),tr("AccountsNotify")));
        }
        Dialog.exec();
        break;
    }
}
