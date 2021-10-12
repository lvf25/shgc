#ifndef CPUSHBUTTONITEMDELEGATE_H
#define CPUSHBUTTONITEMDELEGATE_H
#include "QPersistentStyledItemDelegate.h"


class QPushButton;

class CPushButtonItemDelegate :public QPersistentStyledItemDelegate
{
    Q_OBJECT
public:
    CPushButtonItemDelegate(QIcon _icon,QObject *parent = Q_NULLPTR);

    //因为按钮既不需要从model读取数据，也不需要写入，因此仅需要重写一个函数即可
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
signals:
    void BtnClicked(QModelIndex index);

private:
    QIcon g_Icon;
};

#endif // CPUSHBUTTONITEMDELEGATE_H
