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
    ProjectNumber=List.at(3);
    DBShow(QueryString(INIT));
}

void ProjectResultInputDialog::mouseDoubleClickEvent(QMouseEvent *event)
{
    if((event->pos().x()>= ui->label_Acttach1->geometry().x()) && (event->pos().y()>= ui->label_Acttach1->geometry().y()) &&
            (event->pos().x()<= ui->label_Acttach1->geometry().bottomRight().x()) && (event->pos().y()<= ui->label_Acttach1->geometry().bottomRight().y()))
    {
       QDesktopServices::openUrl(QUrl("file:///"+Acttach[0]));
    }
    if((event->pos().x()>= ui->label_Acttach2->geometry().x()) && (event->pos().y()>= ui->label_Acttach2->geometry().y()) &&
            (event->pos().x()<= ui->label_Acttach2->geometry().bottomRight().x()) && (event->pos().y()<= ui->label_Acttach2->geometry().bottomRight().y()))
    {
       QDesktopServices::openUrl(QUrl("file:///"+Acttach[1]));
    }
    if((event->pos().x()>= ui->label_Acttach3->geometry().x()) && (event->pos().y()>= ui->label_Acttach3->geometry().y()) &&
            (event->pos().x()<= ui->label_Acttach3->geometry().bottomRight().x()) && (event->pos().y()<= ui->label_Acttach3->geometry().bottomRight().y()))
    {
       QDesktopServices::openUrl(QUrl("file:///"+Acttach[2]));
    }if((event->pos().x()>= ui->label_Acttach4->geometry().x()) && (event->pos().y()>= ui->label_Acttach4->geometry().y()) &&
            (event->pos().x()<= ui->label_Acttach4->geometry().bottomRight().x()) && (event->pos().y()<= ui->label_Acttach4->geometry().bottomRight().y()))
    {
       QDesktopServices::openUrl(QUrl("file:///"+Acttach[3]));
    }
    if((event->pos().x()>= ui->label_Acttach5->geometry().x()) && (event->pos().y()>= ui->label_Acttach5->geometry().y()) &&
            (event->pos().x()<= ui->label_Acttach5->geometry().bottomRight().x()) && (event->pos().y()<= ui->label_Acttach5->geometry().bottomRight().y()))
    {
       QDesktopServices::openUrl(QUrl("file:///"+Acttach[4]));
    }
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
                             ",carriedexecution='%7',carriedbusiness='%8',recognition='%9',governmentsubsidy='%10',return='%11'").arg(ui->lineEdit_Excution_Cash->text(),ui->lineEdit_Excution_Goods->text())
                     .arg(ui->lineEdit_ExcutionBalance->text(),ui->lineEdit_Appearinterest->text(),ui->lineEdit_Interestuse->text(),ui->lineEdit_Interestbalance->text())
                     .arg(ui->lineEdit_Carriedexecution->text(),ui->lineEdit_Carriedbusiness->text(),ui->lineEdit_Recognition->text(),ui->lineEdit_Governmentsubsidy->text())
                     .arg(ui->lineEdit_Return->text()));

        QString DBPath=Setting->value("Configuration/DBPath").toString().replace("/ProjectManagement.db","");
        for(int i=0; i<5; i++)
        {
            if(Acttach[i]!=InputActtach[i])
            {
                if(ActtachPath.isEmpty())
                {
                    ActtachPath=QString("%1/%2").arg(DBPath,ProjectNumber);
                }
                QDir Dir(ActtachPath);

                if(!Dir.exists())
                {
                    Dir.mkdir(ActtachPath);
                }

                switch(i)
                {
                case ACTTACH1:
                    Query.append(QString(",acttach1='%1/%2'").arg(ActtachPath,ui->label_Acttach1->text()));
                    QFile::copy(InputActtach[i],QString("%1/%2").arg(ActtachPath,ui->label_Acttach1->text()));
                    break;
                case ACTTACH2:
                    Query.append(QString(",acttach2='%1/%2'").arg(ActtachPath,ui->label_Acttach2->text()));
                    QFile::copy(InputActtach[i],QString("%1/%2").arg(ActtachPath,ui->label_Acttach2->text()));
                    break;
                case ACTTACH3:
                    Query.append(QString(",acttach3='%1/%2'").arg(ActtachPath,ui->label_Acttach3->text()));
                    QFile::copy(InputActtach[i],QString("%1/%2").arg(ActtachPath,ui->label_Acttach3->text()));
                    break;
                case ACTTACH4:
                    Query.append(QString(",acttach4='%1/%2'").arg(ActtachPath,ui->label_Acttach4->text()));
                    QFile::copy(InputActtach[i],QString("%1/%2").arg(ActtachPath,ui->label_Acttach4->text()));
                    break;
                case ACTTACH5:
                    Query.append(QString(",acttach5='%1/%2'").arg(ActtachPath,ui->label_Acttach5->text()));
                    QFile::copy(InputActtach[i],QString("%1/%2").arg(ActtachPath,ui->label_Acttach5->text()));
                    break;
                }
            }
        }
        if(!ActtachPath.isEmpty())
        {
            Query.append(QString(",acttachpath='%1'").arg(ActtachPath));
        }
        Query.append(QString(",accountperson='%1',memo='%2' where projectname='%3'").arg(ui->lineEdit_AccountPerson->text(),ui->lineEdit_Memo->text(),ui->lineEdit_ProjectName->text()));
        break;

    }
    return Query;
}

