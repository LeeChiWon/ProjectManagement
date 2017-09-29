#include "userregistration_form.h"
#include "ui_userregistration_form.h"

UserRegistration_Form::UserRegistration_Form(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::UserRegistration_Form)
{
    ui->setupUi(this);
    ui->dateEdit_ResignDate->setDate(QDate::fromString("1900-01-01","yyyy-MM-dd"));
    ComboboxInit();
    SettingInit();
    ContextMenuInit();
    UserShow();
}

UserRegistration_Form::~UserRegistration_Form()
{
    delete ui;
}

void UserRegistration_Form::mouseReleaseEvent(QMouseEvent *event)
{
    if((event->pos().x()>= ui->lineEdit_CompanyNum->geometry().x()) && (event->pos().y()>= ui->lineEdit_CompanyNum->geometry().y()) &&
            (event->pos().x()<= ui->lineEdit_CompanyNum->geometry().bottomRight().x()) && (event->pos().y()<= ui->lineEdit_CompanyNum->geometry().bottomRight().y()))
    {
        ui->lineEdit_CompanyNum->setEnabled(true);
        ui->pushButton_Modify->setEnabled(false);
    }
}

void UserRegistration_Form::on_pushButton_Add_clicked()
{
    if(isEditEmpty())
    {
        if(UserAdd())
        {
            UserShow();
            UIInit();
            QMessageBox::information(this,tr("Information"),tr("User Add"),QMessageBox::Ok);
        }
    }
}

void UserRegistration_Form::on_pushButton_Modify_clicked()
{
    if(isEditEmpty())
    {
        if(UserModify())
        {
            UserShow();
            UIInit();
            QMessageBox::information(this,tr("Information"),tr("User Modify"),QMessageBox::Ok);
        }
    }
}

void UserRegistration_Form::on_pushButton_Delete_clicked()
{
    if(isEditEmpty())
    {
        if(UserDelete())
        {
            UserShow();
            UIInit();
            QMessageBox::information(this,tr("Information"),tr("User Delete"),QMessageBox::Ok);
        }
    }
}

void UserRegistration_Form::DBInit()
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

void UserRegistration_Form::SettingInit()
{
    Setting=new QSettings("EachOne","ProjectManagement",this);
}

