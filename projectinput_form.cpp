#include "projectinput_form.h"
#include "ui_projectinput_form.h"

ProjectInput_Form::ProjectInput_Form(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ProjectInput_Form)
{
    ui->setupUi(this);
}

ProjectInput_Form::~ProjectInput_Form()
{
    delete ui;
}
