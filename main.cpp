#include "mainwindow.h"
#include <QApplication>
#include "logindialog.h"
#include <QtWidgets>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QTranslator Translator;
    Translator.load(":/translator/Lang_ko_KR.qm");
    QApplication::installTranslator(&Translator);

    MainWindow w;
    LoginDialog LoginDlg;

    QObject::connect(&LoginDlg,SIGNAL(LoggedIn()),&w,SLOT(show()));
    LoginDlg.show();
    //w.show();

    return a.exec();
}
