#include "FlowVelocimeterLink.h"
#include "QGCApplication.h"

#include <QFileInfo>
#include <QtEndian>

#include <QDebug>
#include <QChar>
#include <QString>
#include <algorithm>
#include <cstring>

#include "SHGCMainSetup.h"
#include "MainWindow.h"

static FlowVelocimeterLink* _instance = nullptr;

FlowVelocimeterLink* FlowVelocimeterLink::_create()
{
    _instance = new FlowVelocimeterLink();
    return _instance;
}

FlowVelocimeterLink* FlowVelocimeterLink::instance(void)
{
    return _instance;
}

void FlowVelocimeterLink::deleteInstance(void)
{
    delete this;
}

FlowVelocimeterLink::FlowVelocimeterLink(QObject *parent)
    : QObject(parent)
    , postStatus(false)
{
    initqTimer();//初始化定时器

    SHGCMainSetup *_SHGCMainSetup = SHGCMainSetup::instance();
    //设置串口号
    connect(_SHGCMainSetup,SIGNAL(sendToVMcomPort(QString)),this,SLOT(changePort(QString)),Qt::AutoConnection);
    //设置波特率
    connect(_SHGCMainSetup,SIGNAL(sendToVMcomRate(QString)),this,SLOT(changeBaudRate(QString)),Qt::AutoConnection);


    MainWindow *_MainWindow = MainWindow::instance();
    //开始连接
    connect(_MainWindow,SIGNAL(connectFV_serialPort()),this,SLOT(connectSerialPort()));
    //点击connect_pushButton后 单次发送测速指令
    connect(_MainWindow,SIGNAL(single_send_Instructions_FV()),this,SLOT(single_sendInstructions()));
    //    //若点选了checkBox，且点击了"应用"按钮 则刷新测量间隔 间隔发送查询指令
    //    connect(_MainWindow,SIGNAL(interval_Measurement_changeTime(QString)),this,SLOT(resetQTimer_startIntervalTransmission(QString)));
    //    //若不点选checkBox, 则停止定时 取消间隔发送查询指令
    //    connect(_MainWindow,SIGNAL(single_Measurement()),this,SLOT(singleMeasurement_cancelIntervalTransmission()));


    //    //关闭页面停止计时器
    //        connect(p,&QGCMain::qtimerStop,this,&QGCVMDataCom::qtimer_Stop);

    //若水位计 重复测量进行中 则停止流速计仪的测量 且流速计测量按钮不可点击
    connect(_MainWindow,SIGNAL(FV_receiveMeasureStates(bool)),this,SLOT(noMeasurement_FV(bool)));
    //若水位计 重复测量进行中 且此时流速计测速按钮被点击 则此时水位计重复测量将暂停5秒后恢复
    //    connect(_MainWindow,SIGNAL(FV_receiveMeasureStates(bool)),this,SLOT(suspendWLGMeasurement(bool)));

    //若水位计停止重复请求数据 则 恢复流速计可测量
    connect(_MainWindow,SIGNAL(recoveryFlowVelocimeterCanBeMeasured()),this,SLOT(recoveryFVCanBeMeasured()));
}

FlowVelocimeterLink::~FlowVelocimeterLink()
{

    if(g_mserial != nullptr)
    {
        mydisconnect();
    }

    _instance = nullptr;

    if (qtimer->isActive())
    {
        qtimer->stop();
        delete qtimer;
        qtimer = nullptr;
    }
}

