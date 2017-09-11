#ifndef DEFINE_H
#define DEFINE_H

#include <QtWidgets>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QtXlsx>
#include <QtPrintSupport/QPrinter>
#include <QtPrintSupport/QPrintDialog>
#include <QtPrintSupport/QPrintPreviewDialog>

enum UserLevel
{
    LOGIN_USER, LOGIN_MANAGER, LOGIN_MASTER
};

enum LogMode
{
   MODE_LOGOUT=1, MODE_EXIT=2,
};

enum Project_UserManagement_Column
{
   COLUMN_COMPANYNUMBER, COLUMN_NAME, COLUMN_JOINDATE, COLUMN_RESIGNDATE, COLUMN_ADMIN
};

enum QueryCommand
{
    INIT, SEARCH, DEL, UPDATE
};

enum Project_Management_Column
{
    BUSINESSTYPE,PROJECTNUMBER,MANAGEMENTAGENCY,BUSINESSGROUP1,BUSINESSGROUP2,BUSINESSGROUP3,ORGANIZATION,MANAGER,PROJECTNAME
};
enum DBErrorList
{
    DB_ERROR_INSERT_PRIMARYKEY=19
};
enum Acttach
{
    ACTTACH1,ACTTACH2,ACTTACH3,ACTTACH4,ACTTACH5
};
enum Statistics
{
    STATISTICS_ALL=2,STATISTICS_BEFORECOMPLETE=3,STATISTICS_BEFORENON=4,STATISTICS_AFTERCOMPLETE=5,STATISTICS_AFTERNON=6,
    TAB_SUBJECT=0,TAB_RECOGNITION=1
};


extern int g_LoginLevel;
extern QString g_User;
extern QString g_LoginID;

#endif // DEFINE_H
