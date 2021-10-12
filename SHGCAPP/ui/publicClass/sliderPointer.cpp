#include "sliderPointer.h"
#include <QPalette>

sliderPointer::sliderPointer(QWidget *parent):QSlider(parent)
{
    this->setMinimum(0);
    this->setMaximum(44);
    this->setValue(5);

    m_displayLabel=new QLabel(this);
    m_displayLabel->setFixedSize(QSize(45,20));
    m_displayLabel->setStyleSheet("font:8pt;");
//    m_displayLabel->adjustSize();
//    m_displayLabel->setScaledContents(true);
    //设置游标背景为透明
    m_displayLabel->setAutoFillBackground(true);
    m_displayLabel->setStyleSheet("color:#03A8F7;");
    QPalette palette;
    palette.setColor(QPalette::Background, QColor(255,255,255,0));
    m_displayLabel->setPalette(palette);

    m_displayLabel->setAlignment(Qt::AlignCenter);

    m_displayLabel->setVisible(true);
    m_displayLabel->move(0,-1);
}

sliderPointer::~sliderPointer()
{

}

//鼠标按下时，游标上数字显示，但数值不改变
void sliderPointer::mousePressEvent(QMouseEvent *event)
{
    if(!m_displayLabel->isVisible())
    {
        m_displayLabel->setVisible(true);
        voiceTipTime();
    }
    this->setPageStep(0);
    QSlider::mousePressEvent(event);
}

//鼠标释放时，游标上数字显示
void sliderPointer::mouseReleaseEvent(QMouseEvent *event)
{
    if(m_displayLabel->isVisible())
    {
        m_displayLabel->setVisible(true);
    }
    QSlider::mouseReleaseEvent(event);
}

//鼠标拖动时，游标上方数值随之改变
void sliderPointer::mouseMoveEvent(QMouseEvent *event)
{
    voiceTipTime();
    m_displayLabel->move((this->width()-m_displayLabel->width()*5/4)*this->value()/(this->maximum()-this->minimum()),-1);
    QSlider::mouseMoveEvent(event);
}

void sliderPointer::voiceTipTime()
{
    m_displayLabel->setAlignment(Qt::AlignCenter);
    if(this->value() <= 5){
        m_displayLabel->setText(QString::number(this->value()+5)+"s");
    }else if(this->value() == 6)
    {
        m_displayLabel->setText(QString::number(15)+"s");
    }else if(this->value() == 7)
    {
        m_displayLabel->setText(QString::number(20)+"s");
    }else if(this->value() == 8)
    {
        m_displayLabel->setText(QString::number(25)+"s");
    }else if(this->value() == 9)
    {
        m_displayLabel->setText(QString::number(30)+"s");
    }else if(this->value() == 10)
    {
        m_displayLabel->setText(QString::number(35)+"s");
    }else if(this->value() == 11)
    {
        m_displayLabel->setText(QString::number(40)+"s");
    }else if(this->value() == 12)
    {
        m_displayLabel->setText(QString::number(45)+"s");
    }else if(this->value() == 13)
    {
        m_displayLabel->setText(QString::number(50)+"s");
    }else if(this->value() == 14)
    {
        m_displayLabel->setText(QString::number(55)+"s");
    }else if(this->value() >= 15 && this->value() <45)
    {
        m_displayLabel->setText(QString::number(this->value()-14)+"min");
    }
}

void sliderPointer::wheelEvent(QWheelEvent * event)
{
    //滚轮操作不做任何处理
}