void FlowVelocimeterLink::StartCom()
{
    if(g_mserial->isOpen())
    {
        g_mserial->clear();
        g_mserial->close();
    }
    /* 设置串口名称 */
    g_mserial->setPortName(g_port);

    /* 设置波特率 */
    if (this->g_baudRate == "1200")
    {
        g_mserial->setBaudRate(QSerialPort::Baud1200);
    }
    if (this->g_baudRate == "2400")
    {
        g_mserial->setBaudRate(QSerialPort::Baud2400);
    }
    if (this->g_baudRate == "4800")
    {
        g_mserial->setBaudRate(QSerialPort::Baud4800);
    }
    if (this->g_baudRate == "9600")
    {
        g_mserial->setBaudRate(QSerialPort::Baud9600);
    }
    if(this->g_baudRate == "19200")
    {
        g_mserial->setBaudRate(QSerialPort::Baud19200);
    }
    if (this->g_baudRate == "38400")
    {
        g_mserial->setBaudRate(QSerialPort::Baud38400);
    }
    if(this->g_baudRate == "57600")
    {
        g_mserial->setBaudRate(QSerialPort::Baud57600);
    }
    if (this->g_baudRate == "115200")
    {
        g_mserial->setBaudRate(QSerialPort::Baud115200);
    }

    /* 设置数据位数 */
    g_mserial->setDataBits(QSerialPort::Data8);

    /* 设置奇偶校验 */
    g_mserial->setParity(QSerialPort::NoParity);

    /* 设置停止位*/
    g_mserial->setStopBits(QSerialPort::OneStop);

    /* 设置流控制 */
    g_mserial->setFlowControl(QSerialPort::NoFlowControl);

    g_mserial->setReadBufferSize(1024);


    /* 打开水流测速仪串口 */
    if(g_mserial->open(QIODevice::ReadWrite)==true)
    {
        postStatus = true;
        emit openSuccess(g_port);
        emit startRead(g_mserial);
        qDebug()<< "打开水流测速仪串口成功" <<endl;
        qDebug()<< "qgcVMdataCom...ID is:" << QThread::currentThreadId();
        //收到消息 解析
        connect(g_mserial, SIGNAL(readyRead()), this, SLOT(readMyCom()));
    }
    else
    {
        postStatus = false;
        qDebug()<< "打开水流测速仪串口失败" <<endl;
        emit openFail(g_port);
    }

    //收到消息 保存
    //    connect(g_mserial, SIGNAL(readyRead()), this, SLOT(generateJSONFile()));
    //收到消息 展示
    //    connect(g_mserial, SIGNAL(readyRead()), this, SIGNAL(receivetheMessage()));

    qDebug()<<"打开 g_port:"<<this->g_port;
    qDebug()<<"打开 this->g_baudRate:"<<this->g_baudRate;

    return;
}

//关闭串口
void FlowVelocimeterLink::closePort()
{
    mydisconnect();
    postStatus = false;
}

void FlowVelocimeterLink::changePort(QString str)
{
    this->g_port = str;
    qDebug()<<"temp VM g_port:"<<g_port;
}

void FlowVelocimeterLink::changeBaudRate(QString str)
{
    this->g_baudRate = str;
    qDebug()<<"temp VM this->g_baudRat:"<<this->g_baudRate;
}

void FlowVelocimeterLink::mydisconnect(void)
{
    if (g_mserial->isOpen())
    {
        g_mserial->close();
        g_mserial->deleteLater();
        delete g_mserial;
        g_mserial = nullptr;
    };
}

void FlowVelocimeterLink::connectSerialPort()
{
    if(postStatus == false)
    {
        StartCom();
    }
}

void FlowVelocimeterLink::noMeasurement_FV(bool b)
{
    WLG_measureStates = b;
    if(WLG_measureStates == true && resendStates == true)
    {
        resendStates =  false;
    }

    if(WLG_measureStates == true && intervalVelocimetryStates == true)
    {
        intervalVelocimetryStates = false;
    }


    if(WLG_measureStates == true && postStatus ==true)
    {
        //测速按钮不可点击
        emit connectFV_pushButtonUnableToClick();
    }
}

void FlowVelocimeterLink::single_sendInstructions()
{
    sendButtonIsClicked = true;

    QByteArray buf;
    bool ok;
    char data;
    QStringList list;

    HEXsdStr = "01 03 03 E8 00 01 04 7A";
    list = HEXsdStr.split(" ");
    for(int i = 0; i < list.count(); i++)
    {
        if(list.at(i) == " ")
            continue;
        if(list.at(i).isEmpty())
            continue;
        data = (char)list.at(i).toInt(&ok, 16);
        buf.append(data);
    }
    if(postStatus == true && WLG_measureStates != true)
    {
        //发送单次测速指令
        qDebug()<<"单次测速:"<<buf<<endl;
        g_mserial->write(buf,buf.length());
    }
}

