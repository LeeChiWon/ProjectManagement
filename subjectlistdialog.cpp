#include "subjectlistdialog.h"
#include "ui_subjectlistdialog.h"

SubjectListDialog::SubjectListDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SubjectListDialog)
{
    ui->setupUi(this);
    ContextMenuInit();
}

SubjectListDialog::~SubjectListDialog()
{
    delete ui;
}

void SubjectListDialog::SetTitle(QString Label)
{   
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

        while(query.next())
        {
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

void SubjectListDialog::ContextMenuInit()
{
    QList <QAction*> ContextAction;
    ContextAction.append(new QAction(tr("ExcelSave")));
    ContextAction.at(0)->setIcon(QIcon(":/img/excel.png"));
    ContextMenu=new QMenu();
    connect(ContextAction.at(0),SIGNAL(triggered(bool)),this,SLOT(ExcelSave(bool)));
    ContextMenu->addActions(ContextAction);
}

void SubjectListDialog::on_listWidget_customContextMenuRequested(const QPoint &pos)
{
    if(ui->listWidget->count()>0)
    {
        ContextMenu->exec(ui->listWidget->viewport()->mapToGlobal(pos));
    }
}
void SubjectListDialog::ExcelSave(bool clicked)
{
    QFileDialog FileDialog;
    QString FileName=FileDialog.getSaveFileName(this,tr("Save File"),"c://",tr("Excel (*.xlsx)"));

    if(!FileName.isEmpty() || !FileName.isNull())
    {
        QXlsx::Document xlsx(FileName);
        QStringList ColumnTitle=QStringList()<<tr("No")<<tr("SubjectName");
        int ColumnWidth[2]={7,50};
        QXlsx::Format format;

        format.setBottomBorderStyle(QXlsx::Format::BorderDouble);
        format.setFontBold(true);
        format.setFontSize(13);

        xlsx.write(1,1,ui->label_Title->text(),format);
        for(int i=1; i<=2; i++)
        {
            xlsx.setColumnWidth(i,ColumnWidth[i-1]);
            xlsx.write(2,i,ColumnTitle.at(i-1),format);
        }

        format.setBorderStyle(QXlsx::Format::BorderThin);
        format.setFontSize(10);
        format.setFontBold(false);

        for(int i=0; i<ui->listWidget->count(); i++)
        {
            xlsx.write(i+3,1,i+1,format);
            xlsx.write(i+3,2,ui->listWidget->item(i)->text(),format);
        }

        if(xlsx.save())
        {
            QMessageBox::information(this,tr("Save"),tr("Excel File is Saved."),QMessageBox::Ok);
        }

        xlsx.deleteLater();
    }
}
