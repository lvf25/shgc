#include "pushbuttonitemdelegate.h"
#include <QSpinBox>
#include <QDebug>
#include <QPushButton>
#include <QPainter>
#include <QHBoxLayout>

CPushButtonItemDelegate::CPushButtonItemDelegate(QIcon _icon,QObject *parent)
    :QPersistentStyledItemDelegate(parent)
{
    g_Icon = _icon;
}


QWidget *CPushButtonItemDelegate::createEditor(QWidget *parent,const QStyleOptionViewItem &/* option */,const QModelIndex &index) const
{
    QPersistentModelIndex perIndex(index);

    QWidget *widget = new QWidget(parent);
    widget->setAutoFillBackground(true);
    QGridLayout *layout = new QGridLayout;
    layout->setMargin(2);
//    layout->addStretch();

    QPushButton *btn = new QPushButton(g_Icon,"");
    btn->setIconSize(QSize(48, 48));
    layout->addWidget(btn);

    btn->setStyleSheet("QPushButton{outline:0px;\
                       border-width:0;}");

    widget->setLayout(layout);

    QObject::connect(btn,&QPushButton::clicked,[=]{
        QModelIndex tIndex = static_cast<QModelIndex>(perIndex);
        //const成员里，不能修改对象，因此不能emit信号
        auto temp = const_cast<CPushButtonItemDelegate *>(this);
        emit temp->BtnClicked(tIndex);
    });

    return widget;
}

