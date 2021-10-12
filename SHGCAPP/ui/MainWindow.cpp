#include "MainWindow.h"
#include "ui_MainWindow.h"

#include <QScreen>
#include <QSettings>
#include <QTimer>
#include <QDebug>
#include <QNetworkInterface>
#include <QtQml>
#include "AutoResize.h"
#include <QHostAddress>

#include "SHGCVelocimeterInteractiveWidget.h"
#include "SHGCVelocimeterSetupWidget.h"
#include "FlowVelocimeterLink.h"
#include "WaterLevelGaugeLink.h"
#include "CreateMeasurementTempJsonFiles.h"


const char* MAIN_SETTINGS_GROUP = "QGC_MAINWINDOW";
const char* MAINWINDOW = "MainWindow";


static MainWindow* _instance = nullptr;   ///< @brief MainWindow singleton

MainWindow* MainWindow::_create()
{
    new MainWindow();
    return _instance;
}

MainWindow* MainWindow::instance(void)
{
    return _instance;
}

void MainWindow::deleteInstance(void)
{
    delete this;
}


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    _uploadWidget(nullptr),
    ui(new Ui::MainWindow)
  , _lowPowerMode         (false)
  , _showStatusBar        (false)
  , _forceClose           (false)
  , _linkVehicle          (0)

{
    ui->setupUi(this);
    _instance = this;

//    connect(item,SIGNAL(connectUAVSerial()),this,SLOT(connectUAVSerial()));//点击按钮连接无人机

//    saveDeviceID();//保存无人机设备ID至ini文件

    // Setup user interface
    loadSettings();
    emit initStatusChanged(tr("Setting up user interface"), Qt::AlignLeft | Qt::AlignBottom, QColor(62, 93, 141));


    // Create actions
    connectCommonActions();

    //    configureWindowName();

    //    if (settings.contains(_getWindowGeometryKey()))
    //    {
    //        restoreGeometry(settings.value(_getWindowGeometryKey()).toByteArray());
    //    }
    //    else
    //    {
    // Adjust the size
    QScreen* scr = QApplication::primaryScreen();
    scrSize = scr->availableSize();
    QFont f = qgcApp()->font();
    int pixelSize = (f.pointSize() * scr->logicalDotsPerInch()) / 72;



    if (scrSize.width() <= 1280)
    {
        w = scrSize.width();
        h = scrSize.height();
        resize(scrSize.width(), scrSize.height());
    }
    else
    {
        w = scrSize.width()  > 1600 ? 1600 : scrSize.width();
        h = scrSize.height() >  800 ?  800 : scrSize.height();
        resize(w, h);
        move((scrSize.width() - w) / 2, (scrSize.height() - h) / 2);
    }

    //设置窗口最大化
//    this->showMaximized();

    _saveMainWindowSettings();
    show();

    // Status Bar
    setStatusBar(new QStatusBar(this));
    statusBar()->setSizeGripEnabled(true);

    //#ifndef __mobile__
    emit initStatusChanged(tr("Building common widgets."), Qt::AlignLeft | Qt::AlignBottom, QColor(62, 93, 141));
    _buildCommonWidgets();
    emit initStatusChanged(tr("Building common actions"), Qt::AlignLeft | Qt::AlignBottom, QColor(62, 93, 141));
    //#endif


    QGCDashBoard *_QGCDashBoard = QGCDashBoard::_create(this);
    _QGCDashBoard->move(0, (h/9));
    _QGCDashBoard->show();

    //实例化设置 界面 类
    SHGCMainSetup::_create(this);

    //    _uploadWidget = SHGCUpLoadWidget::_create(this);

    creatMapWindow();



    //实例化 "雷达水流测速仪"交互页面 类
    SHGCVelocimeterInteractiveWidget::_create(this);
    //实例化 "雷达水流测速仪"设置界面 类
    SHGCVelocimeterSetUpWidget::_create(this);
    //实例化流速计、水位计 通讯和数据处理 类
    FlowVelocimeterLink::_create();
    WaterLevelGaugeLink::_create();
    CreateMeasurementTempJsonFiles::_create();


    //获取历史设置参数
    emit getSettingParameters();

    FV_WLGConnected();//流速计_水位计 连接

    initialization_isMainWindow_width = this->width();
    initialization_isMainWindow_height = this->height();
    qDebug()<<"MainWindow 01:"<<this->width()<<" "<<this->height();
}

MainWindow::~MainWindow()
{
    QGCDashBoard* _QGCDashBoard = QGCDashBoard::instance();
    if (_QGCDashBoard) {
        _QGCDashBoard->deleteInstance();
    }
    SHGCMainSetup* _SHGCMainSetup = SHGCMainSetup::instance();
    if(_SHGCMainSetup)
    {
        _SHGCMainSetup->deleteInstance();
    }
    SHGCVelocimeterInteractiveWidget *_SHGCVelocimeterInteractiveWidget = SHGCVelocimeterInteractiveWidget::instance();
    if(_SHGCVelocimeterInteractiveWidget)
    {
        _SHGCVelocimeterInteractiveWidget->deleteInstance();
    }
    SHGCVelocimeterSetUpWidget *_SHGCVelocimeterSetUpWidget = SHGCVelocimeterSetUpWidget::instance();
    if(_SHGCVelocimeterSetUpWidget)
    {
        _SHGCVelocimeterSetUpWidget->deleteInstance();
    }
    FlowVelocimeterLink *_FlowVelocimeterLink = FlowVelocimeterLink::instance();
    if(_FlowVelocimeterLink)
    {
        _FlowVelocimeterLink->deleteInstance();
    }
    WaterLevelGaugeLink *_WaterLevelGaugeLink = WaterLevelGaugeLink::instance();
    if(_WaterLevelGaugeLink)
    {
        _WaterLevelGaugeLink->deleteInstance();
    }
    CreateMeasurementTempJsonFiles *_CreateMeasurementTempJsonFiles = CreateMeasurementTempJsonFiles::instance();
    if(_CreateMeasurementTempJsonFiles)
    {
        _CreateMeasurementTempJsonFiles->deleteInstance();
    }
    SHGCUpLoadWidget *_SHGCUpLoadWidget =  SHGCUpLoadWidget::instance();
    if(_SHGCUpLoadWidget)
    {
        _SHGCUpLoadWidget->deleteInstance();
    }

    point_Edit_Widget* pointEdit = point_Edit_Widget::instance();
    if(pointEdit != nullptr)
    {
        pointEdit->deleteInstance();
    }

    _instance = nullptr;
    delete ui;
}

