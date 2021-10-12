#ifndef FLIGHTDASHBOARD_H
#define FLIGHTDASHBOARD_H

#include <QWidget>

#ifdef quc
#if (QT_VERSION < QT_VERSION_CHECK(5,7,0))
#include <QtDesigner/QDesignerExportWidget>
#else
#include <QtUiPlugin/QDesignerExportWidget>
#endif

class QDESIGNER_WIDGET_EXPORT FlightDashboard : public QWidget
        #else
class FlightDashboard : public QWidget
        #endif

{

    Q_PROPERTY(double compassValue READ getCompassValue WRITE setCompassValue)
    Q_PROPERTY(int degValue READ getDegValue WRITE setDegValue)
    Q_PROPERTY(int rollValue READ getRollValue WRITE setRollValue)
    Q_PROPERTY(bool wOrnDialScale READ getwOrnDialScale WRITE setwOrnDialScale)
    Q_PROPERTY(bool wOrnPlaneLine READ getwOrnPlaneLine WRITE setwOrnPlaneLine)

    Q_OBJECT
public:
    explicit FlightDashboard(QWidget *parent = nullptr);
    ~FlightDashboard();

protected:
    void paintEvent(QPaintEvent *);
    void drawCompassCircle(QPainter *painter);
    void drawCompassScale(QPainter *painter);
    void drawCompassScaleValue(QPainter *painter);
    void drawCompassScaleNum(QPainter *painter);
    void drawDialCircle(QPainter *painter);
    void drawDialScale(QPainter *painter);
    void drawCircularArc(QPainter *painter);
    void drawDialScaleNum(QPainter *painter);
    void drawPointerTriangle(QPainter *painter);
    void drawPlaneLine(QPainter *painter);
    void drawHandle(QPainter *painter);


private:
    double left_maxValue;           //左边的最大值
    double minValue;                //最小值
    double right_maxValue;          //右边的最大值
    double value;                   //目标值
    int precision;                  //精确度,小数点后几位

    double compassValue;            //当前值

    int scaleMajor;                 //大刻度数量
    int scaleMinor;                 //小刻度数量
    int startAngle;                 //开始旋转角度
    int endAngle;                   //结束旋转角度

    QColor crownColorStart;         //外边框渐变开始颜色
    QColor crownColorEnd;           //外边框渐变结束颜色

    QColor textColor;               //文字颜色

    QColor testNColor;              //指南针N字母颜色

    QColor scaleDialColor;          //刻度尺颜色
    QColor scaleDialColorNum;       //刻度尺数字颜色

    QColor pointerColor;            //指针颜色

    bool reverse;                   //是否往回走

    QColor planeLineColor;          //线条颜色
    QColor planeLineTextColorOn;    //上半部分文字颜色
    QColor planeLineTextColorDown;  //下半部分文字颜色
    QColor handleColor;             //手柄颜色

    double degValue;                   //旋转角度
    double rollValue;                  //滚动值

    bool wOrnDialScale;             //是否显示"旋转角"刻度
    bool wOrnPlaneLine;             //是否显示"滚动值"刻度

public:
    int getDegValue()               const;
    int getRollValue()              const;
    double getCompassValue()        const;

    bool getwOrnDialScale()         const;
    bool getwOrnPlaneLine()         const;

    QTimer *qtimer;
    void initqTimer();


public Q_SLOTS:
    //设置旋转角度值
    void setDegValue(int degValue);
    //设置前进旋转值
    void setRollValue(int rollValue);

    //设置"指南针仪表盘"旋转角度值
    void setCompassValue(double compassValue);
    //设置是否显示"旋转角"刻度的值
    void setwOrnDialScale(bool wOrnDialScale);
    //设置是否显示"滚动值"刻度的值
    void setwOrnPlaneLine(bool wOrnPlaneLine);

    void UpdateAngle();//刷新控件，会调用paintEvent函数

signals:

Q_SIGNALS:
    void dialValueChanged(int value);
};

#endif // FLIGHTDASHBOARD_H
