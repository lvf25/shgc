#include "WaterLevelGaugeLink.h"
#include "SHGCMainSetup.h"
#include "MainWindow.h"

static WaterLevelGaugeLink* _instance = nullptr;

WaterLevelGaugeLink* WaterLevelGaugeLink::_create()
{
    _instance = new WaterLevelGaugeLink();
    return _instance;
}

WaterLevelGaugeLink* WaterLevelGaugeLink::instance(void)
{
    return _instance;
}

void WaterLevelGaugeLink::deleteInstance(void)
{  
    delete this;
}

WaterLevelGaugeLink::WaterLevelGaugeLink(QObject *parent) : QObject(parent)
{
    //初始化定时器
    initqTimer();

    SHGCMainSetup *_SHGCMainSetup = SHGCMainSetup::instance();
    //设置串口号
    connect(_SHGCMainSetup,SIGNAL(sendToWLGcomPort(QString)),this,SLOT(changePort(QString)),Qt::AutoConnection);
    //设置波特率
    connect(_SHGCMainSetup,SIGNAL(sendToWLGcomRate(QString)),this,SLOT(changeBaudRate(QString)),Qt::AutoConnection);



    MainWindow *_MainWindow = MainWindow::instance();
    //开始连接
    connect(_MainWindow,SIGNAL(connectWLG_serialPort()),this,SLOT(connectSerialPort()));
    //点击connect_pushButton后 单次发送测水位指令
    connect(_MainWindow,SIGNAL(single_send_Instructions_WLG()),this,SLOT(single_sendInstructions()));

    //关闭页面停止计时器
    //        connect(p,&QGCMain::qtimerStop,this,&QGCWLGDataCom::qtimer_Stop);

    //若测速仪 重复测量进行中 则停止水位计的测量 且水位计按钮不可点击
    connect(_MainWindow,SIGNAL(WLG_receiveMeasureStates(bool)),this,SLOT(noMeasurement_WLG(bool)));

    //若点选了checkBox，且点击了"应用"按钮 则刷新测量间隔 间隔发送查询指令
    connect(_MainWindow,SIGNAL(interval_Measurement_changeTime(QString)),this,SLOT(resetQTimer_startIntervalTransmission(QString)));
    //若不点选checkBox, 则停止定时 取消间隔发送查询指令
    connect(_MainWindow,SIGNAL(single_Measurement()),this,SLOT(singleMeasurement_cancelIntervalTransmission()));
    //若水位计 重复测量进行中 且此时流速计测速按钮被点击 则此时水位计重复测量将暂停5秒后恢复
    //    connect(_MainWindow,SIGNAL(suspendMeasurement_WLG()),this,SLOT(suspendMeasurementTime_WLG()));

    //若流速计停止重复请求数据 则 恢复水位计可测量
    connect(_MainWindow,SIGNAL(recoveryWaterLevelGaugeCanBeMeasured()),this,SLOT(recoveryWLGCanBeMeasured()));
}

WaterLevelGaugeLink::~WaterLevelGaugeLink()
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

//void WaterLevelGaugeLink::createConnectedLink(SharedLinkConfigurationPointer& config, bool isPX4Flow)
//{
//    pLink = new SerialLink(config, isPX4Flow);

//    pLink->_connect();
//}

//void WaterLevelGaugeLink::disconnectLink()
//{
//    pLink->_disconnect();
//}

void WaterLevelGaugeLink::connectSerialPort()
{
    qDebug()<<"postStatus:"<<postStatus;
    if(postStatus == false)
    {
        StartCom();
    }
}

void WaterLevelGaugeLink::StartCom()
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
        qDebug()<< "打开水位计串口成功" <<endl;
        qDebug()<< "QGCWLGDataCom...ID is:" << QThread::currentThreadId();
        //收到消息 解析
        connect(g_mserial, SIGNAL(readyRead()), this, SLOT(readMyCom()));
    }
    else
    {
        postStatus = false;
        qDebug()<< "打开水位计串口失败" <<endl;
        emit openFail(g_port);
    }


    //收到消息 保存
    //    connect(g_mserial, SIGNAL(readyRead()), this, SLOT(generateWLGJSONFile()));
    //收到消息 展示
    //    connect(g_mserial, SIGNAL(readyRead()), this, SIGNAL(WLG_receivetheMessage()));

    qDebug()<<"打开 g_port:"<<this->g_port;
    qDebug()<<"打开 this->g_baudRate:"<<this->g_baudRate;

    return;
}

