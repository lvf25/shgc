#ifndef SQLINIT_H
#define SQLINIT_H
#include <QMessageBox>
#include <QSqlDatabase>
#include <QSqlQuery>


static bool createConnection()
{


    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("SHGCData.db");
    if (!db.open()) {
        QMessageBox::critical(0, "Cannot open database1",
                              "Unable to establish a database connection.", QMessageBox::Cancel);
        return false;
    }
    QSqlQuery query;

    // 使数据库支持中文
    query.exec("SET NAMES 'Latin1'");

    // 创建分类表
    query.exec("create table logfile(filename varchar(60),opentime varchar(60),filedir varchar(60),id integer NOT NULL PRIMARY KEY AUTOINCREMENT)");

    return true;

}
#endif // SQLINIT_H
