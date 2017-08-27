#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "define.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    CloseMode(MODE_EXIT)
{
    ui->setupUi(this);
    TabWidgetInit();
    SettingInit();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_Logout_clicked()
{
    CloseMode=MODE_LOGOUT;
    this->close();
    QDesktopServices::openUrl(QUrl("file:///"+QApplication::applicationFilePath()));
}

void MainWindow::on_actionConfiguration_triggered(bool checked)
{   
    if(!TabWidgetCompare(tr("Configuration")))
    {
        ui->tabWidget->addTab(new Configuration_form,tr("Configuration"));
        ui->tabWidget->setCurrentIndex(ui->tabWidget->count()-1);
    }
}

void MainWindow::onSystemTryIconClicked(QSystemTrayIcon::ActivationReason reason)
{
    switch(reason)
    {
    case QSystemTrayIcon::Trigger:
    case QSystemTrayIcon::DoubleClick:
        this->setWindowState(Qt::WindowActive);
        this->show();
        break;
    }
}

void MainWindow::on_actionExit_triggered()
{
    CloseMode=MODE_EXIT;
    this->close();
}

void MainWindow::TrayIconInit()
{
    TrayIcon=new QSystemTrayIcon(this);
    TrayIconMenu=new QMenu(this);
    TrayIconMenu->addAction(ui->actionExit);
    TrayIconMenu->addSeparator();
    TrayIcon->setContextMenu(TrayIconMenu);
    QIcon Icon(":/img/projectmanagement.png");
    TrayIcon->setIcon(Icon);
    TrayIcon->show();
    TrayIcon->setToolTip(tr("ProjectManagement"));
    TrayIcon->showMessage(tr("ProjectManagement"),tr("Executing..."),QSystemTrayIcon::Information,5000);
    connect(TrayIcon,SIGNAL(activated(QSystemTrayIcon::ActivationReason)),this,SLOT(onSystemTryIconClicked(QSystemTrayIcon::ActivationReason)));
}

void MainWindow::UserInfo(QString ID, QString Name, int Level)
{
    LoginID=ID;
    User=Name;
    ui->label_ID->setText(User);
    LoginLevel=Level;
    LoginLevelCheck();
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    event->ignore();

    switch(CloseMode)
    {
    case MODE_EXIT:
        int ret = QMessageBox::warning(this, tr("Application Exit"),
                                       tr("Do you want exit application?"),
                                       QMessageBox::Ok| QMessageBox::Cancel,
                                       QMessageBox::Ok);
        switch(ret)
        {
        case QMessageBox::Ok:
            if(TrayIcon->isVisible())
            {
                TrayIcon->hide();
            }
            Setting->setValue("Configuration/Geometry",saveGeometry());
            break;
        default:
            return;
            break;
        }
        break;
    }
    event->accept();
}

bool MainWindow::TabWidgetCompare(QString TabWidgetName)
{
    for(int i=0; i<ui->tabWidget->count(); i++)
    {
        if(ui->tabWidget->tabBar()->tabText(i)==TabWidgetName)
        {
            ui->tabWidget->setCurrentIndex(i);
            return true;
        }
    }
    return false;
}

void MainWindow::TabWidgetInit()
{
    ui->tabWidget->clear();
    ui->tabWidget->addTab(new Main_Form,tr("Main"));
}

void MainWindow::LoginLevelCheck()
{
    switch(LoginLevel)
    {
    case LOGIN_MASTER:
    case LOGIN_MANAGER:
        ui->actionUserRegistration->setEnabled(true);
        ui->actionProjectInput->setEnabled(true);
        break;
    }
}

void MainWindow::hideEvent(QHideEvent *event)
{
    if(isMinimized())
    {
        this->hide();
    }
    QWidget::hideEvent(event);
}

void MainWindow::on_tabWidget_tabCloseRequested(int index)
{
    ui->tabWidget->widget(index)->deleteLater();
    ui->tabWidget->removeTab(index);
}

void MainWindow::on_actionUserRegistration_triggered()
{
    if(!TabWidgetCompare(tr("UserRegistration")))
    {
        ui->tabWidget->addTab(new UserRegistration_Form,tr("UserRegistration"));
        ui->tabWidget->setCurrentIndex(ui->tabWidget->count()-1);
    }
}

void MainWindow::SettingInit()
{
    Setting=new QSettings("EachOne","ProjectManagement",this);
    restoreGeometry(Setting->value("Configuration/Geometry").toByteArray());
}

void MainWindow::DBInit()
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

void MainWindow::on_actionProjectInput_triggered()
{
    if(!TabWidgetCompare(tr("ProjectInput")))
    {
        ui->tabWidget->addTab(new ProjectInput_Form,tr("ProjectInput"));
        ui->tabWidget->setCurrentIndex(ui->tabWidget->count()-1);
    }
}

void MainWindow::on_actionProjectSearch_triggered()
{
    if(!TabWidgetCompare(tr("ProjectSearch")))
    {
        ui->tabWidget->addTab(new ProjectSearch_Form,tr("ProjectSearch"));
        ui->tabWidget->setCurrentIndex(ui->tabWidget->count()-1);
    }
}
