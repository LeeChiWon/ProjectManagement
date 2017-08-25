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

#define LOGIN_USER 0
#define LOGIN_MANAGER 1
#define LOGIN_MASTER 2

#define MODE_LOGOUT 1
#define MODE_EXIT 2

#define COLUMN_COMPANYNUMBER 0
#define COLUMN_NAME 1
#define COLUMN_JOINDATE 2
#define COLUMN_RESIGNDATE 3
#define COLUMN_ADMIN 4

#define DB_ERROR_INSERT_PRIMARYKEY 19
#endif // DEFINE_H
