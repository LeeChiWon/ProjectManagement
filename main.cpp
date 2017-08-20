#include "mainwindow.h"
#include <QApplication>
#include "logindialog.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    LoginDialog LoginDlg;

    QObject::connect(&LoginDlg,SIGNAL(LoggedIn()),&w,SLOT(show()));
    LoginDlg.show();
    //w.show();

    return a.exec();
}