void WaterLevelGaugeLink::changePort(QString str)
{
    this->g_port = str;
    qDebug()<<"temp WLG g_port:"<<g_port;
}

void WaterLevelGaugeLink::changeBaudRate(QString str)
{
    this->g_baudRate = str;
    qDebug()<<"temp WLG this->g_baudRat:"<<this->g_baudRate;
}

void WaterLevelGaugeLink::mydisconnect(void)
{
    if (g_mserial->isOpen()) {
        g_mserial->close();
        g_mserial->deleteLater();
        delete g_mserial;
        g_mserial = nullptr;
    };
}

void WaterLevelGaugeLink::noMeasurement_WLG(bool b)
{
    FV_measureStates = b;
    if(FV_measureStates == true && resendStates == true)
    {
        resendStates = false;
    }
    if(FV_measureStates == true && intervalVelocimetryStates == true)
    {
        intervalVelocimetryStates = false;
    }

    if(FV_measureStates == true)
    {
        //测水位按钮不可点击
        emit connectWLG_pushButtonUnableToClick();
    }

}

void WaterLevelGaugeLink::single_sendInstructions()
{
    QByteArray buf;
    bool ok;
    char data;
    QStringList list;

    HEXsdStr = "40 20 36 20 30 20 30 0D 0A ";
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
    if(postStatus == true && FV_measureStates != true)
    {
        //发送单次测水位指令
        qDebug()<<"单次测水位:"<<buf<<endl;
        g_mserial->write(buf,buf.length());
    }

}

