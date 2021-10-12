#include "elecPointer.h"
#include <QPalette>

elecPointer::elecPointer(QWidget *parent):QSlider(parent)
{
    this->setMinimum(0);
    this->setMaximum(95);
    this->setValue(15);

    n_displayLabel=new QLabel(this);
    n_displayLabel->setFixedSize(QSize(40,20));
    n_displayLabel->setStyleSheet("font:8pt;");
//    n_displayLabel->adjustSize();
//    n_displayLabel->setScaledContents(true);
    n_displayLabel->setStyleSheet("color:#FE5723;");
    n_displayLabel->setAutoFillBackground(true);

    //设置游标背景为透明
    QPalette palette;
    palette.setColor(QPalette::Background, QColor(255,255,255,0));
    n_displayLabel->setPalette(palette);

    n_displayLabel->setAlignment(Qt::AlignCenter);

    n_displayLabel->setVisible(true);
    n_displayLabel->move(0,-1);
}

elecPointer::~elecPointer()
{

}

void elecPointer::mousePressEvent(QMouseEvent *event)
{
    if(!n_displayLabel->isVisible())
    {
        n_displayLabel->setVisible(true);
        n_displayLabel->setText(QString::number(this->value()+5)+"%");
    }
    this->setPageStep(0);
    QSlider::mousePressEvent(event);
}

void elecPointer::mouseReleaseEvent(QMouseEvent *event)
{
    if(n_displayLabel->isVisible())
    {
        n_displayLabel->setVisible(true);
    }
    QSlider::mouseReleaseEvent(event);
}

void elecPointer::mouseMoveEvent(QMouseEvent *event)
{
    n_displayLabel->setText(QString::number(this->value()+5)+"%");
    n_displayLabel->move((this->width()-n_displayLabel->width()*5/4)*this->value()/(this->maximum()-this->minimum()),-1);
    QSlider::mouseMoveEvent(event);
    //设置游标上方字体颜色
    if(this->value() < 15)
    {
        n_displayLabel->setStyleSheet("color:#FF0000;");
    }else if(this->value() >= 15 && this->value() < 55)
    {
        n_displayLabel->setStyleSheet("color:#FE5723;");
    }else{
        n_displayLabel->setStyleSheet("color:#019C07;");
    }
}

void elecPointer::wheelEvent(QWheelEvent * event)
{
    //滚轮操作不做任何处理
}

