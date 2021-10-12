#pragma execution_character_set("utf-8")

#include "powerdisplay.h"
#include <QPainter>

PowerDisplay::PowerDisplay(QWidget *parent) : QWidget(parent)
{
    minValue = 0;
    maxValue = 100;
    value = 0;
    precision = 0;

    space = 0;

    textColor = QColor(255, 255, 255);

    bgColorStart = QColor(100, 100, 100);
    bgColorEnd = QColor(60, 60, 60);

    barBgColor = QColor(40, 40, 40);
    LessThan20_barColor = QColor(255, 0, 0);
    LessThan60_barColor = QColor(254,87,35);
    LessThan100_barColor = QColor(1,156,7);

    currentValue = 0;
}

void PowerDisplay::paintEvent(QPaintEvent *)
{
    //绘制准备工作,启用反锯齿
    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);

    //绘制背景
    drawBg(&painter);
    //绘制柱状背景
    drawBarBg(&painter);
    //绘制柱状
    drawBar(&painter);
    //绘制百分比数字
    drawNum(&painter);
}

void PowerDisplay::drawBg(QPainter *painter)
{
    painter->save();
    painter->setPen(Qt::NoPen);
    QLinearGradient bgGradient(QPointF(0, 0), QPointF(width(), 0));
    bgGradient.setColorAt(0.0, bgColorStart);
    bgGradient.setColorAt(1.0, bgColorEnd);
    painter->setBrush(bgGradient);
    painter->drawRect(rect());
    painter->restore();
}


void PowerDisplay::drawBarBg(QPainter *painter)
{
    painter->save();
    painter->setPen(Qt::NoPen);

    QPointF topLeftPot(space, space);
    QPointF bottomRightPot(width() - space, height() - space);
    barRect = QRectF(topLeftPot, bottomRightPot);
    painter->setBrush(barBgColor);
    painter->drawRect(barRect);
    painter->restore();
}

void PowerDisplay::drawBar(QPainter *painter)
{
    painter->save();
    painter->setPen(Qt::NoPen);


    double barwidth = barRect.width();
    double increment = (double)barwidth / (maxValue - minValue);
    double initx = (currentValue - minValue) * increment;

    QPointF topLeftPot(barRect.topLeft());
    QPointF bottomRightPot(initx, barRect.bottomRight().y());
    QRectF currentRect(topLeftPot, bottomRightPot);

    QPointF repair_bottomRightPot(initx - 20, barRect.bottomRight().y());
    QRectF repair_currentRect(topLeftPot, repair_bottomRightPot);

    if(currentValue > 0&&currentValue <= 19)
    {
        painter->setBrush(LessThan20_barColor);
    }
    else if(currentValue >= 20&&currentValue <= 59)
    {
        painter->setBrush(LessThan60_barColor);
    }
    else if(currentValue >= 60&&currentValue <= 100)
    {
        painter->setBrush(LessThan100_barColor);
    }

    painter->drawRoundedRect(currentRect, 10, 10);
    painter->drawRect(repair_currentRect);//修复左边的圆角
    painter->restore();
}

void PowerDisplay::drawNum(QPainter *painter)//显示大小、位置需要微调
{
    painter->save();

    //自动计算文字字体大小
    QFont font;
    font.setPixelSize((width() / 12) * 0.1);
    painter->setFont(font);

    //将电量值转换为QString,并设置绘制坐标
    double inc = (double)(currentValue - minValue) * 100 / (maxValue - minValue);
    QString strValue = QString("%1%").arg(inc, 0, 'f', precision);

    double barwidth = barRect.width();
    double increment = (double)barwidth / (maxValue - minValue);
    double initx = (currentValue - minValue) * increment;

    QPointF topLeftPot(barRect.topLeft().x(),barRect.topLeft().y() - 3);
    QPointF bottomRightPot(initx - 10, barRect.bottomRight().y());
    QRectF textRect(topLeftPot, bottomRightPot);

    //绘制电量值
    if(currentValue == 1)//电量为1的时候，直接显示1
    {
        painter->setPen(textColor);
        QPointF bottomRightPot(initx-5, barRect.bottomRight().y());
        QRectF textRect(topLeftPot, bottomRightPot);
        painter->drawText(textRect, Qt::AlignRight, QString("1"));

    }
    else if(currentValue > 1)//电量大于1的时候，显示百分比
    {
        painter->setPen(textColor);
        painter->drawText(textRect, Qt::AlignRight, strValue);
    }

    painter->restore();
}

double PowerDisplay::getValue() const
{
    return this->value;
}

void PowerDisplay::setValue(double value)
{
    //值小于最小值或者值大于最大值则无需处理
    if (value < minValue || value > maxValue) {
        return;
    }

    if (value > this->value) {
        reverse = false;
    } else if (value < this->value) {
        reverse = true;
    }

    this->value = value;
    emit valueChanged(value);

    currentValue = this->value;
    update();
}

void PowerDisplay::setValue(int value)
{
    setValue((double)value);
}
