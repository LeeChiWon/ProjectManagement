#-------------------------------------------------
#
# Project created by QtCreator 2017-08-18T12:27:49
#
#-------------------------------------------------

QT       += core gui sql xlsx printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ProjectManagement
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += main.cpp\
        mainwindow.cpp \
    logindialog.cpp \
    configuration_form.cpp \
    main_form.cpp \
    userregistration_form.cpp \
    tableprinter.cpp \
    projectinput_form.cpp \
    projectsearch_form.cpp \
    projectresultinputdialog.cpp

HEADERS  += mainwindow.h \
    logindialog.h \
    define.h \
    configuration_form.h \
    main_form.h \
    userregistration_form.h \
    tableprinter.h \
    projectinput_form.h \
    projectsearch_form.h \
    projectresultinputdialog.h

FORMS    += mainwindow.ui \
    logindialog.ui \
    configuration_form.ui \
    main_form.ui \
    userregistration_form.ui \
    projectinput_form.ui \
    projectsearch_form.ui \
    projectresultinputdialog.ui

RESOURCES += \
    projectmanagement.qrc

win32
{
   RC_FILE = ProjectManagement.rc
   CONFIG += embed_manifest_exe
   QMAKE_LFLAGS_WINDOWS += /MANIFESTUAC:level=\'requireAdministrator\'
}

TRANSLATIONS += Lang_ko_KR.ts

DISTFILES +=


