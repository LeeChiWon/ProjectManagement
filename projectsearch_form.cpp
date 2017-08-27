#include "projectsearch_form.h"
#include "ui_projectsearch_form.h"

ProjectSearch_Form::ProjectSearch_Form(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ProjectSearch_Form)
{
    ui->setupUi(this);
}

ProjectSearch_Form::~ProjectSearch_Form()
{
    delete ui;
}
