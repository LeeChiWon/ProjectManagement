#include "projectinput_form.h"
#include "ui_projectinput_form.h"

ProjectInput_Form::ProjectInput_Form(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ProjectInput_Form)
{
    ui->setupUi(this);
    SettingInit();

    ui->dateEdit_AgreementEnd->setDate(QDate::currentDate());
    ui->dateEdit_AgreementStart->setDate(QDate::currentDate());
}

ProjectInput_Form::~ProjectInput_Form()
{
    delete ui;
}

void ProjectInput_Form::on_pushButton_Save_clicked()
{
    if(isEditEmpty())
    {
        QString Values=QString("'%1','%2','%3','%4','%5','%6','%7','%8','%9','%10','%11','%12','%13','%14','%15','%16','%17','%18'")
                .arg(ui->lineEdit_BusinessType->text(),ui->lineEdit_ProjectNumber->text(),ui->lineEdit_ManagementAgency->text(),ui->lineEdit_BusinessGroup1->text(),ui->lineEdit_BusinessGroup2->text(),ui->lineEdit_BusinessGroup3->text())
                .arg(ui->lineEdit_Organization->text(),ui->lineEdit_Manager->text(),ui->lineEdit_ProjectName->text(),ui->dateEdit_AgreementStart->date().toString("yyyy-MM-dd"),ui->dateEdit_AgreementEnd->date().toString("yyyy-MM-dd"))
                .arg(ui->lineEdit_Contribution->text().replace(",",""),ui->lineEdit_LardaceousSpleen->text().replace(",",""),ui->lineEdit_Deductible_Cash->text().replace(",",""),ui->lineEdit_Deductible_Goods->text().replace(",",""),ui->lineEdit_ProjectCost->text().replace(",",""))
                .arg(ui->lineEdit_TotalProjectCost->text().replace(",",""),ui->lineEdit_CarriedCost->text().replace(",",""));
        if(Input(Values))
        {
            UIInit();
            QMessageBox::information(this,tr("Information"),tr("ProjectInput Saved."),QMessageBox::Ok);
        }
    }
}

bool ProjectInput_Form::isEditEmpty()
{
    if(ui->lineEdit_BusinessGroup1->text().isEmpty() || ui->lineEdit_BusinessGroup2->text().isEmpty() || ui->lineEdit_BusinessGroup3->text().isEmpty() || ui->lineEdit_BusinessType->text().isEmpty()
            ||ui->lineEdit_CarriedCost->text().isEmpty() ||ui->lineEdit_Contribution->text().isEmpty()||ui->lineEdit_Deductible_Cash->text().isEmpty()||ui->lineEdit_Deductible_Goods->text().isEmpty()
            ||ui->lineEdit_LardaceousSpleen->text().isEmpty()||ui->lineEdit_ManagementAgency->text().isEmpty()||ui->lineEdit_Manager->text().isEmpty() ||ui->lineEdit_Organization->text().isEmpty()
            ||ui->lineEdit_ProjectCost->text().isEmpty() ||ui->lineEdit_ProjectName->text().isEmpty() ||ui->lineEdit_ProjectNumber->text().isEmpty()||ui->lineEdit_TotalProjectCost->text().isEmpty())
    {
        QMessageBox::information(this,tr("Information"),tr("LineEdit is Empty."),QMessageBox::Ok);
        return false;
    }
    return true;
}

bool ProjectInput_Form::ExcelUpload(QString FileName)
{
    int Count=2;
    QXlsx::Document xlsx(FileName);
    QString Temp;
    QXlsx::Format lAlign;
    lAlign.setHorizontalAlignment(QXlsx::Format::AlignLeft);
    xlsx.write("S1","=COUNTA(A1:A200)",lAlign);

    while(1)
    {
        if(xlsx.read(QString("A%1").arg(Count)).isNull())
        {
            break;
        }
        for(int j=0; j<18; j++)
        {
            if(j==17)
            {                
                Temp.append("'"+xlsx.read(Count,j+1).toString()+"'");
                break;
            }
            else if(j>10)
            {
                Temp.append(QString("'%1',").arg(xlsx.read(Count,j+1).toInt()));
            }
            else
            {
                Temp.append("'"+xlsx.read(Count,j+1).toString()+"',");
            }
        }

        Input(Temp);
        Temp.clear();
        Count++;
    }
    xlsx.deleteLater();
    return true;
}