void MainWindow::connectUAVSerialLink(bool str)
{
    if(str)
    {
        LinkManager* linkMgr = qgcApp()->toolbox()->linkManager();
        SharedLinkConfigurationPointer sharedConfig;
        switch (_linkType) {
            case LinkTypeSerial:
            {
                SerialConfiguration* linkconfig = new SerialConfiguration("serial link");
                linkconfig->setBaud(g_baudRate.toInt());
                linkconfig->setPortName(g_port);
                sharedConfig = linkMgr->addConfiguration(linkconfig);
                _seriallink = (SerialLink*)qgcApp()->toolbox()->linkManager()->createConnectedLink(sharedConfig);
                break;
            }

            case LinkTypeTcp:
            {
                qDebug() << "tcp-----------------link " << endl;
                TCPConfiguration* tcpconfig = new TCPConfiguration("tcp link");
                tcpconfig->setHost(_ip);
                tcpconfig->setPort(_port.toUInt());
                qDebug() << "=====================" << tcpconfig->port() << tcpconfig->host() << "=====================" << endl;
                sharedConfig = linkMgr->addConfiguration(tcpconfig);
                _tcpLink = (TCPLink*)qgcApp()->toolbox()->linkManager()->createConnectedLink(sharedConfig);
                break;
            }
            case LinkTypeUdp:
            {
               UDPConfiguration* udpConfig = new UDPConfiguration("udp link");
               udpConfig->addHost(_ip,_port.toUInt());
               sharedConfig = linkMgr->addConfiguration(udpConfig);
               _udpLink = (UDPLink*)qgcApp()->toolbox()->linkManager()->createConnectedLink(sharedConfig);
               break;
            }
        }
    }
    else
    {
//        qgcApp()->toolbox()->linkManager()->disconnectLink(_seriallink);
        qgcApp()->toolbox()->linkManager()->disconnectAll();
    }

}



void MainWindow::_showLeftWidget()
{
    if(leftWindow == nullptr)
    {
        leftWindow = new leftMainWindow(this);
    }
    leftWindow->move(this->geometry().x(),this->geometry().y());
    leftWindow->resize(this->width()/4,this->height());
    leftWindow->activateWindow();
    leftWindow->show();
    disconnect(leftWindow,SIGNAL(sendBtnData(QString)),this,SLOT(createMyAction(QString)));
    connect(leftWindow,SIGNAL(sendBtnData(QString)),this,SLOT(createMyAction(QString)));

    //    QGCDashBoard * _QGCDashBoard = QGCDashBoard::instance();
    //    if(_QGCDashBoard != nullptr)
    //    {
    //            QRect resizeRect=_QGCDashBoard->rect();
    //    }

    //槽函数结束之前先断开信号和槽 避免槽函数多次重复调用
    //    disconnect(item,SIGNAL(showLeftMainWindow()),this,SLOT(_showLeftWidget()));
}



void MainWindow::resizeEvent(QResizeEvent *event)
{
    if(event->oldSize() == event->size())
    {
        return;
    }
    _saveMainWindowSettings();

    resize_isMainWindow_width = this->width();
    resize_isMainWindow_height = this->height();

    resize_Coefficient_width = initialization_isMainWindow_width / resize_isMainWindow_width;
    resize_Coefficient_height = initialization_isMainWindow_height / resize_isMainWindow_height;

    QGCDashBoard* _QGCDashBoard = QGCDashBoard::instance();
    if(_QGCDashBoard != nullptr)
    {
        AutoResize::ResizeParentWidget(this,_QGCDashBoard,AutoResize::INCLUDE_DASHBOARD);//仪表盘自适应
    }

    SHGCMainSetup* _SHGCMainSetup = SHGCMainSetup::instance();
    if(_SHGCMainSetup != nullptr)
    {
        _SHGCMainSetup->resize(660,1800);
        _SHGCMainSetup->move(this->width() - _SHGCMainSetup->width(),0);
        AutoResize::ResizeParentWidget(this,_SHGCMainSetup,AutoResize::INCLUDE_MAINSETUP);//设置界面自适应
    }

    SHGCVelocimeterInteractiveWidget* _SHGCVelocimeterInteractiveWidget = SHGCVelocimeterInteractiveWidget::instance();
    if(_SHGCVelocimeterInteractiveWidget != nullptr)
    {
//        _SHGCVelocimeterInteractiveWidget->resize(489,429);

//        qDebug()<<"MainWindow 02:"<<this->width()<<" "<<this->height();

//        int mainWidgetwidth = this->width();
//        int W_VelocimeterInteractiveWidget = mainWidgetwidth - 489;
//        _SHGCVelocimeterInteractiveWidget->move(W_VelocimeterInteractiveWidget - 70, 160);
//        AutoResize::ResizeParentWidget(this,_SHGCVelocimeterInteractiveWidget,AutoResize::INCLUDE_VELOCIMETERINTERACTIVE);//设置界面自适应
    }

    SHGCVelocimeterSetUpWidget* _SHGCVelocimeterSetUpWidget = SHGCVelocimeterSetUpWidget::instance();
    if(_SHGCVelocimeterSetUpWidget != nullptr)
    {
//        _SHGCVelocimeterSetUpWidget->resize(489,387);

//        int mainWidgetwidth = this->width();
//        int W_VelocimeterSetUpWidget = mainWidgetwidth - 489;
//        _SHGCVelocimeterSetUpWidget->move(W_VelocimeterSetUpWidget - 70,160);
//        AutoResize::ResizeParentWidget(this,_SHGCVelocimeterSetUpWidget,AutoResize::INCLUDE_VELOCIMETERSETUP);//设置界面自适应
    }

    if(_uploadWidget != nullptr)
    {
        _uploadWidget->resize(this->width(),this->height());
        _uploadWidget->move(0,0);
    }

    point_Edit_Widget* pointEdit = point_Edit_Widget::instance();
    if(pointEdit != nullptr)
    {
        pointEdit->resize(660,1800);
        pointEdit->move(this->width() - pointEdit->width(),0);
        AutoResize::ResizeParentWidget(this,pointEdit,AutoResize::INCLUDE_MAINSETUP);//设置界面自适应
    }

}

