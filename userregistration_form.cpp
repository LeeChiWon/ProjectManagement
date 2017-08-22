#include "userregistration_form.h"
#include "ui_userregistration_form.h"

UserRegistration_Form::UserRegistration_Form(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::UserRegistration_Form)
{
    ui->setupUi(this);
}

UserRegistration_Form::~UserRegistration_Form()
{
    delete ui;
}
