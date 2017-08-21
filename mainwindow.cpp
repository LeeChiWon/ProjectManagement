#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "define.h"

QString LoginID;
int LoginLevel;


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_Logout_clicked()
{
    this->hide();
    this->close();
    QDesktopServices::openUrl(QUrl("file:///"+QApplication::applicationFilePath()));
}

void MainWindow::on_actionConfiguration_triggered(bool checked)
{
    ui->actionConfiguration->setChecked(checked);
}
