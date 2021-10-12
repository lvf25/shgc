#include "flightdashboard.h"
#include <qpainter.h>
#include <qmath.h>
#include <qtimer.h>
#include <QDebug>
#include <QGraphicsScene>
#include <QGraphicsView>

FlightDashboard::FlightDashboard(QWidget *parent) : QWidget(parent)
{
    initqTimer();//初始化定时器

    //刷新控件，会调用paintEvent函数
    connect(FlightDashboard::qtimer, SIGNAL(timeout()), this, SLOT(UpdateAngle()));
    qtimer->start(100);

    left_maxValue = 60;
    minValue = 0;
    right_maxValue = 60;
    value = 0;
    precision = 0;

    compassValue = 0;

    scaleMajor = 12;
    scaleMinor = 2;
    startAngle = 120;
    endAngle = 120;

    crownColorStart = QColor(1,75,116);
    crownColorEnd = QColor(1,75,116);

    textColor = QColor(255, 255, 255);

    testNColor = QColor(3, 168, 247);

    scaleDialColor = QColor(255, 255, 255);
    scaleDialColorNum = QColor(0, 120, 177);

    pointerColor = QColor(0, 0, 0);

    reverse = false;

    planeLineColor = QColor(255, 255, 255);
    planeLineTextColorOn = QColor(0, 120, 177);
    planeLineTextColorDown = QColor(87, 61, 53);
    handleColor = QColor(51, 51, 51);

    degValue = 0;
    rollValue = 0;

    wOrnDialScale = true;
    wOrnPlaneLine = true;

}

FlightDashboard::~FlightDashboard()
{
    if (qtimer->isActive())
    {
        qtimer->stop();
        delete qtimer;
        qtimer = nullptr;
    }
}


void FlightDashboard::paintEvent(QPaintEvent *)
{
    int width = this->width();
    int height = this->height();
    int side = qMin(width, height);

    //绘制准备工作,启用反锯齿,平移坐标轴中心,等比例缩放
    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
    painter.translate(width / 2, height / 2);
    painter.scale(side / 200.0, side / 200.0);

    //绘制外边框"指南针仪表盘"
    drawCompassCircle(&painter);
    //绘制刻度
    drawCompassScale(&painter);
    //绘制数字
    drawCompassScaleValue(&painter);
    //绘制东南西北标识
    drawCompassScaleNum(&painter);

    //绘制覆盖圆内圆"刻度盘"背景
    drawDialCircle(&painter);
    if(wOrnDialScale)
    {
        //绘制刻度值
        drawDialScaleNum(&painter);
        //绘制刻度线
        drawDialScale(&painter);
        //绘制圆弧
        drawCircularArc(&painter);
    }

    //绘制三角指针
    drawPointerTriangle(&painter);
    if(wOrnPlaneLine)
    {
        //绘制线条
        drawPlaneLine(&painter);
    }
    //绘制手柄
    drawHandle(&painter);
}

void FlightDashboard::drawCompassCircle(QPainter *painter)
{
    int radius = 95;

    painter->save();

    painter->setPen(Qt::NoPen);
    QLinearGradient lineGradient(0, -radius, 0, radius);
    lineGradient.setColorAt(0, crownColorStart);
    lineGradient.setColorAt(1, crownColorEnd);
    painter->setBrush(lineGradient);
    painter->drawEllipse(-radius, -radius, radius * 2, radius * 2);

    painter->restore();
}

void FlightDashboard::drawCompassScale(QPainter *painter)
{
    int radius = 95;

    painter->save();

    //总共8格,4格为NESW字母,4格为数字
    int steps = 8;
    double angleStep = 360.0 / steps;

    QPen pen;
    pen.setColor(textColor);
    pen.setCapStyle(Qt::RoundCap);
    pen.setWidth(0);
    painter->setPen(pen);

    int rotateNumber = 360 - this -> compassValue;
    if (rotateNumber > 360||rotateNumber < 0)
    {
        rotateNumber = 0;
    }

    //绘制线条刻度
    for (int i = 0; i <= steps; i++) {
        painter->drawLine(0, radius - 3, 0, radius);
        painter->rotate(angleStep);
    }

    painter->restore();
}