void WaterLevelGaugeLink::readMyCom()
{
    //接收到测速返回值
    ib_recStatus = true;
    //接收数据
    alone_waterLevelGaugeBuf = g_mserial->readAll();
    qDebug()<<"WLG len:"<<alone_waterLevelGaugeBuf.length();
    str_alone_waterLevelGaugeBuf = QString(alone_waterLevelGaugeBuf);//QByteArray  to   QString
    qDebug()<<"单次测水位 str:"<<str_alone_waterLevelGaugeBuf;
    //若数据开头为"A",并且字节不完整
    if(str_alone_waterLevelGaugeBuf.mid(0,1) == "A" && str_alone_waterLevelGaugeBuf.length() < 15)
    {
        tempStr_alone_waterLevelGaugeBuf_2.clear();
        tempStr_alone_waterLevelGaugeBuf_2 = nullptr;
        tempStr_alone_waterLevelGaugeBuf_1 = str_alone_waterLevelGaugeBuf;//临时存储 第一段测量结果
    }

    //若数据开头不是"A",并且字节不完整
    if(str_alone_waterLevelGaugeBuf.mid(0,1) != "A" && str_alone_waterLevelGaugeBuf.length() <= 16)
    {
        tempStr_alone_waterLevelGaugeBuf_2 = str_alone_waterLevelGaugeBuf;//临时存储 第二段测量结果
    }
    qDebug()<<"tempStr_alone_waterLevelGaugeBuf_1:"<<tempStr_alone_waterLevelGaugeBuf_1;
    qDebug()<<"tempStr_alone_waterLevelGaugeBuf_2:"<<tempStr_alone_waterLevelGaugeBuf_2;

    /******考虑了设备返回的结果有 有没有换行符的情况 并且考虑到不同平台下换行符有所区别(\r\n和\n)******/
    //收到的数据以及两段临时数据字节少于15 或者
    //收到第一段临时数据为"A",且第二段临时数据长度为15或者16,并且第二段临时数据第一个字节值为"6" 或者
    //收到第一段临时数据第一个字节值为"A",并且第二段数据长度为15 则
    //两段数据合并 否则
    //将此时接收到的数据"str_alone_waterLevelGaugeBuf"直接保存到临时变量"str_tempResults"中
    if((str_alone_waterLevelGaugeBuf.length() < 15 && tempStr_alone_waterLevelGaugeBuf_1.length() < 15 && tempStr_alone_waterLevelGaugeBuf_2.length() <= 15) ||
            (tempStr_alone_waterLevelGaugeBuf_1 == "A" &&  tempStr_alone_waterLevelGaugeBuf_2.mid(0,1) == "6" && tempStr_alone_waterLevelGaugeBuf_2.length() == 15) ||
            (tempStr_alone_waterLevelGaugeBuf_1 == "A" &&  tempStr_alone_waterLevelGaugeBuf_2.mid(0,1) == "6" && tempStr_alone_waterLevelGaugeBuf_2.length() == 16) ||
            (tempStr_alone_waterLevelGaugeBuf_1.mid(0,1) == "A" && tempStr_alone_waterLevelGaugeBuf_2.length() == 15))
    {
        str_tempResults = tempStr_alone_waterLevelGaugeBuf_1 + tempStr_alone_waterLevelGaugeBuf_2;
        qDebug()<<"111";
    }
    else
    {
        str_tempResults = str_alone_waterLevelGaugeBuf;
        qDebug()<<"222";
    }

    //若数据开头为"A",并且字节是完整的 则
    //清空缓存数据
    if((str_alone_waterLevelGaugeBuf.mid(0,1) == "A" && str_alone_waterLevelGaugeBuf.length() >= 15) ||
            (str_tempResults.mid(0,1) == "A" && str_tempResults.length() >= 15) )
    {
        tempStr_alone_waterLevelGaugeBuf_1.clear();
        tempStr_alone_waterLevelGaugeBuf_1 = nullptr;

        tempStr_alone_waterLevelGaugeBuf_2.clear();
        tempStr_alone_waterLevelGaugeBuf_2 = nullptr;
    }

    //若临时测量结果 大于17字节 则
    //隔3秒后 再自动请求一次数据,并且return 否则
    //让最终拼接好的数据"str_Results"直接等于上面保存的临时拼接好的数据"str_tempResults"
    if(str_tempResults.length() > 17)
    {
        time = 3;
        resendStates = true;

        qDebug()<<"WLG 再次请求！";

        //测水位按钮不可点击
        emit connectWLG_pushButtonUnableToClick();

        return;
    }
    else
    {

        str_Results = str_tempResults;
    }

    qDebug()<<"str_Results:"<<str_Results;

    /******考虑了设备返回的结果有 有没有换行符的情况 并且考虑到不同平台下换行符有所区别(\r\n和\n)******/
    //若收到的合并后返回值不完整,并且水流测速仪没有在测速 或者
    //收到的合并后返回值不完整,且第一段临时数据开头第一字节不为"A",并且收到的第二段临时数据开头第一字节不为"6" 或者
    //收到的第一段临时数据内容不为"A",并且合并后的数据开头不为"A",且长度小于等于15 或者
    //收到的第一段临时数据内容不为"A",并且收到的第二段临时数据长度小于等于16 或者
    //收到的第一段临时数据内容为"A",并且收到的第二段临时数据开头第一字节不为"6" 则
    //每隔3秒后 再自动请求一次数据 否则
    //直到返回正确数据
    if((str_Results.length() < 15  && FV_measureStates != true) ||
            (str_Results.length() < 15 && tempStr_alone_waterLevelGaugeBuf_1.mid(0,1) != "A" && tempStr_alone_waterLevelGaugeBuf_2.mid(0,1) != "6") ||
            (tempStr_alone_waterLevelGaugeBuf_1 != "A" && str_Results.mid(0,1) != "A" && str_Results.length() <= 15) ||
            (tempStr_alone_waterLevelGaugeBuf_1 != "A" && tempStr_alone_waterLevelGaugeBuf_2.length() == 16) ||
            (tempStr_alone_waterLevelGaugeBuf_1 == "A" && tempStr_alone_waterLevelGaugeBuf_2.mid(0,1) != "6"))
    {
        time = 3;
        resendStates = true;

        qDebug()<<"WLG 再次请求！";

        //测水位按钮不可点击
        emit connectWLG_pushButtonUnableToClick();
    }
    else
    {
        time = timeTemp;
        resendStates = false;

        qDebug()<<"WLG stop！";

        //恢复测水位按钮可点击
        if(resendStates == false && intervalVelocimetryStates == false)
        {
            emit connectWLG_pushButtonClickable();
        }

        emit recoveryFV_CanBeMeasured();//恢复流速计测量按钮可点击

        emit WLG_transferMeasurementResults(str_Results);//发送测量结果

    }



}