void MainWindow::_buildCommonWidgets(void)
{
//#ifdef WIN32
//    logPlayer = new QGCMAVLinkLogPlayer(statusBar());
//    logPlayer->close();
//    statusBar()->addPermanentWidget(logPlayer);
//#endif
}


//创建弹出菜单按钮
void MainWindow::createMyAction(QString name)
{
    //    QAction *act = (QAction *)sender();
    //    QString name = act->text();
    if (name == "btnFlightMonit") {
        actionFlightMonit();
        leftWindow->hide();
    } else if (name == "btnRoutePlan") {
        actionRoutePlan();
        leftWindow->hide();
    } else if (name == "btnActionAdjust"){
        actionAdjust();
        //        connect(mainSetup,SIGNAL(closeSetup()),this,SLOT(quitSetup()));
        leftWindow->hide();
    } else if (name == "btnActionSetUp"){
        actionSetUp();
        leftWindow->hide();
    }
}

//飞行控制
void MainWindow::actionFlightMonit()
{
    emit showFlightMonitRectange();
    QGCDashBoard* _QGCDashBoard = QGCDashBoard::instance();
    _QGCDashBoard->show();
}


//航线规划
void MainWindow::actionRoutePlan()
{
    emit showRoutePlanRectange();
    QGCDashBoard* _QGCDashBoard = QGCDashBoard::instance();
    _QGCDashBoard->hide();
}


//校准
void MainWindow::actionAdjust()
{
    if(_linkVehicle == 0)
    {
        QMessageBox::warning(NULL, "warning", "not connect vehicle...", QMessageBox::Yes, QMessageBox::Yes);
        return;
    }
    emit showActionAdjust();
    QGCDashBoard* _QGCDashBoard = QGCDashBoard::instance();
    _QGCDashBoard->hide();
}

//设置
void MainWindow::actionSetUp()
{
    SHGCMainSetup* _SHGCMainSetup = SHGCMainSetup::instance();

    _SHGCMainSetup->resize(660,1800);
    _SHGCMainSetup->move(this->width() - _SHGCMainSetup->width(),0);
    _SHGCMainSetup->activateWindow();//设置为顶层窗口
    _SHGCMainSetup->setWindowFlags(Qt::CustomizeWindowHint|Qt::FramelessWindowHint);
    _SHGCMainSetup->show();

    SHGCVelocimeterInteractiveWidget *_SHGCVelocimeterInteractiveWidget = SHGCVelocimeterInteractiveWidget::instance();
    if(_SHGCVelocimeterInteractiveWidget != nullptr)
    {
        _SHGCVelocimeterInteractiveWidget->close();
    }
    SHGCVelocimeterSetUpWidget  *_SHGCVelocimeterSetUpWidget = SHGCVelocimeterSetUpWidget::instance();
    if(_SHGCVelocimeterSetUpWidget != nullptr)
    {
        _SHGCVelocimeterSetUpWidget->close();
    }


    disconnect(_SHGCMainSetup,SIGNAL(closeSetup()),this,SLOT(quitSetup()));
    connect(_SHGCMainSetup,SIGNAL(closeSetup()),this,SLOT(quitSetup()));
}

void MainWindow::setAircraftcomPort(QString port)
{
    g_port = port;
}

void MainWindow::setAircraftcomRate(QString rate)
{
    g_baudRate = rate;
}

void MainWindow::quitSetup()
{
    SHGCMainSetup *_SHGCMainSetup = SHGCMainSetup::instance();
    _SHGCMainSetup->hide();
}