void FlightDashboard::drawCompassScaleValue(QPainter *painter)
{
    int radius = 92;
    painter->save();
    painter->setPen(textColor);

    //总共8格,4格为NESW字母,4格为数字
    int steps = 8;
    double angleStep = 360.0 / steps;

    QFont font;
    font.setPixelSize(12);
    font.setFamily("Microsoft YaHei");
    font.setBold(false);
    painter->setFont(font);

    int rotateNumber = 360 - this -> compassValue;
    if (rotateNumber > 360||rotateNumber < 0)
    {
        rotateNumber = 0;
    }

    //绘制数字
    for(int i=0;i<steps;i++)
    {
        painter->rotate(angleStep);
        QRect textRect = QRect(-radius, -radius, radius * 2, radius * 2);
        if(i == 0)
        {
            if(compassValue > 32 && compassValue < 58)
            {
                painter->drawText(textRect, Qt::AlignTop | Qt::AlignHCenter, "");
            }
            else
            {
                painter->drawText(textRect, Qt::AlignTop | Qt::AlignHCenter, QString::number(45));
            }
        }

        if(i == 2)
        {
            if(compassValue > 122 && compassValue < 148)
            {
                painter->drawText(textRect, Qt::AlignTop | Qt::AlignHCenter, "");
            }
            else
            {
                painter->drawText(textRect, Qt::AlignTop | Qt::AlignHCenter, QString::number(135));
            }
        }

        if(i == 4)
        {
            if(compassValue > 212 && compassValue < 238)
            {
                painter->drawText(textRect, Qt::AlignTop | Qt::AlignHCenter, "");
            }
            else
            {
                painter->drawText(textRect, Qt::AlignTop | Qt::AlignHCenter, QString::number(225));
            }
        }

        if(i == 6)
        {
            if(compassValue > 302 && compassValue < 328)
            {
                painter->drawText(textRect, Qt::AlignTop | Qt::AlignHCenter, "");
            }
            else
            {
                painter->drawText(textRect, Qt::AlignTop | Qt::AlignHCenter, QString::number(315));
            }
        }
    }

    painter->restore();
}

