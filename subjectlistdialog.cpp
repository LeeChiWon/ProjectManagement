#include "subjectlistdialog.h"
#include "ui_subjectlistdialog.h"

SubjectListDialog::SubjectListDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SubjectListDialog)
{
    ui->setupUi(this);

}

SubjectListDialog::~SubjectListDialog()
{
    delete ui;
}

void SubjectListDialog::SetTitle(QString Label)
{
    qDebug()<<Label;
    ui->label_Title->setText(Label);
}

void SubjectListDialog::DBShow(QString QueryStr)
{
    QSqlDatabase DB=QSqlDatabase::database("MainDB");
    ui->listWidget->clear();

    try
    {
        if(!DB.isOpen())
        {
            QSqlDatabase::removeDatabase("MainDB");
            DBInit();
        }

        QSqlQuery query(DB);
        query.exec(QueryStr);
        qDebug()<<query.lastQuery()<<query.lastError()<<query.size();


        while(query.next())
        {
            qDebug()<<query.value(0).toString();
            ui->listWidget->addItem(query.value("projectname").toString());
            if(ui->listWidget->count()%2==0)
            {
                ui->listWidget->item(ui->listWidget->count()-1)->setBackgroundColor(qRgb(200,255,255));
            }
        }

        DB.close();
    }

    catch(QException &e)
    {
        QMessageBox::warning(this,tr("Warning"),QString("%1\n%2").arg(tr("Database Error!"),e.what()),QMessageBox::Ok);
        QSqlDatabase::removeDatabase("MainDB");
    }
}

void SubjectListDialog::DBInit()
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

void SubjectListDialog::SettingInit()
{
    Setting=new QSettings("EachOne","ProjectManagement",this);
}
