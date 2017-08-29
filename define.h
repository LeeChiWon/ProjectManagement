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


extern int g_LoginLevel;
extern QString g_User;
extern QString g_LoginID;

#endif // DEFINE_H
