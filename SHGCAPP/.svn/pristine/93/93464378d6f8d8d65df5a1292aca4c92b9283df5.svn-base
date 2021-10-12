#ifndef WATERLEVELGAUGELINK_H
#define WATERLEVELGAUGELINK_H

#include <QObject>
#include <QThread>
#include <QtSerialPort/QSerialPort>
#include <QTimer>

#include "QGCToolbox.h"
#include "LinkConfiguration.h"
#include "SerialLink.h"

class WaterLevelGaugeLink : public QObject
{
    Q_OBJECT
public:
    explicit WaterLevelGaugeLink(QObject *parent = nullptr);
    ~WaterLevelGaugeLink();

    static WaterLevelGaugeLink* instance(void);
    void deleteInstance(void);
    static WaterLevelGaugeLink* _create();

    void createConnectedLink(SharedLinkConfigurationPointer& config, bool isPX4Flow);
    void disconnectLink();

    SerialLink* pLink = nullptr;

    QString g_port = "";
    QString g_baudRate = "";

    bool postStatus = false;
    void StartCom();

    QSerialPort *g_mserial = new QSerialPort();

    QString HEXsdStr = "";
    QString HEXrdStr = "";

    QByteArray alone_waterLevelGaugeBuf;//测量结果
    QString str_alone_waterLevelGaugeBuf = "";//测量结果

    QString tempStr_alone_waterLevelGaugeBuf_1 = "";//临时存储 第一段测量结果
    QString tempStr_alone_waterLevelGaugeBuf_2 = "";//临时存储 第二段测量结果
    QString str_tempResults = "";//临时测量结果
    QString str_Results = "";//合并后的测量结果

    int i = 0;
    bool resendStates = false;//是否重复发送
    int time = 1;//默认重复发送频率为1秒
    int timeStates = 0;
//    bool timeStates_Alone = false;
    QTimer *qtimer = new QTimer(this);
//    QTimer *_qtimer = new QTimer(this);
//    QTimer *qtimer_s;
    void initqTimer();

    void handle_queryResults();
    void mydisconnect(void);

    bool FV_measureStates = false;//测速仪 重复测量是否进行中
    bool timeNotReceived = false;//超时3秒 有没有收到数据

    bool ib_recStatus = false;
    int  i_interval = 0;
    QByteArray ib_recbuffer;

    int timeTemp = 0;
    bool intervalVelocimetryStates = false;

signals:
    void openSuccess(QString);
    void openFail(QString);
    void startRead(QSerialPort *);

    void connectWLG_pushButtonUnableToClick();
    void connectWLG_pushButtonClickable();

    void WLG_sendMeasureStates(bool);//测量状态 是否启动测量

    void WLG_transferMeasurementResults(QString);//传递测量结果

    void recoveryFV_CanBeMeasured();

public slots:
    void changePort(QString);//改变串口号
    void changeBaudRate(QString);//改变波特率
    void connectSerialPort();//连接串口
    void single_sendInstructions();//单次发送测量指令

    void readMyCom();

    void runqTimer();

    void noMeasurement_WLG(bool);
//    void setTimeNotReceivedStates();

    void resetQTimer_startIntervalTransmission(QString);
    void singleMeasurement_cancelIntervalTransmission();

    void stopWaterLevelMeasurement();

    void recoveryWLGCanBeMeasured();

//    void suspendMeasurementTime_WLG();
//    void suspendTimer();
};

#endif  //WATERLEVELGAUGELINK_H