//读取返回的数据
void FlowVelocimeterLink::readMyCom()
{
    //接收测速返回值
    alone_flowVelocityBuf = g_mserial->readAll();
    HEXrdStr = ByteArrayToHexStr(alone_flowVelocityBuf);
    qDebug()<<"单次测速:"<<HEXrdStr;
    qDebug()<<"FV HEXrdStr len:"<<HEXrdStr.length();
    qDebug()<<"FV HEXrdStr size:"<<HEXrdStr.size();
    qDebug()<<"前 tempHEXrdStr_1:"<<tempHEXrdStr_1;
    //接收到的第一段临时数据的前两个字节为"01",且此时接收到的数据的前两个字节为"01",并且此时的数据长度小于32字节 或者
    //此时接收到的数据值为"01",且长度为2 或者
    //此时接收到的数据的前5个字节为"01 03",且长度小于32字节 则
    //先清空保存的第二段临时数据，然后将刚接收到的这段数据临时存储于第一段临时数据"tempHEXrdStr_1"中
    if((tempHEXrdStr_1.mid(0,2) != "01" && HEXrdStr.mid(0,2) == "01" && HEXrdStr.length() < 32) ||
            (HEXrdStr == "01" && HEXrdStr.length() == 2) ||
            (HEXrdStr.mid(0,5) == "01 03" && HEXrdStr.length() < 32))
    {
        tempHEXrdStr_2.clear();
        tempHEXrdStr_2 = nullptr;

        tempHEXrdStr_1 = HEXrdStr;
    }

    //此时接收到的数据的前两个字节不为"01",且长度小于32字节 或者
    //存储的第一段临时数据的前三个字节为"01 ",且此时接收到的数据的前两个字节为"01",并且此时接收到的数据的长度小于32字节 则
    //将刚接收到的这段数据临时存储于第二段临时数据"tempHEXrdStr_2"中
    if((HEXrdStr.mid(0,2) != "01" && HEXrdStr.length() < 32) ||
            (tempHEXrdStr_1.mid(0,3) == "01 " && HEXrdStr.mid(0,2) == "01" && HEXrdStr.length() < 32))
    {
        tempHEXrdStr_2 = HEXrdStr;
    }

    //如果存储的第一段临时数据的长度大于等于5,且这段数据的前五个字节不为"01 03" 则
    //清空第一段临时数据
    if(tempHEXrdStr_1.length() >= 5  && tempHEXrdStr_1.mid(0,5) != "01 03")
    {
        tempHEXrdStr_1.clear();
        tempHEXrdStr_1 = nullptr;
    }

    int st = 0;
    st = tempHEXrdStr_1.length() / 3;
    if(tempHEXrdStr_1.length() == 2 + (3 * st))
    {
        tempHEXrdStr_1 += " ";//拼接缺失的空格
    }

    qDebug()<<"后 tempHEXrdStr_1:"<<tempHEXrdStr_1;
    qDebug()<<"tempHEXrdStr_2:"<<tempHEXrdStr_2;

    //若此时接收的数据长度小于32字节,且存储的第一段临时数据的长度小于32字节,并且存储的第二段临时数据的长度也小于32字节  则
    //拼接字符串，将两段临时数据保存在"str_tempResults"中 否则
    //将此时接收到的数据"HEXrdStr"直接保存到临时变量"str_tempResults"中
    if(HEXrdStr.length() < 32 && tempHEXrdStr_1.length() < 32 && tempHEXrdStr_2.length() < 32)
    {
        str_tempResults = tempHEXrdStr_1 + tempHEXrdStr_2;
        qDebug()<<"aaa";
    }
    else
    {
        str_tempResults = HEXrdStr;
        qDebug()<<"bbb";
    }

    //若接收的数据的前两个字节为"01",且长度等于32字节 或者
    //拼接后的临时数据的前两个字节为"01",且长度等于32字节
    if((HEXrdStr.mid(0,2) == "01" && HEXrdStr.length() == 32) ||
            (str_tempResults.mid(0,2) == "01" && str_tempResults.length() == 32))
    {
        tempHEXrdStr_1.clear();
        tempHEXrdStr_1 = nullptr;

        tempHEXrdStr_2.clear();
        tempHEXrdStr_2 = nullptr;
    }

    //若拼接后的临时数据的长度大于32字节 则
    //隔3秒后 再自动请求一次数据,并且return 否则
    //让最终拼接好的数据"str_Results"直接等于上面保存的临时拼接好的数据"str_tempResults"
    if(str_tempResults.length() > 32)
    {
        time = 3;
        resendStates = true;

        qDebug()<<"FV 再次请求！";

        //测速按钮不可点击
        emit connectFV_pushButtonUnableToClick();

        return;
    }
    else
    {
        str_Results = str_tempResults;
    }

    //若合并后的值不完整 则
    //隔3秒后 再自动请求一次数据  并且
    //停止水位计的测量 否则
    //取消自动请求
    if(str_Results.length() < 32 && WLG_measureStates != true)
    {
        time = 3;
        resendStates = true;

        qDebug()<<"FV 再次请求！";

        //测速按钮不可点击
        emit connectFV_pushButtonUnableToClick();
        //停止测水位 且 测水位按钮不可点击
        emit changeResendStates_WLG();
        emit connectWLG_pushButtonUnableToClick();

    }
    else
    {
        resendStates = false;
        qDebug()<<"FV stop！";

        //恢复测速按钮可点击
        emit connectFV_pushButtonClickable();

        emit recoveryWLG_CanBeMeasured();//恢复水位计按钮可点击

        emit FV_transferMeasurementResults(str_Results);//发送测量结果

    }

}

