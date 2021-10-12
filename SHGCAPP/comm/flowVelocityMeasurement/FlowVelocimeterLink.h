#ifndef FLOWVELOCIMETERLINK_H
#define FLOWVELOCIMETERLINK_H

#include <QDebug>
#include <QObject>
#include <QThread>
#include <QtSerialPort/QSerialPort>
#include "Vehicle.h"
#include "QHostAddress"
#include "QSerialPort"

class FlowVelocimeterLink : public QObject
{
    Q_OBJECT
public:
    explicit FlowVelocimeterLink(QObject *parent = nullptr);
    ~FlowVelocimeterLink();

    static FlowVelocimeterLink* instance(void);
    void deleteInstance(void);
    static FlowVelocimeterLink* _create();

    //字节数组转16进制字符串
    static QString ByteArrayToHexStr(QByteArray data)
    {
        QString temp="";
        QString hex=data.toHex();
        for (int i=0;i<hex.length();i=i+2)
        {
            temp+=hex.mid(i,2)+" ";
        }
        return temp.trimmed().toUpper();
    }

public:
    int i = 0;

    QString HEXsdStr = "";
    QString HEXrdStr = "";

    QString tempHEXrdStr_1 = "";//存储第一段临时数据
    QString tempHEXrdStr_2 = "";//存储第二段临时数据

    QByteArray alone_flowVelocityBuf;//测速结果

    QString str_tempResults = "";//临时存储合并后的数据
    QString str_Results = "";//合并后的数据

    QString g_port = "";
    QString g_baudRate = "";

    bool postStatus;

    QSerialPort *g_mserial = new QSerialPort();

    void mydisconnect(void);
    void StartCom();
    void closePort();

    void query_measurementMode();
    void handle_queryResults();

    bool resendStates = false;//是否重复发送
    int time = 1;//默认重复发送频率为1秒
    int timeTemp = 0;
    int timeStates = 0;
    bool intervalVelocimetryStates = false;
    QTimer *qtimer;
    void initqTimer();

    bool WLG_measureStates = false;

    bool sendButtonIsClicked = false;

signals:
    void openSuccess(QString);
    void openFail(QString);
    void startRead(QSerialPort *);
    void connectFV_pushButtonUnableToClick();
    void connectFV_pushButtonClickable();

    void FV_sendMeasureStates(bool);//测量状态 是否启动测量

    void FV_transferMeasurementResults(QString);//传递测量结果

    void changeResendStates_WLG();
    void connectWLG_pushButtonUnableToClick();

    void recoveryWLG_CanBeMeasured();

//    void suspendWaterLevelGaugeMeasurement();//暂停水位计测量 5秒

public slots:
    void changePort(QString);  //改变串口号
    void changeBaudRate(QString); //改变波特率
    void connectSerialPort();//连接串口
    void single_sendInstructions();//单次发送测速指令
    void readMyCom();

//    void resetQTimer_startIntervalTransmission(QString);
//    void singleMeasurement_cancelIntervalTransmission();

    void runqTimer();

    void noMeasurement_FV(bool);

    void recoveryFVCanBeMeasured();

//    void suspendWLGMeasurement(bool);

};

#endif // FLOWVELOCIMETERLINK_H
