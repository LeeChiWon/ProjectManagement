#ifndef PROJECTINPUT_FORM_H
#define PROJECTINPUT_FORM_H

#include <QWidget>

namespace Ui {
class ProjectInput_Form;
}

class ProjectInput_Form : public QWidget
{
    Q_OBJECT

public:
    explicit ProjectInput_Form(QWidget *parent = 0);
    ~ProjectInput_Form();

private:
    Ui::ProjectInput_Form *ui;
};

#endif // PROJECTINPUT_FORM_H
