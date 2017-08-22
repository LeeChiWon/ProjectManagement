#ifndef MAIN_FORM_H
#define MAIN_FORM_H

#include <QWidget>

namespace Ui {
class Main_Form;
}

class Main_Form : public QWidget
{
    Q_OBJECT

public:
    explicit Main_Form(QWidget *parent = 0);
    ~Main_Form();

private:
    Ui::Main_Form *ui;
};

#endif // MAIN_FORM_H
