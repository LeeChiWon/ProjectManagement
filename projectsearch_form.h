#ifndef PROJECTSEARCH_FORM_H
#define PROJECTSEARCH_FORM_H

#include <QWidget>

namespace Ui {
class ProjectSearch_Form;
}

class ProjectSearch_Form : public QWidget
{
    Q_OBJECT

public:
    explicit ProjectSearch_Form(QWidget *parent = 0);
    ~ProjectSearch_Form();

private:
    Ui::ProjectSearch_Form *ui;
};

#endif // PROJECTSEARCH_FORM_H