void MainWindow::creatMapWindow()
{
    QUrl source("qrc:/qml/frmmap.qml");
    ui->MapQuickWidget->setSource(source);
    ui->MapQuickWidget->show();

    item = qobject_cast<QObject*>(ui->MapQuickWidget->rootObject());

    connect(item,SIGNAL(logReplay()),this,SLOT(openLogReplay()));
    connect(this,SIGNAL(showFlightMonitRectange()),item,SIGNAL(showFlightMonitRectange()));
    connect(this,SIGNAL(showRoutePlanRectange()),item,SIGNAL(showRoutePlanRectange()));
    connect(this,SIGNAL(showActionAdjust()),item,SIGNAL(showActionAdjust()));

    disconnect(item,SIGNAL(showUpload()),this,SLOT(showUpload()));
    connect(item,SIGNAL(showUpload()),this,SLOT(showUpload()));
    connect(item,SIGNAL(showLeftMainWindow()),this,SLOT(_showLeftWidget()));
    connect(qgcApp(),SIGNAL(showmessage(QString)),item,SIGNAL(showMessage(QString)));

    connect(item,SIGNAL(point_Edit_WidgetStatue(QString)),this,SLOT(Point_Edit_Widget(QString)));

    SHGCMainSetup *_SHGCMainSetup = SHGCMainSetup::instance();
    //设置无人机串口号和波特率
    disconnect(_SHGCMainSetup,SIGNAL(sendToAircraftcomPort(QString)),this,SLOT(setAircraftcomPort(QString)));
    connect(_SHGCMainSetup,SIGNAL(sendToAircraftcomPort(QString)),this,SLOT(setAircraftcomPort(QString)));
    disconnect(_SHGCMainSetup,SIGNAL(sendToAircraftcomRate(QString)),this,SLOT(setAircraftcomRate(QString)));
    connect(_SHGCMainSetup,SIGNAL(sendToAircraftcomRate(QString)),this,SLOT(setAircraftcomRate(QString)));

    connect(item,SIGNAL(connectUAVSerialLink(bool)),this,SLOT(connectUAVSerialLink(bool)));

    connect(_SHGCMainSetup,SIGNAL(LinkTcp(QString,QString)),this,SLOT(linkTcp(QString,QString)));
    connect(_SHGCMainSetup,SIGNAL(linkUdp(QString,QString)),this,SLOT(linkUdp(QString,QString)));
    connect(_SHGCMainSetup,SIGNAL(linkSerial(QString,QString)),this,SLOT(linkSerial(QString,QString)));
}


void MainWindow::linkTcp(QString ip,QString port)
{
    _linkType = LinkTypeTcp;
    _ip = ip;
    _port = port;
    qDebug() << "-------------------------------" << _ip << _port << endl;
}

void MainWindow::linkUdp(QString ip,QString port)
{
    _linkType = LinkTypeUdp;
    _ip = ip;
    _port = port;
    qDebug() << "-------------------------------" << _ip << _port << endl;
}

void MainWindow::linkSerial(QString port,QString baud)
{
    _linkType = LinkTypeSerial;
    g_port = port;
    g_baudRate = baud;
    qDebug() << "-------------------------------" << port << baud << endl;

}




void MainWindow::showUpload()
{
    if(_uploadWidget == nullptr)
    {
        _uploadWidget = SHGCUpLoadWidget::_create(this);
    }else{
        _uploadWidget = SHGCUpLoadWidget::instance();
    }
    _uploadWidget->move(0,0);
    _uploadWidget->resize(this->width(),this->height());

    connect(_uploadWidget,SIGNAL(uploadToUAV()),item,SIGNAL(uploadToUAV()));
    _uploadWidget->show();
}

void MainWindow::Point_Edit_Widget(QString seq_no)
{
    int num = seq_no.toInt();
    qDebug() << "seq_no ::" << num << endl;
    point_Edit_Widget* pointEdit = point_Edit_Widget::instance();
    if(pointEdit == nullptr)
    {
        pointEdit = point_Edit_Widget::_create(this);
    }
    else
    {
        pointEdit = point_Edit_Widget::instance();
    }

    pointEdit->move(this->geometry().x()+this->width()*2/3,this->geometry().y());
    pointEdit->resize(this->width()/3,this->height());
    pointEdit->activateWindow();
    pointEdit->show();

    connect(pointEdit,SIGNAL(addPointItem(double,double,int)),item,SIGNAL(addPointItem(double,double,int)),Qt::UniqueConnection);
    connect(pointEdit,SIGNAL(removePointItem(int)),item,SIGNAL(removePointItem(int)),Qt::UniqueConnection);
    connect(pointEdit,SIGNAL(upPointItem(int)),item,SIGNAL(upPointItem(int)),Qt::UniqueConnection);
    connect(pointEdit,SIGNAL(downPointItem(int)),item,SIGNAL(downPointItem(int)),Qt::UniqueConnection);

}

void MainWindow::openLogReplay()
{
//    if(logPlayer->isVisible())
//    {
//        logPlayer->close();

//    }else{

//        logPlayer->show();
//    }


    //槽函数结束之前先断开信号和槽 避免槽函数多次重复调用
    //    disconnect(item,SIGNAL(logReplay()),this,SLOT(openLogReplay()));
}


void MainWindow::configureWindowName()
{
    setWindowTitle(qApp->applicationName() + " " + qApp->applicationVersion());
}


void MainWindow::connectCommonActions()
{
    // Connect internal actions
    connect(qgcApp()->toolbox()->multiVehicleManager(), &MultiVehicleManager::vehicleAdded, this, &MainWindow::_vehicleAdded);
    connect(qgcApp()->toolbox()->multiVehicleManager(), &MultiVehicleManager::vehicleRemoved,this,&MainWindow::_vehicleRemoved);
    connect(this, &MainWindow::reallyClose, this, &MainWindow::_reallyClose, Qt::QueuedConnection); // Queued to allow closeEvent to fully unwind before _reallyClose is called
}

