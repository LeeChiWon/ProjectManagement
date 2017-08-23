#include "configuration_form.h"
#include "ui_configuration_form.h"

Configuration_form::Configuration_form(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Configuration_form)
{
    ui->setupUi(this);
    TabWidgetInit();
    TreeWidgetInit();
    SettingInit();
}

Configuration_form::~Configuration_form()
{
    delete ui;
}

void Configuration_form::on_pushButton_DBPath_clicked()
{
    ui->lineEdit_DBPath->setText(QFileDialog::getOpenFileName(this,tr("Open DB File"),"c://",tr("SQLITE (*.db)")));
}

void Configuration_form::on_pushButton_Apply_clicked()
{
    Setting->setValue("Configuration/AutoStartCheckbox",ui->checkBox_AutoStart->isChecked());
    AutoStartChecked(ui->checkBox_AutoStart->isChecked());
    Setting->setValue("Configuration/DBBackup",ui->checkBox_DBBackup->isChecked());
    Setting->setValue("Configuration/DBPath",ui->lineEdit_DBPath->text());
    QMessageBox::information(this,tr("Information"),tr("Configuration Applied."),QMessageBox::Ok);
}

void Configuration_form::TabWidgetInit()
{
    ui->tabWidget->tabBar()->setVisible(false);
}

void Configuration_form::TreeWidgetInit()
{
    ui->treeWidget->clear();

    QTreeWidgetItem *Item;
    // QTreeWidgetItem *subItem;

    QStringList Text=QStringList()<<tr("Defalut")<<tr("Database");
    ui->treeWidget->setColumnCount(1);
    ui->treeWidget->header()->setHidden(true);

    for(int i=0; i<Text.count(); i++)
    {
        Item=new QTreeWidgetItem();
        Item->setText(0,Text.at(i));
        ui->treeWidget->addTopLevelItem(Item);
    }

    ui->treeWidget->expandAll();
}

void Configuration_form::SettingInit()
{
    Setting=new QSettings("EachOne","ProjectManagement",this);
    ui->checkBox_AutoStart->setChecked(Setting->value("Configuration/AutoStartCheckbox").toBool());    
    ui->checkBox_DBBackup->setChecked(Setting->value("Configuration/DBBackup").toBool());
    ui->lineEdit_DBPath->setText(Setting->value("Configuration/DBPath").toString());
}

void Configuration_form::AutoStartChecked(bool checked)
{
    QSettings settings("HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Run",
                           QSettings::NativeFormat);
    QString Path=QString("\"%1\"").arg(QCoreApplication::applicationFilePath().replace('/','\\'));

    if(checked)
    {
        settings.setValue("ProjectManagement",Path);
    }
    else
    {
        settings.remove("ProjectManagement");
    }
}

void Configuration_form::on_treeWidget_itemClicked(QTreeWidgetItem *item, int column)
{
    for(int i=0; i<ui->tabWidget->count(); i++)
    {
        if(ui->tabWidget->tabText(i)==item->text(column))
        {
            ui->tabWidget->setCurrentIndex(i);
            ui->label_Title->setText(ui->tabWidget->tabText(i));
        }
    }
}
