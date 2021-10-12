#include "qmlobjecttablemodel.h"


#include <QDebug>
#include <QQmlEngine>
#include "VisualMissionItem.h"
#include "SimpleMissionItem.h"

#include "MissionSettingsItem.h"

#pragma execution_character_set("utf-8")

const int QmlObjectTableModel::ObjectRole = Qt::UserRole;
const int QmlObjectTableModel::TextRole = Qt::UserRole + 1;

QmlObjectTableModel::QmlObjectTableModel(QObject* parent)
    : QAbstractTableModel(parent)
    , _dirty(false)
    , _skipDirtyFirstItem(false)
{

    _objectList.clear();
    header << tr("序号") << tr("动作") << tr("经度") << tr("纬度") << tr("相对起飞\n点高度M") << tr("相对上一航点\n距离M") << tr("增加") << tr("删除") << tr("上移") << tr("下移");
}

QmlObjectTableModel::~QmlObjectTableModel()
{
    qDebug() << "------------------------------delete QmlObjectTableModel-------------------" << endl;
}

int QmlObjectTableModel::columnCount(const QModelIndex &parent) const
{
    return COLUMNUM;
}

int QmlObjectTableModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);

    return _objectList.count();
}

QVariant QmlObjectTableModel::data(const QModelIndex &index, int role) const
{
    if(!index.isValid())
    {
        return QVariant();
    }

    if (index.row() < 0 || index.row() >= _objectList.count()) {
        return QVariant();
    }

    if(role == Qt::DisplayRole)
    {
        if(index.row() >= _objectList.count())
            return QVariant();
        if(index.column() >= COLUMNUM)
            return QVariant();

        QObject* object = _objectList[index.row()];
        if(index.column() == 0)
        {
            if(index.row() == 0)
            {
                MissionSettingsItem* pointItem = qobject_cast<MissionSettingsItem*>(object);
                return pointItem->sequenceNumber();
            }
            else {
                VisualMissionItem*  pointItem =  qobject_cast<VisualMissionItem*>(object);
                return pointItem->sequenceNumber();
            }
        }

        if(index.column() == 1)
        {
            if(index.row() == 0)
            {
                MissionSettingsItem* pointItem = qobject_cast<MissionSettingsItem*>(object);
                return pointItem->commandName();
            }
            else {
                VisualMissionItem*  pointItem =  qobject_cast<VisualMissionItem*>(object);
                return pointItem->commandName();
            }
        }

        if(index.column() == 2)
        {
            if(index.row() == 0)
            {
                MissionSettingsItem* pointItem = qobject_cast<MissionSettingsItem*>(object);
                return pointItem->coordinate().latitude();
            }
            else {
                VisualMissionItem*  pointItem =  qobject_cast<VisualMissionItem*>(object);
                return pointItem->coordinate().latitude();
            }
        }


        if(index.column() == 3)
        {
            if(index.row() == 0)
            {
                MissionSettingsItem* pointItem = qobject_cast<MissionSettingsItem*>(object);
                return pointItem->coordinate().longitude();
            }
            else {
                VisualMissionItem*  pointItem =  qobject_cast<VisualMissionItem*>(object);
                return pointItem->coordinate().longitude();
            }
        }

        if(index.column() == 4)
        {
            if(index.row() == 0)
            {
                MissionSettingsItem* pointItem = qobject_cast<MissionSettingsItem*>(object);
                return pointItem->coordinate().altitude();
            }
            else {
                VisualMissionItem*  pointItem =  qobject_cast<VisualMissionItem*>(object);
                return pointItem->coordinate().altitude();
            }
        }

        if(index.column() == 5)
        {
            if(index.row() == 0)
            {
                MissionSettingsItem* pointItem = qobject_cast<MissionSettingsItem*>(object);
                return pointItem->distance();
            }
            else {
                VisualMissionItem*  pointItem =  qobject_cast<VisualMissionItem*>(object);
                return pointItem->distance();
            }
        }

    }

    if (role == Qt::CheckStateRole)
    {
        if (index.column() == 0)
        {
            if (rowCheckStateMap.contains(index.row()))
            return rowCheckStateMap[index.row()] == Qt::Checked ? Qt::Checked : Qt::Unchecked; return Qt::Unchecked;
        }
    }

    return QVariant();

}