void MainWindow::_vehicleAdded(Vehicle* vehicle)
{
    _linkVehicle++;

    //    connect(vehicle->uas(), &UAS::valueChanged, this, &MainWindow::valueChanged);
    //槽函数结束之前先断开信号和槽 避免槽函数多次重复调用
//    disconnect(qgcApp()->toolbox()->multiVehicleManager(), &MultiVehicleManager::vehicleAdded, this, &MainWindow::_vehicleAdded);
}

void MainWindow::_vehicleRemoved(Vehicle* vehicle)
{
    _linkVehicle--;
//    disconnect(qgcApp()->toolbox()->multiVehicleManager(), &MultiVehicleManager::vehicleRemoved,this,&MainWindow::_vehicleRemoved);
}

void MainWindow::_reallyClose(void)
{
    _forceClose = true;
    close();
}


QObject* MainWindow::rootQmlObject(void)
{
    return _instance;
}

void MainWindow::loadSettings()
{
    // Why the screaming?
    QSettings settings;
    settings.beginGroup(MAIN_SETTINGS_GROUP);
    _lowPowerMode   = settings.value("LOW_POWER_MODE",      _lowPowerMode).toBool();
    _showStatusBar  = settings.value("SHOW_STATUSBAR",      _showStatusBar).toBool();
    settings.endGroup();
}

QString MainWindow::_getWindowGeometryKey()
{
    return "_geometry";
}


void MainWindow::_saveMainWindowSettings()
{
    QSettings settings;
    settings.beginGroup(MAINWINDOW);
    settings.setValue("width",     w);
    settings.setValue("height",    h);
    settings.endGroup();
}

void MainWindow::storeSettings()
{
    QSettings settings;
    settings.beginGroup(MAIN_SETTINGS_GROUP);
    settings.setValue("LOW_POWER_MODE",     _lowPowerMode);
    settings.setValue("SHOW_STATUSBAR",     _showStatusBar);
    settings.endGroup();
    settings.setValue(_getWindowGeometryKey(), saveGeometry());

    //#ifndef __mobile__
    //    _storeVisibleWidgetsSettings();
    //#endif
}

void MainWindow::FV_WLGConnected()
{
    //若主界面的"测速"按钮产生点击事件，则弹出"雷达水流测速仪"交互页面
    connect(item,SIGNAL(showFV_WLG()),this,SLOT(showVelocimeterInteractiveWidget()));
    //若主界面的"测速"按钮产生点击事件，则弹出"雷达水流测速仪"交互页面 的同时 尝试连接"雷达水流测速仪"串口
//    disconnect(item,SIGNAL(showFV_WLG()),this,SIGNAL(connectFV_serialPort()));
//    connect(item,SIGNAL(showFV_WLG()),this,SIGNAL(connectFV_serialPort()));
//    //若主界面的"测速"按钮产生点击事件，则弹出"雷达水流测速仪"交互页面 的同时 尝试连接"水位计"串口
//    disconnect(item,SIGNAL(showFV_WLG()),this,SIGNAL(connectWLG_serialPort()));
//    connect(item,SIGNAL(showFV_WLG()),this,SIGNAL(connectWLG_serialPort()));
//    //若主界面的"测速"按钮再一次产生点击事件，则关闭窗口。 同时，停止定时器定时发送请求。
    connect(item,SIGNAL(closeFV_WLG()),this,SLOT(closeVelocimeterInteractiveWidget()));


    FlowVelocimeterLink* _FlowVelocimeterLink = FlowVelocimeterLink::instance();
    WaterLevelGaugeLink* _WaterLevelGaugeLink = WaterLevelGaugeLink::instance();
    //更改测速仪测量按钮状态
    disconnect(_FlowVelocimeterLink,SIGNAL(connectFV_pushButtonUnableToClick()),this,SIGNAL(FV_pushButtonUnableToClick()));
    connect(_FlowVelocimeterLink,SIGNAL(connectFV_pushButtonUnableToClick()),this,SIGNAL(FV_pushButtonUnableToClick()));
    disconnect(_FlowVelocimeterLink,SIGNAL(connectFV_pushButtonClickable()),this,SIGNAL(FV_pushButtonClickable()));
    connect(_FlowVelocimeterLink,SIGNAL(connectFV_pushButtonClickable()),this,SIGNAL(FV_pushButtonClickable()));
    //更改水位计测量按钮状态
    disconnect(_WaterLevelGaugeLink,SIGNAL(connectWLG_pushButtonUnableToClick()),this,SIGNAL(WLG_pushButtonUnableToClick()));
    connect(_WaterLevelGaugeLink,SIGNAL(connectWLG_pushButtonUnableToClick()),this,SIGNAL(WLG_pushButtonUnableToClick()));
    disconnect(_WaterLevelGaugeLink,SIGNAL(connectWLG_pushButtonClickable()),this,SIGNAL(WLG_pushButtonClickable()));
    connect(_WaterLevelGaugeLink,SIGNAL(connectWLG_pushButtonClickable()),this,SIGNAL(WLG_pushButtonClickable()));

    disconnect(_FlowVelocimeterLink,SIGNAL(connectWLG_pushButtonUnableToClick()),this,SIGNAL(WLG_pushButtonUnableToClick()));
    connect(_FlowVelocimeterLink,SIGNAL(connectWLG_pushButtonUnableToClick()),this,SIGNAL(WLG_pushButtonUnableToClick()));

    //若测速仪 重复测量进行中 则停止水位计的测量 且水位计测量按钮不可点击
    disconnect(_FlowVelocimeterLink,SIGNAL(FV_sendMeasureStates(bool)),this,SIGNAL(WLG_receiveMeasureStates(bool)));
    connect(_FlowVelocimeterLink,SIGNAL(FV_sendMeasureStates(bool)),this,SIGNAL(WLG_receiveMeasureStates(bool)));
    //若水位计 重复测量进行中 则停止流速计的测量  且流速计测量按钮不可点击          /*并且 若此时流速计测速按钮被点击 则此时水位计重复测量将暂停5秒后恢复*/
    disconnect(_WaterLevelGaugeLink,SIGNAL(WLG_sendMeasureStates(bool)),this,SIGNAL(FV_receiveMeasureStates(bool)));
    connect(_WaterLevelGaugeLink,SIGNAL(WLG_sendMeasureStates(bool)),this,SIGNAL(FV_receiveMeasureStates(bool)));
//    disconnect(_FlowVelocimeterLink,SIGNAL(suspendWaterLevelGaugeMeasurement()),this,SIGNAL(suspendMeasurement_WLG()));
//    connect(_FlowVelocimeterLink,SIGNAL(suspendWaterLevelGaugeMeasurement()),this,SIGNAL(suspendMeasurement_WLG()));

    //若水位计 重复测量进行中 此时若想启动测速仪的间隔测量 则会弹出提示
    disconnect(_WaterLevelGaugeLink,SIGNAL(WLG_sendMeasureStates(bool)),this,SIGNAL(showTipsWidget(bool)));
    connect(_WaterLevelGaugeLink,SIGNAL(WLG_sendMeasureStates(bool)),this,SIGNAL(showTipsWidget(bool)));
    //    connect(_WaterLevelGaugeLink,SIGNAL(WLG_sendMeasureStates(bool)),this,SIGNAL(receiveMeasureStates_fromWLG(bool)));

    //若测速仪接收到返回数据 则将数据传递过去解析
    disconnect(_FlowVelocimeterLink,SIGNAL(FV_transferMeasurementResults(QString)),this,SIGNAL(transferMeasurementResults_formFV(QString)));
    connect(_FlowVelocimeterLink,SIGNAL(FV_transferMeasurementResults(QString)),this,SIGNAL(transferMeasurementResults_formFV(QString)));
    //若测速仪接收到返回数据 则传递数据接收状态
    disconnect(_FlowVelocimeterLink,SIGNAL(FV_transferMeasurementResults(QString)),this,SIGNAL(transmitDataReceiveStatus_formFV()));
    connect(_FlowVelocimeterLink,SIGNAL(FV_transferMeasurementResults(QString)),this,SIGNAL(transmitDataReceiveStatus_formFV()));
    //若水位计接收到返回数据 则将数据传递过去解析
    disconnect(_WaterLevelGaugeLink,SIGNAL(WLG_transferMeasurementResults(QString)),this,SIGNAL(transferMeasurementResults_formWLG(QString)));
    connect(_WaterLevelGaugeLink,SIGNAL(WLG_transferMeasurementResults(QString)),this,SIGNAL(transferMeasurementResults_formWLG(QString)));

    SHGCVelocimeterInteractiveWidget *_SHGCVelocimeterInteractiveWidget = SHGCVelocimeterInteractiveWidget::instance();
    disconnect(_SHGCVelocimeterInteractiveWidget,SIGNAL(sendSignal_setAircraftCenter(bool)),item,SIGNAL(setAircraftCenter(bool)));
    connect(_SHGCVelocimeterInteractiveWidget,SIGNAL(sendSignal_setAircraftCenter(bool)),item,SIGNAL(setAircraftCenter_fromVelocimeter(bool)));
}