void FlightDashboard::drawCompassScaleNum(QPainter *painter)
{
    int radius = 92;
    painter->save();
    painter->setPen(textColor);

    QFont font;
    font.setPixelSize(12);
    font.setFamily("Microsoft YaHei");
    font.setBold(false);
    painter->setFont(font);

    //总共8格,4格为NESW字母,4格为数字
    int steps = 8;
    double angleStep = 360.0 / steps;

    int rotateNumber = 0 + this -> compassValue;
    if (rotateNumber > 360||rotateNumber < 0)
    {
        rotateNumber = 0;
    }

    QRect textRect = QRect(-radius, -radius, radius * 2, radius * 2);

    //绘制字母
    for(int i=0;i<=steps;i++)
    {
        painter->rotate(angleStep);
        if(i == 1)
        {
            if(compassValue > 77 && compassValue < 103)
            {
                painter->drawText(textRect,  Qt::AlignHCenter, "");
            }
            else
            {
                painter->drawText(textRect,  Qt::AlignHCenter, "E");
            }


        }else if(i == 3)
        {
            if(compassValue > 167 && compassValue < 193)
            {
                painter->drawText(textRect, Qt::AlignHCenter, "");
            }
            else
            {
                painter->drawText(textRect, Qt::AlignHCenter, "S");
            }

        }
        else if(i == 5)
        {
            if(compassValue > 257 && compassValue < 283)
            {
                painter->drawText(textRect,  Qt::AlignHCenter, "");
            }
            else
            {
                painter->drawText(textRect,  Qt::AlignHCenter, "W");
            }

        }
        else if(i == 7)
        {
            if(compassValue > 347 && compassValue < 13)
            {
                painter->drawText(textRect,  Qt::AlignHCenter, "");
            }
            else
            {
                painter->drawText(textRect, Qt::AlignHCenter, "N");
            }
        }
    }

    painter->restore();

    painter->save();
    painter->rotate(rotateNumber);

    QImage imag(":/images/ui/ellipse_N.png");
    int imageWidth = imag.width();
    int imageHeight = imag.height();
    QRect Rect = QRect(-(imageWidth / 5 - 3),-(96 + imageHeight / 12),imageWidth / 3,imageHeight / 3);
    QRect imageRect = QRect(0,0,imageWidth,imageHeight);
    painter->drawImage(Rect,imag,imageRect);

    //设置画笔颜色
    QPen pen;
    pen.setColor(testNColor);
    painter->setPen(pen);
    textRect = QRect(-(radius - 1), -(radius + imageHeight / 12), radius * 2, radius * 2);

    //重新设置字体
    font.setPixelSize(12);
    font.setFamily("Microsoft YaHei");
    font.setBold(false);
    painter->setFont(font);

    if(compassValue == 0 || compassValue == 360)
    {
        painter->drawText(textRect, Qt::AlignHCenter, "N");
    }
    else if(compassValue == 90)
    {
        painter->drawText(textRect, Qt::AlignHCenter, "E");
    }
    else if(compassValue == 180)
    {
        painter->drawText(textRect, Qt::AlignHCenter, "S");
    }
    else if(compassValue == 270)
    {
        painter->drawText(textRect, Qt::AlignHCenter, "W");
    }
    else
    {
        painter->drawText(textRect, Qt::AlignHCenter,  QString::number(compassValue,10,0));
    }

    painter->restore();
}

void FlightDashboard::drawDialCircle(QPainter *painter)
{
    int radius = 76;

    painter->save();                         //内环圆的半径是76
    painter->rotate(360 + degValue);                  //坐标系转动横滚角度

    float pitch = rollValue;

    //上半圆
    painter->setBrush(QColor(17,173,248));
    const QRect rectangle = QRect(-radius, -radius,150,150);
    painter->drawChord(rectangle, 16*180/3.14159*asin(1.5*pitch/radius), 16*(180-2*180/3.14159*asin(1.5*pitch/radius)));

    //下半圆
    painter->setBrush(QColor(1,150,7));

    QRect rectangle2;
    if(rollValue <= -50)
    {
        rectangle2 = QRect(radius, radius,150,150);
    }
    else
    {
        rectangle2 = QRect(-radius, -radius,150,150);
    }
    painter->drawChord(rectangle2, 16*(-180-180/3.14159*asin(1.5*pitch/radius)), 16*(180+2*180/3.14159*asin(1.5*pitch/radius)));

    painter->restore();
}

