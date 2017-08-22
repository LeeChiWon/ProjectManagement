#include "main_form.h"
#include "ui_main_form.h"

Main_Form::Main_Form(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Main_Form)
{
    ui->setupUi(this);
}

Main_Form::~Main_Form()
{
    delete ui;
}