void MainWindow::showVelocimeterInteractiveWidget()
{
    SHGCVelocimeterInteractiveWidget *_SHGCVelocimeterInteractiveWidget = SHGCVelocimeterInteractiveWidget::instance();
    int mainWidgetwidth = this->width();
    int W_VelocimeterInteractiveWidget = mainWidgetwidth - _SHGCVelocimeterInteractiveWidget->width();

    _SHGCVelocimeterInteractiveWidget->move(W_VelocimeterInteractiveWidget - 70,160);
    _SHGCVelocimeterInteractiveWidget->show();
    emit SHGCVelocimeterInteractiveWidget_isShow();

    //若"雷达水流测速仪"交互页面连接串口按钮产生点击事件，则转发信号尝试连接串口
    disconnect(_SHGCVelocimeterInteractiveWidget, SIGNAL(connectFV_pushButton_isClicked_conn()),this,SIGNAL(connectFV_serialPort()));
    connect(_SHGCVelocimeterInteractiveWidget, SIGNAL(connectFV_pushButton_isClicked_conn()),this,SIGNAL(connectFV_serialPort()));
    //若"雷达水流测速仪"交互页面连接串口按钮产生点击事件，则转发信号尝试发送单次测速指令
    disconnect(_SHGCVelocimeterInteractiveWidget, SIGNAL(connectFV_pushButton_isClicked_send()),this,SIGNAL(single_send_Instructions_FV()));
    connect(_SHGCVelocimeterInteractiveWidget, SIGNAL(connectFV_pushButton_isClicked_send()),this,SIGNAL(single_send_Instructions_FV()));
    //若"雷达水流测速仪"交互页面checkBox被选中， 且"应用"按钮被点击 则调整水位计发送间隔 间隔发送查询指令 且流速计不可测量
    disconnect(_SHGCVelocimeterInteractiveWidget,SIGNAL(intervalMeasurement_changeTime(QString)),this,SIGNAL(interval_Measurement_changeTime(QString)));
    connect(_SHGCVelocimeterInteractiveWidget,SIGNAL(intervalMeasurement_changeTime(QString)),this,SIGNAL(interval_Measurement_changeTime(QString)));
    //若"雷达水流测速仪"交互页面checkBox未被选中，则转发信号 停止水位计间隔发送 且恢复流速计可测量
    disconnect(_SHGCVelocimeterInteractiveWidget,SIGNAL(checkBoxIsUnchecked()),this,SIGNAL(single_Measurement()));
    connect(_SHGCVelocimeterInteractiveWidget,SIGNAL(checkBoxIsUnchecked()),this,SIGNAL(single_Measurement()));
    disconnect(_SHGCVelocimeterInteractiveWidget,SIGNAL(checkBoxIsUnchecked()),this,SIGNAL(recoveryFlowVelocimeterCanBeMeasured()));
    connect(_SHGCVelocimeterInteractiveWidget,SIGNAL(checkBoxIsUnchecked()),this,SIGNAL(recoveryFlowVelocimeterCanBeMeasured()));

    FlowVelocimeterLink* _FlowVelocimeterLink = FlowVelocimeterLink::instance();
    //若串口连接成功
    disconnect(_FlowVelocimeterLink,SIGNAL(openSuccess(QString)),this,SIGNAL(openVMDataComSuccess()));
    connect(_FlowVelocimeterLink,SIGNAL(openSuccess(QString)),this,SIGNAL(openVMDataComSuccess()));
    //若串口连接失败
    disconnect(_FlowVelocimeterLink,SIGNAL(openFail(QString)),this,SIGNAL(openVMDataComFail()));
    connect(_FlowVelocimeterLink,SIGNAL(openFail(QString)),this,SIGNAL(openVMDataComFail()));

    CreateMeasurementTempJsonFiles *_CreateMeasurementTempJsonFiles = CreateMeasurementTempJsonFiles::instance();
    //若收到"雷达水流测速仪"设备返回的数据 则读取数据 在界面展示
    disconnect(_CreateMeasurementTempJsonFiles,SIGNAL(receivetheMessage(bool)),this,SIGNAL(getTheLatestValue(bool)));
    connect(_CreateMeasurementTempJsonFiles,SIGNAL(receivetheMessage(bool)),this,SIGNAL(getTheLatestValue(bool)));

    //若"水位计"交互页面连接串口按钮产生点击事件，则转发信号尝试连接串口
    disconnect(_SHGCVelocimeterInteractiveWidget, SIGNAL(connectWLG_pushButton_isClicked_conn()),this,SIGNAL(connectWLG_serialPort()));
    connect(_SHGCVelocimeterInteractiveWidget, SIGNAL(connectWLG_pushButton_isClicked_conn()),this,SIGNAL(connectWLG_serialPort()));
    //若"水位计"交互页面连接串口按钮产生点击事件，则转发信号尝试发送单次测速指令
    disconnect(_SHGCVelocimeterInteractiveWidget, SIGNAL(connectWLG_pushButton_isClicked_send()),this,SIGNAL(single_send_Instructions_WLG()));
    connect(_SHGCVelocimeterInteractiveWidget, SIGNAL(connectWLG_pushButton_isClicked_send()),this,SIGNAL(single_send_Instructions_WLG()));

    WaterLevelGaugeLink *_WaterLevelGaugeLink = WaterLevelGaugeLink::instance();
    //若串口连接成功
    disconnect(_WaterLevelGaugeLink,SIGNAL(openSuccess(QString)),this,SIGNAL(openWLGDataComSuccess()));
    connect(_WaterLevelGaugeLink,SIGNAL(openSuccess(QString)),this,SIGNAL(openWLGDataComSuccess()));
    //若串口连接失败
    disconnect(_WaterLevelGaugeLink,SIGNAL(openFail(QString)),this,SIGNAL(openWLGDataComFail()));
    connect(_WaterLevelGaugeLink,SIGNAL(openFail(QString)),this,SIGNAL(openWLGDataComFail()));

    //若收到水位计设备返回的数据 则读取数据 在界面展示
    disconnect(_CreateMeasurementTempJsonFiles,SIGNAL(WLG_receivetheMessage(bool)),this,SIGNAL(WLG_getTheLatestValue(bool)));
    connect(_CreateMeasurementTempJsonFiles,SIGNAL(WLG_receivetheMessage(bool)),this,SIGNAL(WLG_getTheLatestValue(bool)));

    //点击"雷达水流测速仪"界面的设置按钮，弹出设置页面
    disconnect(_SHGCVelocimeterInteractiveWidget,SIGNAL(setUp_pushButton_isClicked()),this,SLOT(showVelocimeterSetUpWidget()));
    connect(_SHGCVelocimeterInteractiveWidget,SIGNAL(setUp_pushButton_isClicked()),this,SLOT(showVelocimeterSetUpWidget()));

    //若水位计接口接收到正确数据后停止重复请求 则恢复流速计可测量
    disconnect(_WaterLevelGaugeLink,SIGNAL(recoveryFV_CanBeMeasured()),this,SIGNAL(recoveryFlowVelocimeterCanBeMeasured()));
    connect(_WaterLevelGaugeLink,SIGNAL(recoveryFV_CanBeMeasured()),this,SIGNAL(recoveryFlowVelocimeterCanBeMeasured()));
    //若流速计接口接收到正确数据后停止重复请求 则恢复水位计可测量
    disconnect(_FlowVelocimeterLink,SIGNAL(recoveryWLG_CanBeMeasured()),this,SIGNAL(recoveryWaterLevelGaugeCanBeMeasured()));
    connect(_FlowVelocimeterLink,SIGNAL(recoveryWLG_CanBeMeasured()),this,SIGNAL(recoveryWaterLevelGaugeCanBeMeasured()));

    //槽函数结束之前先断开信号和槽 避免槽函数多次重复调用
    //    disconnect(item,SIGNAL(showFV_WLG()),this,SLOT(showVelocimeterInteractiveWidget()));
}

