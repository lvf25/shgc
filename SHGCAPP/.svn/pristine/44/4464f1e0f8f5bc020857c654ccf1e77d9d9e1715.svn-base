#ifndef POWERDISPLAY_H
#define POWERDISPLAY_H

#include <QWidget>

#ifdef quc
#if (QT_VERSION < QT_VERSION_CHECK(5,7,0))
#include <QtDesigner/QDesignerExportWidget>
#else
#include <QtUiPlugin/QDesignerExportWidget>
#endif

class QDESIGNER_WIDGET_EXPORT PowerDisplay : public QWidget
#else
class PowerDisplay : public QWidget
#endif

{
    Q_OBJECT

    Q_PROPERTY(double value READ getValue WRITE setValue)

public:
    explicit PowerDisplay(QWidget *parent = nullptr);

protected:
    void paintEvent(QPaintEvent *);
    void drawBg(QPainter *painter);
    void drawBarBg(QPainter *painter);
    void drawBar(QPainter *painter);
    void drawNum(QPainter *painter);

private:
    double minValue;                //最小值
    double maxValue;                //最大值
    double value;                   //目标值
    int precision;                  //精确度,小数点后几位

    int space;                      //间距

    QColor bgColorStart;            //背景渐变开始颜色
    QColor bgColorEnd;              //背景渐变结束颜色

    QColor textColor;               //文本颜色

    QColor barBgColor;              //柱状背景色
    QColor LessThan20_barColor;     //柱状颜色-电量小于20%、大于0%
    QColor LessThan60_barColor;     //柱状颜色-电量小于等于59%、大于等于20%
    QColor LessThan100_barColor;    //柱状颜色-电量小于等于100%、大于等于60%

    bool reverse;                   //是否倒退
    double currentValue;            //当前值
    QRectF barRect;                 //柱状图区域

public:
    double getValue()               const;

public Q_SLOTS:
    //设置目标值
    void setValue(double value);
    void setValue(int value);

Q_SIGNALS:
    void valueChanged(double value);

};

#endif // POWERDISPLAY_H