QHash<int, QByteArray> QmlObjectTableModel::roleNames(void) const
{  
    QHash<int, QByteArray> hash;

    hash[ObjectRole] = "object";
    hash[TextRole] = "text";

    return hash;
}

bool QmlObjectTableModel::setData(const QModelIndex& index, const QVariant& value, int role)
{
    if(!index.isValid())
        return false;

    if (role == Qt::CheckStateRole && index.column() == 0)
    {
        if (value == Qt::Checked)
        {
            rowCheckStateMap[index.row()] = Qt::Checked;
        }
        else
        {
            rowCheckStateMap[index.row()] = Qt::Unchecked;
        }
        return true;
    }

    if (index.isValid() && role == ObjectRole) {
        _objectList.replace(index.row(), value.value<QObject*>());
        emit dataChanged(index, index);
        return true;
    }

    return false;
}





bool QmlObjectTableModel::insertRows(int position, int rows, const QModelIndex& parent)
{
    Q_UNUSED(parent);

    if (position < 0 || position > _objectList.count() + 1) {
        qWarning() << "Invalid position position:count" << position << _objectList.count();
    }

    beginInsertRows(QModelIndex(), position, position + rows - 1);
    endInsertRows();

    emit countChanged(count());

    return true;
}



bool QmlObjectTableModel::removeRows(int position, int rows, const QModelIndex& parent)
{
    Q_UNUSED(parent);

    if (position < 0 || position >= _objectList.count()) {
        qWarning() << "Invalid position position:count" << position << _objectList.count();
    } else if (position + rows > _objectList.count()) {
        qWarning() << "Invalid rows position:rows:count" << position << rows << _objectList.count();
    }

    beginRemoveRows(QModelIndex(), position, position + rows - 1);
    for (int row=0; row<rows; row++) {
        _objectList.removeAt(position);
        qDebug() << "remove one row---------------------" << position << endl;
    }
    endRemoveRows();

    emit countChanged(count());

    return true;
}

QObject* QmlObjectTableModel::operator[](int index)
{
    if (index < 0 || index >= _objectList.count()) {
        return NULL;
    }
    return _objectList[index];
}

QmlObjectTableModel& QmlObjectTableModel::operator= (const QmlObjectTableModel& object)
{
    if(this == &object)
    {
        return *this;
    }
    this->_objectList = object._objectList;
    return  *this;
}



const QObject* QmlObjectTableModel::operator[](int index) const
{
    if (index < 0 || index >= _objectList.count()) {
        return NULL;
    }
    return _objectList[index];
}

void QmlObjectTableModel::clear()
{
    while (rowCount()) {
        qDebug() << "remove --------clear " << "   " << rowCount() << endl;
        removeAt(0);
    }
}

QObject* QmlObjectTableModel::removeAt(int i)
{
    QObject* removedObject = _objectList[i];
    if(removedObject) {
        // Look for a dirtyChanged signal on the object
        if (_objectList[i]->metaObject()->indexOfSignal(QMetaObject::normalizedSignature("dirtyChanged(bool)")) != -1) {
            if (!_skipDirtyFirstItem || i != 0) {
                QObject::disconnect(_objectList[i], SIGNAL(dirtyChanged(bool)), this, SLOT(_childDirtyChanged(bool)));
            }
        }
    }
    removeRows(i, 1);

    setDirty(true);
    return removedObject;
}

bool QmlObjectTableModel::upMove(int index)
{
    beginMoveRows(QModelIndex(),index,index,QModelIndex(),index-1);
    _objectList.swap(index,index-1);
    endMoveRows();
    emit layoutChanged();
    return true;
}

bool QmlObjectTableModel::downMove(int index)
{
    beginMoveRows(QModelIndex(),index+1,index+1,QModelIndex(),index);
    _objectList.swap(index+1,index);
    endMoveRows();
    emit layoutChanged();
    return true;
}