void MainWindow::closeVelocimeterInteractiveWidget()
{
    SHGCVelocimeterInteractiveWidget *_SHGCVelocimeterInteractiveWidget = SHGCVelocimeterInteractiveWidget::instance();
    if(_SHGCVelocimeterInteractiveWidget != nullptr)
    {
        _SHGCVelocimeterInteractiveWidget->close();
    }
    SHGCVelocimeterSetUpWidget *_SHGCVelocimeterSetUpWidget = SHGCVelocimeterSetUpWidget::instance();
    if(_SHGCVelocimeterSetUpWidget != nullptr)
    {
        _SHGCVelocimeterSetUpWidget->close();
    }

    //停止定时器定时发送请求。
    emit qtimerStop();


    //槽函数结束之前先断开信号和槽 避免槽函数多次重复调用
    //    disconnect(item,SIGNAL(closeFV_WLG()),this,SLOT(closeVelocimeterInteractiveWidget()));
}

void MainWindow::showVelocimeterSetUpWidget()
{
    SHGCVelocimeterInteractiveWidget *_SHGCVelocimeterInteractiveWidget = SHGCVelocimeterInteractiveWidget::instance();
    _SHGCVelocimeterInteractiveWidget->hide();

    SHGCVelocimeterSetUpWidget *_SHGCVelocimeterSetUpWidget = SHGCVelocimeterSetUpWidget::instance();
    int mainWidgetwidth = this->width();
    int W_VelocimeterSetUpWidget = mainWidgetwidth - _SHGCVelocimeterSetUpWidget->width();

    _SHGCVelocimeterSetUpWidget->move(W_VelocimeterSetUpWidget - 70,160);
    _SHGCVelocimeterSetUpWidget->show();

    connect(_SHGCVelocimeterSetUpWidget,&SHGCVelocimeterSetUpWidget::return_pushButton_isClicked,
            [=]()
    {
        _SHGCVelocimeterSetUpWidget->hide();
        _SHGCVelocimeterInteractiveWidget->show();

    });
    connect(_SHGCVelocimeterSetUpWidget,&SHGCVelocimeterSetUpWidget::close_pushButton_isClicked,this,
            [=]()
    {
        _SHGCVelocimeterSetUpWidget->close();
        _SHGCVelocimeterInteractiveWidget->close();

    });
    connect(_SHGCVelocimeterSetUpWidget,SIGNAL(SHGCVelocimeterSetUpWidget::close_pushButton_isClicked()),item,
            SLOT(changefv_WLGQmlBool()));//更改MapToHtmlRectange.qml 中 fv_WLGBottonClicked = false


    //设置校准系数
    connect(_SHGCVelocimeterSetUpWidget,SIGNAL(application_pushButton_isClicked_1(double,double,double)),this,
            SIGNAL(setCalibrationCoefficient_1(double,double,double)));
    connect(_SHGCVelocimeterSetUpWidget,SIGNAL(application_pushButton_isClicked_2(double,double,double)),this,
            SIGNAL(setCalibrationCoefficient_2(double,double,double)));
    connect(_SHGCVelocimeterSetUpWidget,SIGNAL(application_pushButton_isClicked_3(double,double,double)),this,
            SIGNAL(setCalibrationCoefficient_3(double,double,double)));

    //更改提示框文字
    connect(_SHGCVelocimeterSetUpWidget,SIGNAL(changeText_Unknown()),this,
            SIGNAL(changeText_isUnknown()));
    connect(_SHGCVelocimeterSetUpWidget,SIGNAL(changeText_Ok()),this,
            SIGNAL(changeText_isOk()));
    connect(_SHGCVelocimeterSetUpWidget,SIGNAL(changeText_Error()),this,
            SIGNAL(changeText_isError()));
}

