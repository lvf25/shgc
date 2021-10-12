#include "LogSqlModel.h"



LogSqlModel::LogSqlModel(QObject *parent)
    :QSqlTableModel(parent)
{
    Init();
}

bool LogSqlModel::Init()
{
    setTable("logfile");
    setEditStrategy(QSqlTableModel::OnRowChange);
    select();
    setHeaderData(0,Qt::Horizontal,("filename"));
    setHeaderData(1,Qt::Horizontal,("opentime"));
    setHeaderData(2,Qt::Horizontal,("filedir"));

    return true;
}

QHash<int, QByteArray> LogSqlModel::roleNames() const
{
    QHash<int, QByteArray> roleNames;
    roleNames[Qt::UserRole] = "filename";
    roleNames[Qt::UserRole+1] = "opentime";
    roleNames[Qt::UserRole+2] = "filedir";
    return roleNames;
}


QVariant LogSqlModel::data(const QModelIndex &index, int role) const
{

    QVariant value = QSqlTableModel::data(index, role);
    if(role < Qt::UserRole)
    {
        value = QSqlTableModel::data(index, role);
    }
    else
    {
        int columnIdx = role - Qt::UserRole;
        QModelIndex modelIndex = this->index(index.row(), columnIdx);
        value = QSqlTableModel::data(modelIndex, Qt::DisplayRole);
    }

    return value;
}

bool LogSqlModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    bool ret;
    if(role < Qt::UserRole)
    {
        ret = QSqlTableModel::setData(index, value, role);
    }
    else
    {
        int colIdx = role - Qt::UserRole - 1;
        ret = QSqlTableModel::setData(index, value, Qt::DisplayRole);
    }

    return ret;
}

void LogSqlModel::setFilters(const QString &filter)
{
//    if(filter == "*")
//    {
//        this->setFilter(filter);
//        return;

//    }
//    this->setFilter(QObject::tr("filename = '%1'").arg(filter));
    this->setFilter(QObject::tr("filename like '%1'").arg(filter + "%"));
}
