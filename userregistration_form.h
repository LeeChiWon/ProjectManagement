#ifndef USERREGISTRATION_FORM_H
#define USERREGISTRATION_FORM_H

#include <QWidget>

namespace Ui {
class UserRegistration_Form;
}

class UserRegistration_Form : public QWidget
{
    Q_OBJECT

public:
    explicit UserRegistration_Form(QWidget *parent = 0);
    ~UserRegistration_Form();

private:
    Ui::UserRegistration_Form *ui;
};

#endif // USERREGISTRATION_FORM_H