void FlightDashboard::drawDialScaleNum(QPainter *painter)
{
    int radius = 66;
    painter->save();
    painter->setPen(scaleDialColorNum);

    //设置画笔字体
    QFont font;
    font.setFamily("Microsoft YaHei");
    font.setPixelSize(8);
    font.setBold(true);
    painter->setFont(font);

    double startRad = ((360 - startAngle - 90 - 60) * (M_PI / 180));
    double deltaRad = ((360 - startAngle - endAngle - 60) * (M_PI / 180) / scaleMajor);

    for (int i = -12; i <= 0 ; i ++)
    {
        if(i >= -6)
        {
            double sina = qSin(startRad - i * deltaRad);
            double cosa = qCos(startRad - i * deltaRad);
            double value = 1.0 * i * ((left_maxValue - minValue) / scaleMajor) + minValue;

            int valueA = qAbs(value);

            QString strValue = QString("%1").arg((double)valueA, 0, 'f', precision);
            double textWidth = fontMetrics().width(strValue);
            double textHeight = fontMetrics().height();
            int x = radius * cosa - textWidth / 2;
            int y = -radius * sina + textHeight / 4;

            painter->drawText(x, y, strValue);

            i++;
        }
        else  if(i == -9 || i == -12)
        {
            double sina = qSin(startRad - i * deltaRad);
            double cosa = qCos(startRad - i * deltaRad);
            double value = 1.0 * i * ((left_maxValue - minValue) / scaleMajor) + minValue;

            int valueA = qAbs(value);

            QString strValue = QString("%1").arg((double)valueA, 0, 'f', precision);
            double textWidth = fontMetrics().width(strValue);
            double textHeight = fontMetrics().height();
            int x = radius * cosa - textWidth / 2;
            int y = -radius * sina + textHeight / 4;

            painter->drawText(x, y, strValue);
        }

    }

    for (int i = 0; i <= 12; i++)
    {
        if(i<=6)
        {
            double sina = qSin(startRad - i * deltaRad);
            double cosa = qCos(startRad - i * deltaRad);
            double value = 1.0 * i * ((left_maxValue - minValue) / scaleMajor) + minValue;

            QString strValue = QString("%1").arg((double)value, 0, 'f', precision);
            double textWidth = fontMetrics().width(strValue);
            double textHeight = fontMetrics().height();
            int x = radius * cosa - textWidth / 2;
            int y = -radius * sina + textHeight / 4;

            painter->drawText(x, y, strValue);

            i++;
        }
        else if(i == 9 || i == 12)
        {
            double sina = qSin(startRad - i * deltaRad);
            double cosa = qCos(startRad - i * deltaRad);
            double value = 1.0 * i * ((left_maxValue - minValue) / scaleMajor) + minValue;

            QString strValue = QString("%1").arg((double)value, 0, 'f', precision);
            double textWidth = fontMetrics().width(strValue);
            double textHeight = fontMetrics().height();
            int x = radius * cosa - textWidth / 2;
            int y = -radius * sina + textHeight / 4;

            painter->drawText(x, y, strValue);
        }
    }
    painter->restore();
}

void FlightDashboard::drawDialScale(QPainter *painter)
{
    int radius = 56;
    painter->save();

    painter->rotate(startAngle);
    int steps = (scaleMajor * scaleMinor);
    double angleStep = (360.0 - startAngle - endAngle) / steps;

    QPen pen;
    pen.setColor(scaleDialColor);
    pen.setCapStyle(Qt::RoundCap);

    for (int i = 0; i <= steps; i++) {
        if ((i % scaleMinor == 0) && (i != 2 && i != 4 && i != 20 && i != 22))
        {
            pen.setWidthF(1);
            painter->setPen(pen);
            painter->drawLine(0, radius - 3, 0, radius);
        }
        else if(i == 3 || i == 21 )
        {
            pen.setWidthF(1);
            painter->setPen(pen);
            painter->drawLine(0, radius - 3, 0, radius);
        }

        painter->rotate(angleStep);
    }

    painter->restore();
}

void FlightDashboard::drawCircularArc(QPainter *painter)
{
    int radius = 51;
    painter->save();
    painter->setPen(QPen(Qt::white,2,Qt::SolidLine));

    QRectF rect(-radius, -radius, radius * 2, radius * 2);

    //计算总范围角度
    double angleAll = 360.0 - startAngle - endAngle;

    //绘制圆弧
    painter->drawArc(rect,(270 - startAngle - angleAll) * 16, angleAll * 16);

    painter->restore();
}

void FlightDashboard::drawPointerTriangle(QPainter *painter)
{
    int radius = 49;
    painter->save();
    painter->rotate(360 + degValue);
    painter->setPen(Qt::NoPen);

    //指针长度
    int len = 6;
    //三角形坐标
    QPolygon pts;
    pts.setPoints(3, 0, -radius, -len / 2, -radius + len, len / 2, -radius + len);

    painter->setBrush(pointerColor);
    painter->drawConvexPolygon(pts);

    painter->restore();
}