void ProjectResultInputDialog::DBShow(QString QueryStr)
{
    QSqlDatabase DB=QSqlDatabase::database("MainDB");
    QStringList List;
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
            if(!query.value("acttach1").toString().isEmpty())
            {
                List=query.value("acttach1").toString().split("/",QString::KeepEmptyParts);
                ui->label_Acttach1->setText(List.at(List.count()-1));
                InputActtach[0]=Acttach[0]=query.value("acttach1").toString();
            }
            if(!query.value("acttach2").toString().isEmpty())
            {
                List.clear();
                List=query.value("acttach2").toString().split("/",QString::KeepEmptyParts);
                ui->label_Acttach2->setText(List.at(List.count()-1));
                InputActtach[1]=Acttach[1]=query.value("acttach2").toString();
            }
            if(!query.value("acttach3").toString().isEmpty())
            {
                List.clear();
                List=query.value("acttach3").toString().split("/",QString::KeepEmptyParts);
                ui->label_Acttach3->setText(List.at(List.count()-1));
                InputActtach[2]=Acttach[2]=query.value("acttach3").toString();
            }
            if(!query.value("acttach4").toString().isEmpty())
            {
                List.clear();
                List=query.value("acttach4").toString().split("/",QString::KeepEmptyParts);
                ui->label_Acttach4->setText(List.at(List.count()-1));
                InputActtach[3]=Acttach[3]=query.value("acttach4").toString();
            }
            if(!query.value("acttach5").toString().isEmpty())
            {
                List.clear();
                List=query.value("acttach5").toString().split("/",QString::KeepEmptyParts);
                ui->label_Acttach5->setText(List.at(List.count()-1));
                InputActtach[4]=Acttach[4]=query.value("acttach5").toString();
            }
            ActtachPath=query.value("acttachpath").toString();
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
    dialog.setWindowTitle(tr("Preview"));
    dialog.setWindowFlags(Qt::WindowMaximizeButtonHint|Qt::WindowCloseButtonHint);
    dialog.setWindowIcon(QIcon(":/img/printer.png"));
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
    painter.translate(-width()/2, -height());

    this->render(&painter);
}

void ProjectResultInputDialog::on_pushButton_Acttach1_clicked()
{
    InputActtach[0]=QFileDialog::getOpenFileName(this,tr("Open File"),"c://",tr("All Files; (*.*)"));
    QStringList FileList=InputActtach[0].split("/",QString::KeepEmptyParts);
    ui->label_Acttach1->setText(FileList.at(FileList.count()-1));
}

void ProjectResultInputDialog::on_pushButton_Acttach2_clicked()
{
    InputActtach[1]=QFileDialog::getOpenFileName(this,tr("Open File"),"c://",tr("All Files; (*.*)"));
    QStringList FileList=InputActtach[1].split("/",QString::KeepEmptyParts);
    ui->label_Acttach2->setText(FileList.at(FileList.count()-1));
}

void ProjectResultInputDialog::on_pushButton_Acttach3_clicked()
{
    InputActtach[2]=QFileDialog::getOpenFileName(this,tr("Open File"),"c://",tr("All Files; (*.*)"));
    QStringList FileList=InputActtach[2].split("/",QString::KeepEmptyParts);
    ui->label_Acttach3->setText(FileList.at(FileList.count()-1));
}

void ProjectResultInputDialog::on_pushButton_Acttach4_clicked()
{
    InputActtach[3]=QFileDialog::getOpenFileName(this,tr("Open File"),"c://",tr("All Files; (*.*)"));
    QStringList FileList=InputActtach[3].split("/",QString::KeepEmptyParts);
    ui->label_Acttach4->setText(FileList.at(FileList.count()-1));
}

void ProjectResultInputDialog::on_pushButton_Acttach5_clicked()
{
    InputActtach[4]=QFileDialog::getOpenFileName(this,tr("Open File"),"c://",tr("All Files; (*.*)"));
    QStringList FileList=InputActtach[4].split("/",QString::KeepEmptyParts);
    ui->label_Acttach5->setText(FileList.at(FileList.count()-1));
}
