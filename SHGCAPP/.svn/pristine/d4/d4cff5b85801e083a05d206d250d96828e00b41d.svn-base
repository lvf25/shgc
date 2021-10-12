#include "QGCDashBoard.h"
#include "ui_QGCDashBoard.h"
#include "QGCApplication.h"


static QGCDashBoard* _instance = nullptr;   ///< @brief MainWindow singleton

QGCDashBoard* QGCDashBoard::_create(QWidget *parent)
{
    new QGCDashBoard(parent);
    return _instance;
}

QGCDashBoard* QGCDashBoard::instance(void)
{
    return _instance;
}

void QGCDashBoard::deleteInstance(void)
{
    delete this;
}


QGCDashBoard::QGCDashBoard(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::QGCDashBoard)
{
    //设置仪表盘样式 背景为透明
    this->setStyleSheet("background:transparent;border-width:0;border-style:outset");

    ui->setupUi(this);

    _instance = this;

    m_autoResizeHandler=new AutoResize(this,this->rect().width(),this->rect().height());
    m_autoResizeHandler->setAutoResizeFlag(
            AutoResize::INCLUDE_DASHBOARD
            );
    //add widget* manualy
    m_autoResizeHandler->pushAllResizeItem();

    DashBoardStart = new QTimer;
    connect(DashBoardStart,&QTimer::timeout,this,&QGCDashBoard::DashTimeStart);
    DashBoardStart->start(100);

}

QGCDashBoard::~QGCDashBoard()
{
    if(DashBoardStart)
    {
        DashBoardStart->stop();
        delete DashBoardStart;
        DashBoardStart = nullptr;
    }
    _instance = nullptr;
    delete ui;
}



void QGCDashBoard::DashTimeStart()
{
    _vehicle =  qgcApp()->toolbox()->multiVehicleManager()->activeVehicle();

    if(_vehicle)
    {
        int _roll = _vehicle->roll()->rawValue().toInt();
        int _pitch = _vehicle->pitch()->rawValue().toInt();
        double _head = _vehicle->heading()->rawValue().toDouble();

        ui->DashBoardWidget->setDegValue (_roll);
        ui->DashBoardWidget->setRollValue ( _pitch);
        ui->DashBoardWidget->setCompassValue(_head);
    }
}

void QGCDashBoard::resizeEvent(QResizeEvent *event)
{
    m_autoResizeHandler->doAutoResize();
}