void ProjectInput_Form::on_pushButton_Upload_clicked()
{
    if(ExcelUpload(QFileDialog::getOpenFileName(this,tr("Open Excel File"),"c://",tr("Excel (*.xlsx)"))))
    {
        QMessageBox::information(this,tr("Information"),tr("Excel Upload Complete."),QMessageBox::Ok);
    }
}

void ProjectInput_Form::UIInit()
{
    ui->lineEdit_BusinessGroup1->clear();
    ui->lineEdit_BusinessGroup2->clear();
    ui->lineEdit_BusinessGroup3->clear();
    ui->lineEdit_BusinessType->clear();
    ui->lineEdit_CarriedCost->clear();
    ui->lineEdit_Contribution->clear();
    ui->lineEdit_Deductible_Cash->clear();
    ui->lineEdit_LardaceousSpleen->clear();
    ui->lineEdit_ManagementAgency->clear();
    ui->lineEdit_Deductible_Goods->clear();
    ui->lineEdit_Manager->clear();
    ui->lineEdit_Organization->clear();
    ui->lineEdit_ProjectCost->clear();
    ui->lineEdit_ProjectName->clear();
    ui->lineEdit_ProjectNumber->clear();
    ui->lineEdit_TotalProjectCost->clear();
    ui->dateEdit_AgreementEnd->setDate(QDate::currentDate());
    ui->dateEdit_AgreementStart->setDate(QDate::currentDate());
}

void ProjectInput_Form::DBInit()
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

void ProjectInput_Form::SettingInit()
{
    Setting=new QSettings("EachOne","ProjectManagement",this);
}

bool ProjectInput_Form::Input(QString Values)
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

        query.exec(QString("insert into project_management(businesstype, projectnumber, managementagency, businessgroup1, businessgroup2, businessgroup3,"
                           "organization, manager, projectname, agreementstartdate, agreementenddate, contribution,lardaceousspleen,"
                           "deductible_cash, deductible_goods, projectcost, totalprojectcost, carriedcost) values (%1)").arg(Values));

        if(query.lastError().number()>0)
        {
            switch(query.lastError().number())
            {
            case DB_ERROR_INSERT_PRIMARYKEY:
                QMessageBox::warning(this,tr("Warning"),tr("ProjectName is depulicate."),QMessageBox::Ok);
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

void ProjectInput_Form::on_lineEdit_TotalProjectCost_editingFinished()
{
    ui->lineEdit_TotalProjectCost->setText(QString("%L1").arg(ui->lineEdit_TotalProjectCost->text().replace(",","").toInt()));
}

void ProjectInput_Form::on_lineEdit_LardaceousSpleen_editingFinished()
{
    ui->lineEdit_LardaceousSpleen->setText(QString("%L1").arg(ui->lineEdit_LardaceousSpleen->text().replace(",","").toInt()));
}

void ProjectInput_Form::on_lineEdit_Deductible_Cash_editingFinished()
{
    ui->lineEdit_Deductible_Cash->setText(QString("%L1").arg(ui->lineEdit_Deductible_Cash->text().replace(",","").toInt()));
}

void ProjectInput_Form::on_lineEdit_Deductible_Goods_editingFinished()
{
    ui->lineEdit_Deductible_Goods->setText(QString("%L1").arg(ui->lineEdit_Deductible_Goods->text().replace(",","").toInt()));
}

void ProjectInput_Form::on_lineEdit_ProjectCost_editingFinished()
{
    ui->lineEdit_ProjectCost->setText(QString("%L1").arg(ui->lineEdit_ProjectCost->text().replace(",","").toInt()));
}

void ProjectInput_Form::on_lineEdit_CarriedCost_editingFinished()
{
    ui->lineEdit_CarriedCost->setText(QString("%L1").arg(ui->lineEdit_CarriedCost->text().replace(",","").toInt()));
}

void ProjectInput_Form::on_lineEdit_Contribution_editingFinished()
{
    ui->lineEdit_Contribution->setText(QString("%L1").arg(ui->lineEdit_Contribution->text().replace(",","").toInt()));
}