void QmlObjectTableModel::insert(int i, QObject* object)
{

    if (i < 0 || i > _objectList.count()) {
        qWarning() << "Invalid index index:count" << i << _objectList.count();
    }

    QQmlEngine::setObjectOwnership(object, QQmlEngine::CppOwnership);

    // Look for a dirtyChanged signal on the object
    if (object->metaObject()->indexOfSignal(QMetaObject::normalizedSignature("dirtyChanged(bool)")) != -1) {
        if (!_skipDirtyFirstItem || i != 0) {
            QObject::connect(object, SIGNAL(dirtyChanged(bool)), this, SLOT(_childDirtyChanged(bool)));
        }
    }

    _objectList.insert(i,object);

    qDebug() << "insert ------------object--------" << endl;
    insertRows(i, 1);
    setDirty(true);

}

void QmlObjectTableModel::insert(int i, QList<QObject*> objects)
{
    if (i < 0 || i > _objectList.count()) {
        qWarning() << "Invalid index index:count" << i << _objectList.count();
    }

    int j = i;
    for (QObject* object: objects) {
        QQmlEngine::setObjectOwnership(object, QQmlEngine::CppOwnership);

        // Look for a dirtyChanged signal on the object
        if (object->metaObject()->indexOfSignal(QMetaObject::normalizedSignature("dirtyChanged(bool)")) != -1) {
            if (!_skipDirtyFirstItem || j != 0) {
                QObject::connect(object, SIGNAL(dirtyChanged(bool)), this, SLOT(_childDirtyChanged(bool)));
            }
        }
        j++;

        _objectList.insert(j, object);
    }

    insertRows(i, objects.count());

    setDirty(true);
}


void QmlObjectTableModel::append(QObject* object)
{
    insert(_objectList.count(), object);
}


void QmlObjectTableModel::append(QList<QObject*> objects)
{
    insert(_objectList.count(), objects);
}


QObjectList QmlObjectTableModel::swapObjectList(const QObjectList& newlist)
{
    QObjectList oldlist(_objectList);
    beginResetModel();
    _objectList = newlist;
    endResetModel();
    emit countChanged(count());
    return oldlist;
}

int QmlObjectTableModel::count() const
{
    return rowCount();
}

void QmlObjectTableModel::setDirty(bool dirty)
{
    if (_dirty != dirty) {
        _dirty = dirty;
        if (!dirty) {
            // Need to clear dirty from all children
            for(QObject* object: _objectList) {
                if (object->property("dirty").isValid()) {
                    object->setProperty("dirty", false);
                }
            }
        }
        emit dirtyChanged(_dirty);
    }
}

void QmlObjectTableModel::_childDirtyChanged(bool dirty)
{
    _dirty |= dirty;
    // We want to emit dirtyChanged even if the actual value of _dirty didn't change. It can be a useful
    // signal to know when a child has changed dirty state
    emit dirtyChanged(_dirty);
}

void QmlObjectTableModel::deleteListAndContents()
{
    for (int i=0; i<_objectList.count(); i++) {
        _objectList[i]->deleteLater();
    }
    deleteLater();
}


void QmlObjectTableModel::clearAndDeleteContents()
{
    beginResetModel();
    for (int i=0; i<_objectList.count(); i++) {
        _objectList[i]->deleteLater();
    }
    qDebug() << "remove --------clearAndDeleteContents " << "   " << rowCount() << endl;
    clear();
    endResetModel();
}



Qt::ItemFlags QmlObjectTableModel::flags( const QModelIndex &index ) const
{
    if(!index.isValid())
        return 0;


    if (index.column() == 0)
        return Qt::ItemIsEnabled | Qt::ItemIsSelectable ;


    Qt::ItemFlags flags = Qt::ItemIsEnabled | Qt::ItemIsSelectable;

    return flags;
}


QVariant QmlObjectTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(role==Qt::DisplayRole && orientation==Qt::Horizontal)
        return header[section];
    return QAbstractTableModel::headerData(section,orientation,role);
}

void QmlObjectTableModel::refrushModel()
{
    beginResetModel();
    endResetModel();

    emit updateCount(this->rowCount(QModelIndex()));
}


void QmlObjectTableModel::addPointToVehicle(int seq)
{
    rowCheckStateMap[seq] = Qt::Checked;
    refrushModel();
}