void FlightDashboard:: drawPlaneLine(QPainter *painter)
{
    double radius = 56;
    painter->save();

    int width = this->width();
    int height = this->height();
    int side = qMin(width, height);

    //重置坐标系
    painter->resetTransform();
    painter->translate(width / 2, height / 2 + rollValue * 2.1);    //? 2.1
    painter->scale(side / 100.0, side / 100.0);


    painter->rotate(360 + degValue);

    //设置画笔颜色
    QPen pen;
    pen.setColor(planeLineColor);
    pen.setWidthF(0.8);
    pen.setCapStyle(Qt::RoundCap);
    painter->setPen(pen);

    //设置画笔字体
    QFont font;
    font.setFamily("Microsoft YaHei");
    font.setPixelSize(5);
    font.setBold(true);
    painter->setFont(font);

    //依次绘制水平距离线条及值
    for (int i = -20; i <= 25; i += 5)
    {
        int value = qAbs(i);
        //绘制中心点
        if(i == 0)
        {
            //绘制横向水平线条  长条
            QPointF pt1(-radius * 14 / 100, radius / 75 * i);
            QPointF pt2(-pt1.x(),pt1.y());
            painter->drawLine(pt1,pt2);

            //根据字体大小计算绘制文字的区域 并根据i的值设置文字的值的正负
            QString strValue = QString::number(value);
            QFontMetrics fm = painter->fontMetrics();
            QRectF textRect(QPointF(0, 0), fm.size(Qt::AlignCenter, strValue));

            //绘制文字之前先改变画笔颜色
            pen.setColor(planeLineTextColorOn);
            painter->setPen(pen);

            //绘制文字
            textRect.moveCenter(pt1 - QPointF(radius / 15, 0));
            painter->drawText(textRect, Qt::AlignCenter, strValue);
        }
        else if(i >= -20 && i < 0)
        {
            if(i % 2 == 0)
            {
                //绘制水平线条之前先改变画笔颜色
                pen.setColor(planeLineColor);
                painter->setPen(pen);

                //绘制横向水平线条  长条
                QPointF pt1(-radius * 14 / 100, radius / 75 * i);
                QPointF pt2(-pt1.x(),pt1.y());
                painter->drawLine(pt1,pt2);

                //根据字体大小计算绘制文字的区域  并根据i的值设置文字的值的正负
                QString strValue = QString::number(value);
                QFontMetrics fm = painter->fontMetrics();
                QRectF textRect(QPointF(0, 0), fm.size(Qt::AlignCenter, strValue));

                //绘制文字之前先改变画笔颜色
                pen.setColor(planeLineTextColorOn);
                painter->setPen(pen);

                textRect.moveCenter(pt1 - QPointF(radius / 15, 0));
                painter->drawText(textRect, Qt::AlignCenter, strValue);
            }
            else
            {
                //绘制水平线条之前先改变画笔颜色
                pen.setColor(planeLineColor);
                painter->setPen(pen);

                //绘制横向水平线条  短条
                QPointF pt1(-radius * 7 / 100, radius / 75 * i);
                QPointF pt2(-pt1.x(),pt1.y());
                painter->drawLine(pt1,pt2);
            }

        }
        else if(i > 0 && i <= 25)
        {
            if(i % 2 == 0)
            {
                //绘制水平线条之前先改变画笔颜色
                pen.setColor(planeLineColor);
                painter->setPen(pen);

                //绘制横向水平线条  长条
                QPointF pt1(-radius * 14 / 100, radius / 75 * i);
                QPointF pt2(-pt1.x(),pt1.y());
                painter->drawLine(pt1,pt2);

                //根据字体大小计算绘制文字的区域  并根据i的值设置文字的值的正负
                QString strValue = QString::number(-value);
                QFontMetrics fm = painter->fontMetrics();
                QRectF textRect(QPointF(0, 0), fm.size(Qt::AlignCenter, strValue));

                //绘制文字之前先改变画笔颜色
                pen.setColor(planeLineTextColorDown);
                painter->setPen(pen);

                textRect.moveCenter(pt1 - QPointF(radius / 15, 0));
                painter->drawText(textRect, Qt::AlignCenter, strValue);
            }
            else
            {
                //绘制水平线条之前先改变画笔颜色
                pen.setColor(planeLineColor);
                painter->setPen(pen);

                //绘制横向水平线条  短条
                QPointF pt1(-radius * 7 / 100 , radius / 75 * i);
                QPointF pt2(-pt1.x(),pt1.y());
                painter->drawLine(pt1,pt2);
            }
        }

    }

    painter->restore();
}