//配置 设备ID ini文件
//void MainWindow::saveDeviceID()
//{
//    //创建保存ini文件的目录
//    bool ok;
//    QString str = QCoreApplication::applicationDirPath();
//    qDebug()<<"str dir:"<<str;
//    QDir dir(str + "/ini");
//    if(dir.exists())
//    {
//        ok = true;
//        qDebug() << "dir exist!";
//    }
//    else
//    {
//        ok = dir.mkdir(str + "/ini");//只创建一级子目录，即必须保证上级目录存在
//        qDebug() << "dir not exists, generate!";
//    }

//    QString path = "ini/cal_DeviceID.ini";
//    QString device_ID = "001";//该值需自动获取

//    QSettings settings(path, QSettings::IniFormat);
//    int number = settings.value("DevOption/number").toInt();
//    int deviceNum = settings.value(QString("DevOption/device_Num_%1").arg(number)).toInt();
//    device_Num = deviceNum;

//    for(int i = 1;i <= number; i++)
//    {
//        QString deviceID = settings.value(QString("DevOption/device_ID_%1").arg(i)).toString();
//        if(device_ID == deviceID)//当前设备ID 在配置文件里面
//        {
//            return;
//        }
//    }

//    device_Num++;
//    settings.beginGroup("DevOption");
//    settings.setValue("number",device_Num);
//    settings.setValue(QString("dividingLine_%1").arg(device_Num),"===================");
//    settings.setValue(QString("device_ID_%1").arg(device_Num),device_ID);
//    settings.setValue(QString("device_Num_%1").arg(device_Num),device_Num);
//    settings.endGroup();

//}

