#ifndef QMLOBJECTTABLEMODEL_H
#define QMLOBJECTTABLEMODEL_H

#include <QAbstractTableModel>
#include <QVariantList>
#include <QList>
#include <QPushButton>
#include <QApplication>
#include <QMouseEvent>
#include <QDialog>
#include <QPainter>
#include <QStyleOption>
#include <QDesktopWidget>
#include <QDebug>
#include <QMap>
#include <QItemDelegate>
#include <QStyledItemDelegate>
#include <QToolTip>
#include "pushbuttonitemdelegate.h"

#define COLUMNUM 10

class QmlObjectTableModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    QmlObjectTableModel(QObject* parent = NULL);
    ~QmlObjectTableModel();

    Q_PROPERTY(int count READ count NOTIFY countChanged)

    /// Returns true if any of the items in the list are dirty. Requires each object to have
    /// a dirty property and dirtyChanged signal.
    Q_PROPERTY(bool dirty READ dirty WRITE setDirty NOTIFY dirtyChanged)

    Q_INVOKABLE QObject* get(int index)     { return _objectList[index]; }

    // Property accessors

    int         count               () const;
    bool        dirty               () const { return _dirty; }
    void        setDirty            (bool dirty);
    void        append              (QObject* object);
    void        append              (QList<QObject*> objects);
    QObjectList swapObjectList      (const QObjectList& newlist);
    void        clear               ();
    QObject*    removeAt            (int i);
    QObject*    removeOne           (QObject* object) { return removeAt(indexOf(object)); }
    void        insert              (int i, QObject* object);
    void        insert              (int i, QList<QObject*> objects);
    bool        contains            (QObject* object) { return _objectList.indexOf(object) != -1; }
    int         indexOf             (QObject* object) { return _objectList.indexOf(object); }

    QObject*    operator[]          (int i);
    const QObject* operator[]       (int i) const;
    QmlObjectTableModel&    operator=          (const QmlObjectTableModel& object);
    template<class T> T value       (int index) { return qobject_cast<T>(_objectList[index]); }
    QList<QObject*>* objectList     () { return &_objectList; }
    void        refrushModel        (void);

    /// Calls deleteLater on all items and this itself.
    void deleteListAndContents      ();

    /// Clears the list and calls deleteLater on each entry
    void clearAndDeleteContents     ();

    void beginReset                 () { beginResetModel(); }
    void endReset                   () { endResetModel();   }
    int rowCount                    (const QModelIndex & parent = QModelIndex()) const override;
    int columnCount                 (const QModelIndex & parent = QModelIndex()) const override;
    bool upMove                     (int index);
    bool downMove                   (int index);

signals:
    void countChanged               (int count);
    void updateCount                (int count);
    void dirtyChanged               (bool dirtyChanged);

private slots:
    void _childDirtyChanged         (bool dirty);

public slots:
    void addPointToVehicle          (int seq);
private:
//    // Overrides from QAbstractListModel
//    int         rowCount    (const QModelIndex & parent = QModelIndex()) const override;
//    int         columnCount (const QModelIndex & parent = QModelIndex()) const override;

    QVariant    data        (const QModelIndex & index, int role = Qt::DisplayRole) const override;
    QVariant    headerData(int section, Qt::Orientation orientation, int role) const override;

    bool        insertRows  (int position, int rows, const QModelIndex &index = QModelIndex()) override;
    bool        removeRows  (int position, int rows, const QModelIndex &index = QModelIndex()) override;
    bool        setData     (const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;
    Qt::ItemFlags flags(const QModelIndex &index) const;
    QHash<int, QByteArray> roleNames(void) const override;

private:
    QList<QObject*> _objectList;
    QObject itemdata;

    bool _dirty;
    bool _skipDirtyFirstItem;

    static const int ObjectRole;
    static const int TextRole;

    QStringList header;
    QMap<int, Qt::CheckState> rowCheckStateMap;

};

#endif // QMLOBJECTTABLEMODEL_H