void FlightDashboard::drawHandle(QPainter *painter)
{
    painter->save();

    QPen pen;
    pen.setColor(handleColor);
    pen.setWidthF(1);
    pen.setCapStyle(Qt::RoundCap);
    pen.setJoinStyle(Qt::MiterJoin);
    painter->setPen(pen);
    painter->setBrush(handleColor);

    //设置画笔字体
    QFont font;
    font.setFamily("Microsoft YaHei");
    font.setPixelSize(12);
    font.setBold(false);
    painter->setFont(font);

    //绘制左侧右侧横向水平线条
    int len = 35;
    QPoint pt1 = QPoint(-len, 0);
    QPoint pt2 = QPoint(0, 0);
    painter->drawLine(pt1, pt2);

    QPoint pt3 = QPoint(len, 0);
    QPoint pt4 = QPoint(0, 0);
    painter->drawLine(pt3, pt4);

    //根据字体大小计算绘制文字的区域  并根据滚动值设置文字的值的正负
    QString strValue = QString::number(rollValue);
    QFontMetrics fm = painter->fontMetrics();
    QRectF textRect(QPointF(0, 0), fm.size(Qt::AlignCenter, strValue));

    textRect.moveCenter(pt1-QPoint(6, 0));
    painter->drawText(textRect, Qt::AlignCenter, strValue);

    //指针长度
    len = 14;
    //三角箭头坐标
    QPolygon pts;
    pts.setPoints(4, 0, 0, -len / 2, len, 0, 10, len / 2, len);
    painter->drawConvexPolygon(pts);

    painter->restore();
}

double FlightDashboard::getCompassValue() const
{
    return this->compassValue;
}

int FlightDashboard::getDegValue() const
{
    return this->degValue;
}

int FlightDashboard::getRollValue() const
{
    return this->rollValue;
}

bool FlightDashboard::getwOrnDialScale() const
{
    return this->wOrnDialScale;
}

bool FlightDashboard::getwOrnPlaneLine() const
{
    return this->wOrnPlaneLine;
}

void FlightDashboard::setCompassValue(double compassValue)
{
    if(this->compassValue != compassValue && compassValue >= 0 && compassValue <= 360){
        this->compassValue = compassValue;
        update();
    }
}

void FlightDashboard::setDegValue(int degValue)
{
    if (this->degValue != degValue && degValue >= -180 && degValue <= 180) {
        this->degValue = degValue;
        update();
    }
}

void FlightDashboard::setRollValue(int rollValue)
{
    if (this->rollValue != rollValue && rollValue >= -90 && rollValue <= 90) {
        this->rollValue = rollValue;
        update();
    }
}

void FlightDashboard::setwOrnDialScale(bool wOrnDialScale)
{

    this->wOrnDialScale = wOrnDialScale;
    update();

}

void FlightDashboard::setwOrnPlaneLine(bool wOrnPlaneLine)
{

    this->wOrnPlaneLine = wOrnPlaneLine;
    update();

}

//初始化定时器
void FlightDashboard::initqTimer()
{
    this->qtimer = new QTimer(this);
}

//刷新控件，会调用paintEvent函数
void FlightDashboard::UpdateAngle()
{
  update();
}