//初始化定时器
void WaterLevelGaugeLink::initqTimer()
{
    connect(WaterLevelGaugeLink::qtimer, SIGNAL(timeout()), this, SLOT(runqTimer()));
    qtimer->start(1000);

    //    this->qtimer_s = new QTimer(this);
    //    connect(QGCWLGDataCom::qtimer_s, SIGNAL(timeout()), this, SLOT(setTimeNotReceivedStates()));
    //    qtimer->start(3000);
}

void WaterLevelGaugeLink::runqTimer()
{
    //    qDebug()<<"ib_recStatus:"<<ib_recStatus;
    //    if (ib_recStatus == false)
    //    {
    //        qDebug()<<"ib_recStatus == false";
    //        i_interval ++;
    //        if (i_interval % 2 == 0)
    //        {
    //            qDebug()<<"i_interval == 2";
    //            ib_recStatus = true;
    //            emit WLG_sendMeasureStates(true);

    //            if(timeStates % 3 == 0)
    //            {
    //                single_sendInstructions();
    //            }
    //        }
    //        else
    //        {
    //            return;
    //        }
    //    }
    //    else
    //    {
    //        i_interval = 0;
    //        if (i_interval < 2)
    //        {
    //            ib_recStatus = false;
    //        }

    //        emit WLG_sendMeasureStates(false);
    //    }

    timeStates++;
    if(resendStates == true  || intervalVelocimetryStates == true)
    {
        emit WLG_sendMeasureStates(true);

        if(timeStates % time == 0)
        {
            single_sendInstructions();
        }
    }
    else
    {
        emit WLG_sendMeasureStates(false);
    }

    if(timeStates % 3 == 0)
    {
        timeNotReceived = true;
    }
}

//void WaterLevelGaugeLink::setTimeNotReceivedStates()
//{
//    timeNotReceived = true;
//}

void WaterLevelGaugeLink::resetQTimer_startIntervalTransmission(QString t)
{
    timeTemp = t.toInt();
    time = timeTemp;
    qDebug()<<"收到 time:"<<time;

    resendStates = true;
    intervalVelocimetryStates = true;

    //测水位按钮不可点击
    emit connectWLG_pushButtonUnableToClick();
}

//取消间隔测速
void WaterLevelGaugeLink::singleMeasurement_cancelIntervalTransmission()
{

    resendStates = false;
    intervalVelocimetryStates = false;

    if(postStatus == true)
    {
        //恢复测水位按钮可点击
        emit connectWLG_pushButtonClickable();
    }
}

void WaterLevelGaugeLink::stopWaterLevelMeasurement()
{
    resendStates = false;
    intervalVelocimetryStates = false;

    //测水位按钮不可点击
    emit connectWLG_pushButtonUnableToClick();

    qDebug()<<"测水位按钮不可点击!";
}

//void WaterLevelGaugeLink::suspendMeasurementTime_WLG()
//{
//    //计时器暂停5秒后恢复

//}

//void WaterLevelGaugeLink::suspendTimer()
//{


//}

void WaterLevelGaugeLink::recoveryWLGCanBeMeasured()
{
    if(postStatus == true)
    {
        //恢复测水位按钮可点击
        emit connectWLG_pushButtonClickable();
        qDebug()<<"恢复测水位按钮可点击!";
    }
}