//初始化定时器
void FlowVelocimeterLink::initqTimer()
{
    this->qtimer = new QTimer(this);

    //    qtimer->moveToThread(this);

    connect(FlowVelocimeterLink::qtimer, SIGNAL(timeout()), this, SLOT(runqTimer()));
    qtimer->start(1000);
}

void FlowVelocimeterLink::runqTimer()
{
    timeStates++;
    if(resendStates == true || intervalVelocimetryStates == true)//接收的值不完整 或者 间隔测速模式启动中
    {
        emit FV_sendMeasureStates(true);//测速仪 重复测量进行中

        if(timeStates % time == 0)
        {
            single_sendInstructions();//每隔time秒重新请求一次
        }
    }
    else
    {
        emit FV_sendMeasureStates(false);
    }
}

////间隔测速
//void FlowVelocimeterLink::resetQTimer_startIntervalTransmission(QString t)
//{
//    timeTemp = t.toInt();
//    time = timeTemp;
//    qDebug()<<"收到 time:"<<time;

//    resendStates = true;
//    intervalVelocimetryStates = true;

//    //测速按钮不可点击
//    emit connectFV_pushButtonUnableToClick();
//}

////取消间隔测速
//void FlowVelocimeterLink::singleMeasurement_cancelIntervalTransmission()
//{
//    resendStates = false;
//    intervalVelocimetryStates = false;

//    //恢复测速按钮可点击
//    emit connectFV_pushButtonClickable();
//}

void FlowVelocimeterLink::recoveryFVCanBeMeasured()
{

    if(postStatus == true)
    {
        //恢复测速按钮可点击
        emit connectFV_pushButtonClickable();
        qDebug()<<"恢复测速按钮可点击!";
    }
}

//void FlowVelocimeterLink::suspendWLGMeasurement(bool b)
//{
//    WLG_measureStates = b;

//    if(WLG_measureStates == true && sendButtonIsClicked == true)
//    {
//        emit suspendWaterLevelGaugeMeasurement();
//    }
//}