void UserRegistration_Form::UserShow()
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

        query.exec("select * from user_management");

        while(query.next())
        {
            ui->tableWidget->setRowCount(ui->tableWidget->rowCount()+1);
            ui->tableWidget->setItem(ui->tableWidget->rowCount()-1,COLUMN_COMPANYNUMBER,new QTableWidgetItem(query.value("companynumber").toString()));
            ui->tableWidget->setItem(ui->tableWidget->rowCount()-1,COLUMN_NAME,new QTableWidgetItem(query.value("name").toString()));
            ui->tableWidget->setItem(ui->tableWidget->rowCount()-1,COLUMN_JOINDATE,new QTableWidgetItem(query.value("joindate").toString()));
            ui->tableWidget->setItem(ui->tableWidget->rowCount()-1,COLUMN_RESIGNDATE,new QTableWidgetItem(query.value("resigndate").toString()));
            ui->tableWidget->setItem(ui->tableWidget->rowCount()-1,COLUMN_ADMIN,new QTableWidgetItem(query.value("admin").toString()));
            /* if(ui->tableWidget->rowCount()%2==0)
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

void UserRegistration_Form::TableWidgetInit()
{
    ui->tableWidget->setColumnCount(5);
    ui->tableWidget->setHorizontalHeaderLabels(QStringList()<<tr("Companynumber")<<tr("Name")<<tr("JoinDate")<<tr("ResignDate")<<tr("Admin"));
}

bool UserRegistration_Form::UserAdd()
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

        if(ui->dateEdit_ResignDate->date().toString("yyyy-MM-dd") == "1900-01-01")
        {
            query.exec(QString("insert into user_management(companynumber, name, password, joindate, admin) values ('%1','%2','%3','%4',%5)")
                       .arg(ui->lineEdit_CompanyNum->text(),ui->lineEdit_Name->text()).arg("1111").arg(ui->dateEdit_JoinDate->date().toString("yyyy-MM-dd"))
                       .arg(ui->checkBox_Admin->isChecked()));
        }

        else
        {
            query.exec(QString("insert into user_management(companynumber, name, password, joindate, resigndate, admin) values ('%1','%2','%3','%4','%5',%6)")
                       .arg(ui->lineEdit_CompanyNum->text(),ui->lineEdit_Name->text()).arg("1111").arg(ui->dateEdit_JoinDate->date().toString("yyyy-MM-dd"),ui->dateEdit_ResignDate->date().toString("yyyy-MM-dd"))
                       .arg(ui->checkBox_Admin->isChecked()));
        }

        if(query.lastError().number()>0)
        {
            switch(query.lastError().number())
            {
            case DB_ERROR_INSERT_PRIMARYKEY:
                QMessageBox::warning(this,tr("Warning"),tr("CompanyNumber is depulicate."),QMessageBox::Ok);
                break;
            default:
                QMessageBox::warning(this,tr("Warning"),QString("%1\n%2").arg(tr("Database Error!"),query.lastError().text()),QMessageBox::Ok);
                break;
            }
            return false;
        }
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

bool UserRegistration_Form::UserModify()
{
    QSqlDatabase DB=QSqlDatabase::database("MainDB");

    try
    {
        if(!DB.isOpen())
        {
            QSqlDatabase::removeDatabase("MainDB");
            DBInit();
        }
        DB.transaction();
        QSqlQuery query(DB);

        query.exec(QString("update user_management set name='%1', joindate='%2', resigndate='%3', admin=%4 where companynumber='%5'")
                   .arg(ui->lineEdit_Name->text()).arg(ui->dateEdit_JoinDate->date().toString("yyyy-MM-dd"),ui->dateEdit_ResignDate->date().toString("yyyy-MM-dd"))
                   .arg(ui->checkBox_Admin->isChecked()).arg(ui->lineEdit_CompanyNum->text()));
        DB.commit();
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

bool UserRegistration_Form::UserDelete()
{
    QSqlDatabase DB=QSqlDatabase::database("MainDB");

    try
    {
        if(!DB.isOpen())
        {
            QSqlDatabase::removeDatabase("MainDB");
            DBInit();
        }
        DB.transaction();
        QSqlQuery query(DB);

        query.exec(QString("delete from user_management where companynumber='%1'")
                   .arg(ui->lineEdit_CompanyNum->text()));
        DB.commit();
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

void UserRegistration_Form::UIInit()
{
    ui->lineEdit_CompanyNum->setText("");
    ui->dateEdit_JoinDate->setDate(QDate::currentDate());
    ui->lineEdit_Name->setText("");
    ui->dateEdit_ResignDate->setDate(QDate::fromString("1900-01-01","yyyy-MM-dd"));
    ui->checkBox_Admin->setChecked(false);
    ui->pushButton_Modify->setEnabled(false);
    ui->pushButton_Delete->setEnabled(false);
}

bool UserRegistration_Form::isEditEmpty()
{
    if(ui->lineEdit_CompanyNum->text().isEmpty() || ui->lineEdit_Name->text().isEmpty())
    {
        QMessageBox::information(this,tr("Information"),tr("LineEdit is Empty."),QMessageBox::Ok);
        return false;
    }
    return true;
}

void UserRegistration_Form::ComboboxInit()
{
    ui->comboBox_Search->clear();
    ui->comboBox_Search->addItems(QStringList()<<tr("Name")<<tr("CompanyNumber"));
    ui->comboBox_Search->setCurrentIndex(0);
}

void UserRegistration_Form::ContextMenuInit()
{
    connect(ui->tableWidget,SIGNAL(customContextMenuRequested(QPoint)),this,SLOT(ContextMenuShow(QPoint)));
    QList <QAction*> ContextAction;
    ContextAction.append(new QAction(tr("ExcelSave")));
    ContextAction.at(0)->setIcon(QIcon(":/img/excel.png"));
    ContextAction.append(new QAction(tr("Print")));
    ContextAction.at(1)->setIcon(QIcon(":/img/printer.png"));
    ContextMenu=new QMenu();
    connect(ContextAction.at(0),SIGNAL(triggered(bool)),this,SLOT(ExcelSave(bool)));
    connect(ContextAction.at(1),SIGNAL(triggered(bool)),this,SLOT(Print(bool)));
    ContextMenu->addActions(ContextAction);
}


void UserRegistration_Form::on_tableWidget_itemClicked(QTableWidgetItem *item)
{
    ui->lineEdit_CompanyNum->setText(ui->tableWidget->item(item->row(),COLUMN_COMPANYNUMBER)->text());
    ui->lineEdit_CompanyNum->setEnabled(false);
    ui->pushButton_Modify->setEnabled(true);
    ui->pushButton_Delete->setEnabled(true);
    ui->lineEdit_Name->setText(ui->tableWidget->item(item->row(),COLUMN_NAME)->text());
    ui->dateEdit_JoinDate->setDate(QDate::fromString(ui->tableWidget->item(item->row(),COLUMN_JOINDATE)->text(),"yyyy-MM-dd"));
    if(ui->tableWidget->item(item->row(),COLUMN_RESIGNDATE)->text().isEmpty())
    {
        ui->dateEdit_ResignDate->setDate(QDate::fromString("1900-01-01","yyyy-MM-dd"));
    }
    else
    {
       ui->dateEdit_ResignDate->setDate(QDate::fromString(ui->tableWidget->item(item->row(),COLUMN_RESIGNDATE)->text(),"yyyy-MM-dd"));
    }
    ui->checkBox_Admin->setChecked(ui->tableWidget->item(item->row(),COLUMN_ADMIN)->text().toInt());
}

void UserRegistration_Form::on_pushButton_Search_clicked()
{
    QList<QTableWidgetItem *>TableWidgetItem=ui->tableWidget->findItems(ui->lineEdit_Search->text(), Qt::MatchContains);
    foreach(QTableWidgetItem *Item, TableWidgetItem)
    {
        ui->tableWidget->selectRow(Item->row());        
        ui->tableWidget->itemClicked(Item);
        QMessageBox::information(this,tr("Search Result"),tr("Search Item."),QMessageBox::Ok);
        break;
    }
}

void UserRegistration_Form::ExcelSave(bool clicked)
{
    QFileDialog FileDialog;
    QString FileName=FileDialog.getSaveFileName(this,tr("Save File"),"c://",tr("Excel (*.xlsx)"));

    if(!FileName.isEmpty() || !FileName.isNull())
    {
        QXlsx::Document xlsx(FileName);
        QStringList ColumnTitle=QStringList()<<tr("CompanyNumber")<<tr("Name")<<tr("JoinDate")<<tr("ResignDate")<<tr("Admin");
        int ColumnWidth[5]={15,10,11,11,11};
        QXlsx::Format format;

        format.setBorderStyle(QXlsx::Format::BorderThin);
        format.setFontSize(10);
        format.setFontBold(false);

        for(int i=0; i<ui->tableWidget->rowCount(); i++)
        {
            for(int j=0; j<5; j++)
            {
                xlsx.write(i+2,j+1,ui->tableWidget->item(i,j)->text(),format);
            }
        }

        format.setBottomBorderStyle(QXlsx::Format::BorderDouble);
        format.setFontBold(true);
        format.setFontSize(13);

        for(int i=1; i<6; i++)
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

void UserRegistration_Form::Print(bool clicked)
{
    QPrintPreviewDialog dialog;
    connect(&dialog, SIGNAL(paintRequested(QPrinter*)), this, SLOT(TableWidgetPrint(QPrinter*)));
    dialog.exec();
}

void UserRegistration_Form::ContextMenuShow(const QPoint &pos)
{
    if(ui->tableWidget->rowCount()>0)
    {
        ContextMenu->exec(ui->tableWidget->viewport()->mapToGlobal(pos));
    }
}

void UserRegistration_Form::TableWidgetPrint(QPrinter *printer)
{
    QPainter painter;
    if(!painter.begin(printer)) {
        QMessageBox::warning(this,tr("Warning"),tr("can't start printer"),QMessageBox::Ok);
        return;
    }
    // print table
    TablePrinter tablePrinter(&painter, printer);
    QVector<int> columnStretch = QVector<int>() << 4 << 3 << 3 << 3 << 1;
    if(!tablePrinter.printTable(ui->tableWidget->model(), columnStretch)) {
        QMessageBox::warning(this,tr("Error"),tablePrinter.lastError(),QMessageBox::Ok);
    }
    painter.end();
}
