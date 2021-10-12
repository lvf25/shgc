#ifndef LOGSQLMODEL_H
#define LOGSQLMODEL_H

#include <QSqlTableModel>
class LogSqlModel : public QSqlTableModel
{
    Q_OBJECT
public:
    LogSqlModel(QObject *parent = nullptr);

    bool Init();
    virtual QHash<int, QByteArray> roleNames() const;

    QVariant data(const QModelIndex &index, int role) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);
    Q_INVOKABLE void setFilters ( const QString & filter );
};

#endif // LOGSQLMODEL_H
