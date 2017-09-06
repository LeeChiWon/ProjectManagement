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
        ui->tableWidget_SubjectNumber->setColumnCount(6);
        ui->tableWidget_SubjectNumber->setHorizontalHeaderLabels(QStringList()<<tr("Managementagency")<<tr("BusinessGroup1")<<tr("TotalReceiptSubjectNumber")
                                                                 <<tr("AccountsCompleteSubject")<<tr("NonSubjectNumber")<<tr("Note"));
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
    TotalSubject.clear();
    CompleteSubject.clear();
    int TotalSubjectCount=0;
    int CompleteSubjectCount=0;
    int NonSubjectCount=0;

    try
    {
        if(!DB.isOpen())
        {
            QSqlDatabase::removeDatabase("MainDB");
            DBInit();
        }

        QSqlQuery query(DB);
        query.exec(QueryStr);

        switch (Select)
        {
        case TAB_SUBJECT:
            Mapping(query,STATISTICS_ALLSUBJECTCOUNT);
            query.exec(QueryString(TAB_SUBJECT,STATISTICS_COMPLETESUBJECTCOUNT));
            Mapping(query,STATISTICS_COMPLETESUBJECTCOUNT);

            for(int i=0; i<TotalSubject.count(); i++)
            {
                ui->tableWidget_SubjectNumber->setRowCount(ui->tableWidget_SubjectNumber->rowCount()+1);
                ui->tableWidget_SubjectNumber->setItem(ui->tableWidget_SubjectNumber->rowCount()-1,0,new QTableWidgetItem(TotalSubject.keys().at(i).at(0)));
                ui->tableWidget_SubjectNumber->setItem(ui->tableWidget_SubjectNumber->rowCount()-1,1,new QTableWidgetItem(TotalSubject.keys().at(i).at(1)));
                ui->tableWidget_SubjectNumber->setItem(ui->tableWidget_SubjectNumber->rowCount()-1,2,new QTableWidgetItem(QString::number(TotalSubject.value(TotalSubject.keys().at(i)))));
                TotalSubjectCount+=TotalSubject.value(TotalSubject.keys().at(i));
                if(CompleteSubject.contains(TotalSubject.keys().at(i)))
                {
                    ui->tableWidget_SubjectNumber->setItem(ui->tableWidget_SubjectNumber->rowCount()-1,3,new QTableWidgetItem(QString::number(CompleteSubject.value(TotalSubject.keys().at(i)))));
                    CompleteSubjectCount+=CompleteSubject.value(TotalSubject.keys().at(i));
                }
                else
                {
                    ui->tableWidget_SubjectNumber->setItem(ui->tableWidget_SubjectNumber->rowCount()-1,3,new QTableWidgetItem(QString::number(0)));
                }
                ui->tableWidget_SubjectNumber->setItem(ui->tableWidget_SubjectNumber->rowCount()-1,4,
                                                       new QTableWidgetItem(QString::number(ui->tableWidget_SubjectNumber->item(ui->tableWidget_SubjectNumber->rowCount()-1,2)->text().toInt()-ui->tableWidget_SubjectNumber->item(ui->tableWidget_SubjectNumber->rowCount()-1,3)->text().toInt())));
                NonSubjectCount+=ui->tableWidget_SubjectNumber->item(ui->tableWidget_SubjectNumber->rowCount()-1,2)->text().toInt()-ui->tableWidget_SubjectNumber->item(ui->tableWidget_SubjectNumber->rowCount()-1,3)->text().toInt();
            }
            ui->tableWidget_SubjectNumber->setRowCount(ui->tableWidget_SubjectNumber->rowCount()+1);
            ui->tableWidget_SubjectNumber->setItem(ui->tableWidget_SubjectNumber->rowCount()-1,0,new QTableWidgetItem(tr("Sum")));
            ui->tableWidget_SubjectNumber->setItem(ui->tableWidget_SubjectNumber->rowCount()-1,2,new QTableWidgetItem(QString::number(TotalSubjectCount)));
            ui->tableWidget_SubjectNumber->setItem(ui->tableWidget_SubjectNumber->rowCount()-1,3,new QTableWidgetItem(QString::number(CompleteSubjectCount)));
            ui->tableWidget_SubjectNumber->setItem(ui->tableWidget_SubjectNumber->rowCount()-1,4,new QTableWidgetItem(QString::number(NonSubjectCount)));

            /* for(int i=0; i<6; i++)
            {
                ui->tableWidget_SubjectNumber->item(ui->tableWidget_SubjectNumber->rowCount()-1,i)->setBackgroundColor(qRgb(200,255,255));
            }*/
            break;
        default:
            Mapping(query,STATISTICS_ALLSUBJECTCOUNT);
            query.exec(QueryString(Select,STATISTICS_COMPLETESUBJECTCOUNT));
            Mapping(query,STATISTICS_COMPLETESUBJECTCOUNT);
            break;
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

void Statistics_Form::Mapping(QSqlQuery Query,int Cmd)
{
    while(Query.next())
    {
        switch(Cmd)
        {
        case STATISTICS_ALLSUBJECTCOUNT:
            TotalSubject.insert(QStringList()<<Query.value("managementagency").toString()<<Query.value("businessgroup1").toString(),Query.value(2).toInt());
            break;
        case STATISTICS_COMPLETESUBJECTCOUNT:
            CompleteSubject.insert(QStringList()<<Query.value("managementagency").toString()<<Query.value("businessgroup1").toString(),Query.value(2).toInt());
            break;
        }
    }
}

QString Statistics_Form::QueryString(int Select,int Cmd)
{
    QString Query;

    switch (Cmd) {
    case STATISTICS_ALLSUBJECTCOUNT:
        switch (Select) {
        case TAB_SUBJECT:
            Query=QString("select managementagency,businessgroup1,count(*) as totalsubjectcount from project_management where receiptdate between"
                          "'%1' and '%2' group by managementagency,businessgroup1").arg(ui->dateEdit_SubjectNumber_ReceiptStartDate->date().toString("yyyy-MM-dd"))
                    .arg(ui->dateEdit_SubjectNumber_ReceiptEndDate->date().toString("yyyy-MM-dd"));
        case TAB_RECOGNITION:
            /* Query=QString("select managementagency,businessgroup1,count(*) as totalsubjectcount from project_management where receiptdate between"
                          "'%1' and '%2' group by managementagency,businessgroup1").arg(ui->dateEdit_Recognition_ReceiptStartDate->date().toString("yyyy-MM-dd"))
                    .arg(ui->dateEdit_Recognition_ReceiptEndDate->date().toString("yyyy-MM-dd"));*/
            break;
        }
        break;
    case STATISTICS_COMPLETESUBJECTCOUNT:
        switch (Select) {
        case TAB_SUBJECT:
            Query=QString("select managementagency,businessgroup1,count(*) as completesubjectcount from project_management where receiptdate between"
                          "'%1' and '%2' and accountscompletedate<='%3' and projectstate='%4' group by managementagency,businessgroup1")
                    .arg(ui->dateEdit_SubjectNumber_ReceiptStartDate->date().toString("yyyy-MM-dd"),ui->dateEdit_SubjectNumber_ReceiptEndDate->date().toString("yyyy-MM-dd"))
                    .arg(ui->dateEdit_SubjectNumber_FixedDate->date().toString("yyyy-MM-dd"),tr("AccountsComplete"));

            break;
        default:

            Query=QString("select managementagency, sum(recognition) as recognitionmoney from project_management group by managementagency");
            break;
        }
        break;
    }
    return Query;
}

void Statistics_Form::on_pushButton_Search_SubjectNumber_clicked()
{    
    TableWidgetInit(TAB_SUBJECT);
    DBShow(TAB_SUBJECT,QueryString(TAB_SUBJECT,STATISTICS_ALLSUBJECTCOUNT));
}

void Statistics_Form::on_pushButton_Search_Recognition_clicked()
{
    TableWidgetInit(TAB_RECOGNITION);
    DBShow(TAB_RECOGNITION,QueryString(TAB_RECOGNITION,STATISTICS_ALLSUBJECTCOUNT));
}

void Statistics_Form::on_tableWidget_SubjectNumber_doubleClicked(const QModelIndex &index)
{
    SubjectListDialog Dialog;
    connect(this,SIGNAL(setTitle(QString)),&Dialog,SLOT(SetTitle(QString)));
    connect(this,SIGNAL(DBShow(QString)),&Dialog,SLOT(DBShow(QString)));

    switch(index.column())
    {
    case COLUMN_TOTALSUBJECTCOUNT:
        if(ui->tableWidget_SubjectNumber->item(index.row(),COLUMN_TOTALSUBJECTCOUNT)->text()=="0")
        {
            return;
        }
        emit setTitle(QString("%1 & %2 & %3").arg(ui->tableWidget_SubjectNumber->item(index.row(),COLUMN_MANAGEMENTAGENCY)->text(),
                                                  ui->tableWidget_SubjectNumber->item(index.row(),COLUMN_BUSINESSGROUP1)->text(),
                                                  ui->tableWidget_SubjectNumber->horizontalHeaderItem(COLUMN_TOTALSUBJECTCOUNT)->text()));

        if(index.row()==ui->tableWidget_SubjectNumber->rowCount()-1)
        {
            emit DBShow(QString("select projectname from project_management where receiptdate between '%1' and '%2'")
                        .arg(ui->dateEdit_SubjectNumber_ReceiptStartDate->date().toString("yyyy-MM-dd"),ui->dateEdit_SubjectNumber_ReceiptEndDate->date().toString("yyyy-MM-dd")));
        }

        else
        {
            emit DBShow(QString("select projectname from project_management where managementagency='%1' and businessgroup1='%2' and receiptdate between '%3' and '%4'")
                        .arg(ui->tableWidget_SubjectNumber->item(index.row(),COLUMN_MANAGEMENTAGENCY)->text(),ui->tableWidget_SubjectNumber->item(index.row(),COLUMN_BUSINESSGROUP1)->text())
                        .arg(ui->dateEdit_SubjectNumber_ReceiptStartDate->date().toString("yyyy-MM-dd"),ui->dateEdit_SubjectNumber_ReceiptEndDate->date().toString("yyyy-MM-dd")));
        }

        Dialog.exec();
        break;
    case COLUMN_COMPLETESUBJECTCOUNT:
        if(ui->tableWidget_SubjectNumber->item(index.row(),COLUMN_COMPLETESUBJECTCOUNT)->text()=="0")
        {
            return;
        }
        emit setTitle(QString("%1 & %2 & %3").arg(ui->tableWidget_SubjectNumber->item(index.row(),COLUMN_MANAGEMENTAGENCY)->text(),
                                                  ui->tableWidget_SubjectNumber->item(index.row(),COLUMN_BUSINESSGROUP1)->text(),
                                                  ui->tableWidget_SubjectNumber->horizontalHeaderItem(COLUMN_COMPLETESUBJECTCOUNT)->text()));
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
                        .arg(ui->tableWidget_SubjectNumber->item(index.row(),COLUMN_MANAGEMENTAGENCY)->text(),ui->tableWidget_SubjectNumber->item(index.row(),COLUMN_BUSINESSGROUP1)->text())
                        .arg(ui->dateEdit_SubjectNumber_ReceiptStartDate->date().toString("yyyy-MM-dd"),ui->dateEdit_SubjectNumber_ReceiptEndDate->date().toString("yyyy-MM-dd"))
                        .arg(ui->dateEdit_SubjectNumber_FixedDate->date().toString("yyyy-MM-dd"),tr("AccountsComplete")));
        }
        Dialog.exec();
        break;
    case COLUMN_NONSUBJECTCOUNT:
        if(ui->tableWidget_SubjectNumber->item(index.row(),COLUMN_NONSUBJECTCOUNT)->text()=="0")
        {
            return;
        }
        emit setTitle(QString("%1 & %2 & %3").arg(ui->tableWidget_SubjectNumber->item(index.row(),COLUMN_MANAGEMENTAGENCY)->text(),
                                                  ui->tableWidget_SubjectNumber->item(index.row(),COLUMN_BUSINESSGROUP1)->text(),
                                                  ui->tableWidget_SubjectNumber->horizontalHeaderItem(COLUMN_NONSUBJECTCOUNT)->text()));
        if(index.row()==ui->tableWidget_SubjectNumber->rowCount()-1)
        {
            emit DBShow(QString("select projectname from project_management where receiptdate between '%1' and '%2' and accountscompletedate<='%3' and projectstate not like '%4'")
                        .arg(ui->dateEdit_SubjectNumber_ReceiptStartDate->date().toString("yyyy-MM-dd"),ui->dateEdit_SubjectNumber_ReceiptEndDate->date().toString("yyyy-MM-dd"))
                        .arg(ui->dateEdit_SubjectNumber_FixedDate->date().toString("yyyy-MM-dd"),tr("AccountsComplete")));
        }
        else
        {
            emit DBShow(QString("select projectname from project_management where managementagency='%1' and businessgroup1='%2' and receiptdate between '%3' and '%4'"
                                " and accountscompletedate<='%5' and projectstate not like '%6'")
                        .arg(ui->tableWidget_SubjectNumber->item(index.row(),COLUMN_MANAGEMENTAGENCY)->text(),ui->tableWidget_SubjectNumber->item(index.row(),COLUMN_BUSINESSGROUP1)->text())
                        .arg(ui->dateEdit_SubjectNumber_ReceiptStartDate->date().toString("yyyy-MM-dd"),ui->dateEdit_SubjectNumber_ReceiptEndDate->date().toString("yyyy-MM-dd"))
                        .arg(ui->dateEdit_SubjectNumber_FixedDate->date().toString("yyyy-MM-dd"),tr("AccountsComplete")));
        }
        Dialog.exec();
        break;
    }
}
