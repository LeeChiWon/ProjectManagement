#include "projectresultinputdialog.h"
#include "ui_projectresultinputdialog.h"

ProjectResultInputDialog::ProjectResultInputDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ProjectResultInputDialog)
{
    ui->setupUi(this);
}

ProjectResultInputDialog::~ProjectResultInputDialog()
{
    delete ui;
}

void ProjectResultInputDialog::on_pushButton_Close_clicked()
{
    this->close();
}
