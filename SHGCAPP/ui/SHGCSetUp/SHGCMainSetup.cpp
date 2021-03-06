#include "SHGCMainSetup.h"
#include "ui_SHGCMainSetup.h"
#include <QPainter>

#ifdef WIN32
#include <Windows.h>
#include <sysinfoapi.h>
#endif

#include <QDebug>
#include <QMessageBox>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QtDebug>
#include <QSqlError>
#include <QSqlQueryModel>
#include <QSqlTableModel>
#include <QSqlRelationalTableModel>
#include <QTableView>
#include <QSqlError>
#include <QSqlDriver>
#include <QSqlRecord>
#include <QSqlField>
#include <QDir>
#include <QFileDialog>
#include <QCompleter>
#include <QtSerialPort/QSerialPort>
#include <QSerialPortInfo>

#include "MainWindow.h"

#ifdef Q_OS_LINUX
#include <QStorageInfo>
#endif

bool SHGCMainSetup::btn_download_clicked = false;
bool SHGCMainSetup::btn_downing_clicked = false;

static SHGCMainSetup* _instance = nullptr;

SHGCMainSetup* SHGCMainSetup::_create(QWidget *parent)
{
    _instance = new SHGCMainSetup(parent);
    return _instance;
}

SHGCMainSetup* SHGCMainSetup::instance(void)
{
    return _instance;
}

void SHGCMainSetup::deleteInstance(void)
{
    delete this;
}

SHGCMainSetup::SHGCMainSetup(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SHGCMainSetup)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint);
    setMouseTracking(true);

    //自动获取并设置串口
//    AutomaticAdditionSerialPort();

    //加载设置参数配置 并发送历史设置参数 给 参数使用者
    loadSettingParameters();
    MainWindow *_MainWindow = MainWindow::instance();
    connect(_MainWindow,SIGNAL(getSettingParameters()),this,SLOT(sendSettingParameters()));

    init_setup();

    connect(ui->data_conn_log_15,SIGNAL(widget_clicked()),this,SLOT(on_data_conn_log_15_click()));
    connect(ui->widget_79,SIGNAL(widget_clicked()),this,SLOT(on_widget_79_click()));
    connect(ui->data_conn_log_14,SIGNAL(widget_clicked()),this,SLOT(on_data_conn_log_14_click()));
    connect(ui->USB_1,SIGNAL(widget_clicked()),this,SLOT(on_USB_1_click()));
    connect(ui->TCP_1,SIGNAL(widget_clicked()),this,SLOT(on_TCP_1_click()));
    connect(ui->UDP_Serve_1,SIGNAL(widget_clicked()),this,SLOT(on_UDP_Serve_1_click()));
    connect(ui->bluetooth_1,SIGNAL(widget_clicked()),this,SLOT(on_bluetooth_1_click()));
    connect(ui->set_return_2,SIGNAL(clicked()),this,SLOT(on_set_return_up_to_setting_clicked()));
    connect(ui->set_return_3,SIGNAL(clicked()),this,SLOT(on_set_return_up_to_setting_clicked()));
    connect(ui->set_return_4,SIGNAL(clicked()),this,SLOT(on_set_return_up_to_setting_clicked()));
    connect(ui->set_return_5,SIGNAL(clicked()),this,SLOT(on_set_return_up_to_setting_clicked()));
    connect(ui->data_conn_log_FV_WLG,SIGNAL(widget_clicked()),this,SLOT(on_data_conn_log_FV_WLG_click()));

    //发送流速最大偏差值
    connect(ui->MaxDeviationValue_Edit,SIGNAL(textChanged(QString)),this,SIGNAL(sendMaxDeviationValue(QString)));


    //    connect(ui->set_return_2,SIGNAL(clicked()),this,SLOT(on_set_return_up_to_setting_clicked()));
    //    connect(ui->set_return_7,SIGNAL(clicked()),this,SLOT(on_set_return_up_to_setting_clicked()));
    //    connect(ui->set_return_8,SIGNAL(clicked()),this,SLOT(on_set_return_up_to_setting_clicked()));
    //    connect(ui->set_return_9,SIGNAL(clicked()),this,SLOT(on_set_return_up_to_setting_clicked()));


    //    connect(ui->set_return_3,SIGNAL(clicked()),this,SLOT(on_set_return_up_to_data_connect()));
    //    connect(ui->set_return_4,SIGNAL(clicked()),this,SLOT(on_set_return_up_to_data_connect()));
    //    connect(ui->set_return_5,SIGNAL(clicked()),this,SLOT(on_set_return_up_to_data_connect()));
    //    connect(ui->set_return_6,SIGNAL(clicked()),this,SLOT(on_set_return_up_to_data_connect()));

    //    connect(ui->cW_USB_1,SIGNAL(widget_clicked()),this,SLOT(on_cW_USB_click()));
    //    connect(ui->cW_USB_3,SIGNAL(widget_clicked()),this,SLOT(on_cW_USB_click()));
    //    connect(ui->cW_USB_4,SIGNAL(widget_clicked()),this,SLOT(on_cW_USB_click()));
    //    connect(ui->cW_USB_5,SIGNAL(widget_clicked()),this,SLOT(on_cW_USB_click()));

    //    connect(ui->cW_TCP_1,SIGNAL(widget_clicked()),this,SLOT(on_cW_TCP_click()));
    //    connect(ui->cW_TCP_2,SIGNAL(widget_clicked()),this,SLOT(on_cW_TCP_click()));
    //    connect(ui->cW_TCP_3,SIGNAL(widget_clicked()),this,SLOT(on_cW_TCP_click()));
    //    connect(ui->cW_TCP_4,SIGNAL(widget_clicked()),this,SLOT(on_cW_TCP_click()));
    //    connect(ui->cW_TCP_5,SIGNAL(widget_clicked()),this,SLOT(on_cW_TCP_click()));

    //    connect(ui->cW_UDP_1,SIGNAL(widget_clicked()),this,SLOT(on_cW_UDP_click()));
    //    connect(ui->cW_UDP_2,SIGNAL(widget_clicked()),this,SLOT(on_cW_UDP_click()));
    //    connect(ui->cW_UDP_3,SIGNAL(widget_clicked()),this,SLOT(on_cW_UDP_click()));
    //    connect(ui->cW_UDP_5,SIGNAL(widget_clicked()),this,SLOT(on_cW_UDP_click()));

    //    connect(ui->cW_bluetooth_1,SIGNAL(widget_clicked()),this,SLOT(on_cW_bluetooth_click()));
    //    connect(ui->cW_bluetooth_2,SIGNAL(widget_clicked()),this,SLOT(on_cW_bluetooth_click()));
    //    connect(ui->cW_bluetooth_3,SIGNAL(widget_clicked()),this,SLOT(on_cW_bluetooth_click()));
    //    connect(ui->cW_bluetooth_4,SIGNAL(widget_clicked()),this,SLOT(on_cW_bluetooth_click()));

    //    connect(ui->cW_USB_2,SIGNAL(widget_clicked()),this,SLOT(on_cW_USB_2_click()));
    //    connect(ui->cW_TCP_3,SIGNAL(widget_clicked()),this,SLOT(on_cW_TCP_3_click()));
    //    connect(ui->cW_UDP_4,SIGNAL(widget_clicked()),this,SLOT(on_cW_UDP_4_click()));
    //    connect(ui->cW_bluetooth_1,SIGNAL(widget_clicked()),this,SLOT(on_cW_bluetooth_1_click()));

    m_autoResizeHandler=new AutoResize(this,this->rect().width(),this->rect().height());
    m_autoResizeHandler->setAutoResizeFlag(
                AutoResize::INCLUDE_MAINSETUP|
                AutoResize::INCLUDE_STACKED|
                AutoResize::INCLUDE_WIDGET|
                AutoResize::INCLUDE_PUSHBUTTON|
                AutoResize::INCLUDE_LABEL|
                AutoResize::INCLUDE_CLICKEDWIDGET|
                AutoResize::INCLUDE_SWITCHBUTTON|
                AutoResize::INCLUDE_AUTOSAVESLIDER|
                AutoResize::INCLUDE_LINEEDIT|
                AutoResize::INCLUDE_SLIDERPOINTER|
                AutoResize::INCLUDE_ELECPOINTER|
                AutoResize::INCLUDE_COMBOBOX
                );
    //add widget* manualy
    m_autoResizeHandler->pushAllResizeItem();
}

SHGCMainSetup::~SHGCMainSetup()
{
    //析构时保存设置参数
    preservingSettingParameters();

    _instance = nullptr;
    delete ui;
}

//界面初始化
void SHGCMainSetup::init_setup()
{
    //    ui->setup_widget->show();
    //    ui->data_connect_widget_Default->hide();
    //    ui->data_connect_widget_USB_On->hide();
    //    ui->data_connect_widget_TCP_On->hide();
    //    ui->data_connect_widget_UDP_On->hide();
    //    ui->data_connect_widget_Bluetooth_On->hide();
    //    ui->tts_widget->hide();
    ui->stackedWidget->setCurrentIndex(0);

    ui->voice_tip_slider_2->setValue(5);
    ui->voice_tip_slider_2->m_displayLabel->setText(QString::number(10)+"s");
    ui->voice_tip_slider_2->m_displayLabel->setVisible(true);
    ui->voice_tip_slider_2->m_displayLabel->move((ui->voice_tip_slider_2->width()-ui->voice_tip_slider_2->m_displayLabel->width())*ui->voice_tip_slider_2->value()/(ui->voice_tip_slider_2->maximum()-ui->voice_tip_slider_2->minimum()),-1);

    ui->elec_tip_slider_2->setValue(15);
    ui->elec_tip_slider_2->n_displayLabel->setText(QString::number(20)+"%");
    ui->elec_tip_slider_2->n_displayLabel->setVisible(true);
    ui->elec_tip_slider_2->n_displayLabel->move((ui->elec_tip_slider_2->width()-ui->elec_tip_slider_2->n_displayLabel->width())*ui->elec_tip_slider_2->value()/(ui->elec_tip_slider_2->maximum()-ui->elec_tip_slider_2->minimum()),-1);

    ui->saveMap_2->setEnabled(false);
    ui->saveVideoPlane_2->setEnabled(false);
    ui->saveVideoLocal_2->setEnabled(false);
    ui->saveVideoPath->setEnabled(false);
    ui->saveMissonPath->setEnabled(false);
    //    m_timer = new QTimer(this);
    //    connect(m_timer, &QTimer::timeout, this, &QGCMainSetup::updateInfo);
    //    m_timer->start(100);

    ui->return_height_2->setText(QString::number(50));

    QString dirPath;
    dirPath = "E:\\Qt\\QGC-APP";
    ui->saveVideoPath->setText(dirPath);
    ui->saveMissonPath->setText("\\bin\\pointData");

    //    updateInfo(dirPath);

    initSwitchButton();

    ui->show_con_way->setAlignment(Qt::AlignRight | Qt::AlignHCenter);

    ui->widget_continueFlyingExplain->hide();
    ui->widget_turnBackExplain->show();

    //将QComboBox的编辑框设置成可编辑的
    ui->aircraft_SerialPort->setEditable(true);
    ui->aircraft_BaudRate->setEditable(true);
    ui->VM_SerialPort->setEditable(true);
    ui->VM_BaudRate->setEditable(true);
    ui->WLG_SerialPort->setEditable(true);
    ui->WLG_BaudRate->setEditable(true);

    ui->UAV_Number->setAlignment(Qt::AlignRight);
    ui->UAV_Number->setText(UAV_Number);
}

void SHGCMainSetup::initSwitchButton()
{
    QString textOn = tr("");
    QString textOff = tr("");

    ui->map_save_switch_2->setTextOn(textOn);
    ui->map_save_switch_2->setTextOff(textOff);
    ui->map_save_switch_2->setChecked(false);

    ui->video_save_local_switch_2->setTextOn(textOn);
    ui->video_save_local_switch_2->setTextOff(textOff);
    ui->video_save_local_switch_2->setChecked(false);
    ui->video_save_plane_switch_2->setTextOn(textOn);
    ui->video_save_plane_switch_2->setTextOff(textOff);
    ui->video_save_plane_switch_2->setChecked(false);

    ui->choose_word_switch_2->setTextOn(textOn);
    ui->choose_word_switch_2->setTextOff(textOff);
    ui->choose_word_switch_2->setChecked(false);
    ui->choose_voice_switch_2->setTextOn(textOn);
    ui->choose_voice_switch_2->setTextOff(textOff);
    ui->choose_voice_switch_2->setChecked(false);

    ui->open_video_switch_2->setTextOn(textOn);
    ui->open_video_switch_2->setTextOff(textOff);
    ui->open_video_switch_2->setChecked(false);
    ui->open_board_switch_2->setTextOn(textOn);
    ui->open_board_switch_2->setTextOff(textOff);
    ui->open_board_switch_2->setChecked(false);
    ui->show_data_switch_2->setTextOn(textOn);
    ui->show_data_switch_2->setTextOff(textOff);
    ui->show_data_switch_2->setChecked(false);

    ui->choose_word_switch_12->setTextOn(textOn);
    ui->choose_word_switch_12->setTextOff(textOff);
    ui->choose_word_switch_12->setChecked(false);
    ui->choose_word_switch_3->setTextOn(textOn);
    ui->choose_word_switch_3->setTextOff(textOff);
    ui->choose_word_switch_3->setChecked(false);
    ui->choose_word_switch_4->setTextOn(textOn);
    ui->choose_word_switch_4->setTextOff(textOff);
    ui->choose_word_switch_4->setChecked(false);
    ui->choose_word_switch_7->setTextOn(textOn);
    ui->choose_word_switch_7->setTextOff(textOff);
    ui->choose_word_switch_7->setChecked(false);
    ui->choose_word_switch_8->setTextOn(textOn);
    ui->choose_word_switch_8->setTextOff(textOff);
    ui->choose_word_switch_8->setChecked(false);
    ui->choose_word_switch_6->setTextOn(textOn);
    ui->choose_word_switch_6->setTextOff(textOff);
    ui->choose_word_switch_6->setChecked(false);
    ui->choose_word_switch_10->setTextOn(textOn);
    ui->choose_word_switch_10->setTextOff(textOff);
    ui->choose_word_switch_10->setChecked(false);
    ui->choose_word_switch_11->setTextOn(textOn);
    ui->choose_word_switch_11->setTextOff(textOff);
    ui->choose_word_switch_11->setChecked(false);
    ui->choose_word_switch_14->setTextOn(textOn);
    ui->choose_word_switch_14->setTextOff(textOff);
    ui->choose_word_switch_14->setChecked(false);
    ui->choose_word_switch_13->setTextOn(textOn);
    ui->choose_word_switch_13->setTextOff(textOff);
    ui->choose_word_switch_13->setChecked(false);

    ui->debugMode_switch->setTextOn(textOn);
    ui->debugMode_switch->setTextOff(textOff);
    ui->debugMode_switch->setChecked(bool_FV_WLG_DebugMode);
}

#ifdef WIN32
//获取电脑磁盘剩余容量
void SHGCMainSetup::updateInfo(const QString&)
{
    double dTotalBytes, dFreeBytes;
    GetDiskSpace( "C:/", dTotalBytes, dFreeBytes );
    double C_Freebytes = QString::number( dFreeBytes, 'f', 3 ).toDouble();

    GetDiskSpace( "D:/", dTotalBytes, dFreeBytes );
    double D_Freebytes = QString::number( dFreeBytes, 'f', 3 ).toDouble();

    GetDiskSpace( "E:/", dTotalBytes, dFreeBytes );
    double E_Freebytes = QString::number( dFreeBytes, 'f', 3 ).toDouble();

    GetDiskSpace( "F:/", dTotalBytes, dFreeBytes );
    double F_Freebytes = QString::number( dFreeBytes, 'f', 3 ).toDouble();

    QString show = "剩余";
    show.append(QString::number( C_Freebytes+D_Freebytes+E_Freebytes+F_Freebytes, 'f', 3 ));
    show.append("G");
    ui->saveVideoLocal_2->setText(show);
    ui->saveMap_2->setText(show);

}

//LPCWSTR只适用于windows系统
void SHGCMainSetup::GetDiskSpace( QString re_dirPath, double& dTotalBytes, double& dFreeBytes )
{
    LPCWSTR lpcwstrDriver = LPCWSTR(re_dirPath.utf16());
    ULARGE_INTEGER lFreeBytesAvailable, lTotalBytesTemp, lTotalFreeBytes;

    if( !GetDiskFreeSpaceEx( lpcwstrDriver, &lFreeBytesAvailable, &lTotalBytesTemp, &lTotalFreeBytes ) )
    {
        QMessageBox::warning( nullptr, "Warning", "Acquire Disk Space Failed !" );
        dTotalBytes = -1;
        dFreeBytes = -1;
        return;
    }

    //单位 : GB
    dTotalBytes = lTotalBytesTemp.QuadPart / 1024 / 1024 / 1024;
    dFreeBytes = lTotalFreeBytes.QuadPart / 1024 / 1024 / 1024;
}
#else

//获取安卓手机剩余磁盘容量
void SHGCMainSetup::updateInfo(const QString&)
{
    QStorageInfo storage = QStorageInfo::root();
    storage.refresh();  //获得最新磁盘信息

    QString show = "剩余";
    show.append(QString::number(storage.bytesAvailable()/1000/1000, 'f', 3 ));
    show.append("G");
    ui->saveVideoLocal_2->setText(show);
    ui->saveMap_2->setText(show);
}
#endif

////选中USB连接方式
//void SHGCMainSetup::on_USB_1_click()
//{
//    ui->data_connect_widget_USB_On->show();
//    ui->tts_widget->hide();
//    ui->setup_widget->show();
//    ui->data_connect_widget_TCP_On->hide();
//    ui->data_connect_widget_UDP_On->hide();
//    ui->data_connect_widget_Bluetooth_On->hide();
//    ui->data_connect_widget_Default->hide();
//    ui->w_USB_1->setStyleSheet("background-color: rgb(255, 255, 255);");
//    ui->choose_usb_1->setStyleSheet("background-image: url(:/images/choose.png);");
//    ui->w_TCP_1->setStyleSheet("background-color: rgb(255, 255, 255);");
//    ui->choose_tcp_1->setStyleSheet(" ");
//    ui->w_UDP_1->setStyleSheet("background-color: rgb(255, 255, 255);");
//    ui->choose_udp_1->setStyleSheet(" ");
//    ui->w_blueth_1->setStyleSheet("background-color: rgb(255, 255, 255);");
//    ui->choose_bluetooth_1->setStyleSheet(" ");
//    ui->w_USB_2->setStyleSheet("background-color: rgb(255, 255, 255);");
//    ui->choose_usb_2->setStyleSheet("background-image: url(:/images/choose.png);");
//    ui->w_TCP_2->setStyleSheet("background-color: rgb(255, 255, 255);");
//    ui->choose_tcp_2->setStyleSheet(" ");
//    ui->w_UDP_2->setStyleSheet("background-color: rgb(255, 255, 255);");
//    ui->choose_udp_2->setStyleSheet(" ");
//    ui->w_blueth_2->setStyleSheet("background-color: rgb(255, 255, 255);");
//    ui->choose_bluetooth_2->setStyleSheet(" ");
//    ui->show_con_way->setText("USB");
//}

//关闭IP、端口填写界面
//void SHGCMainSetup::on_set_return_1_clicked()
//{
//    ui->setup_widget->show();
//    ui->data_connect_widget_Default->hide();
//    ui->data_connect_widget_USB_On->hide();
//    ui->data_connect_widget_TCP_On->hide();
//    ui->data_connect_widget_UDP_On->hide();
//    ui->data_connect_widget_Bluetooth_On->hide();
//    ui->tts_widget->hide();
//}


//打开无人机连接方式界面
void SHGCMainSetup::on_data_conn_log_15_click()
{
    //    ui->data_connect_widget_Default->show();
    //    ui->setup_widget->hide();
    //    ui->data_connect_widget_USB_On->hide();
    //    ui->data_connect_widget_TCP_On->hide();
    //    ui->data_connect_widget_UDP_On->hide();
    //    ui->data_connect_widget_Bluetooth_On->hide();
    //    ui->tts_widget->hide();
    //    if (ui->show_con_way->text()=="USB")
    ui->stackedWidget->setCurrentIndex(1);
//    ui->choose_usb_1->setStyleSheet("background-image: url(:/images/resources/images/ui/SHGCSetUp/choose.png);");
    ui->widget_TCP->hide();
    ui->widget_UDP->hide();
    ui->widget_USB->setStyleSheet("background-color: rgb(255, 255, 255);");
//    ui->choose_usb_1->setStyleSheet("background-image: url(:/images/resources/images/ui/SHGCSetUp/choose.png);");

   ui->w_TCP_1->setStyleSheet("background-color: rgb(255, 255, 255);");
   ui->w_UDP_1->setStyleSheet("background-color: rgb(255, 255, 255);");
   ui->w_blueth_1->setStyleSheet("background-color: rgb(255, 255, 255);");

    //    if(ui->show_con_way->text()=="TCP")
    //        ui->stackedWidget->setCurrentIndex(1);
    //        ui->widget_USB->hide();
    //        ui->widget_UDP->hide();
    //        ui->widget_TCP->setStyleSheet("background-color: rgb(232, 232, 232);");

    //    if(ui->show_con_way->text()=="UDP")
    //        ui->stackedWidget->setCurrentIndex(1);
    //        ui->widget_USB->hide();
    //        ui->widget_TCP->hide();
    //        ui->widget_UDP->setStyleSheet("background-color: rgb(232, 232, 232);");

    //    if(ui->show_con_way->text()=="bluetooth")
    //        ui->stackedWidget->setCurrentIndex(1);
    //        ui->widget_USB->hide();
    //        ui->widget_TCP->hide();
    //        ui->widget_UDP->hide();
    //        ui->bluetooth_1->setStyleSheet("background-color: rgb(232, 232, 232);");
    //        ui->choose_bluetooth_1->setStyleSheet("background-image: url(:/images/choose.png);");

    //加载设置参数配置
//    loadSettingParameters();

    //设置无人机串口
    disconnect(ui->aircraft_SerialPort,SIGNAL(currentTextChanged(QString)),this,SIGNAL(sendToAircraftcomPort(QString)));
    connect(ui->aircraft_SerialPort,SIGNAL(currentTextChanged(QString)),this,SIGNAL(sendToAircraftcomPort(QString)));
    //设置无人机波特率
    disconnect(ui->aircraft_BaudRate,SIGNAL(currentTextChanged(QString)),this,SIGNAL(sendToAircraftcomRate(QString)));
    connect(ui->aircraft_BaudRate,SIGNAL(currentTextChanged(QString)),this,SIGNAL(sendToAircraftcomRate(QString)));
}

//选中USB连接方式
void SHGCMainSetup::on_USB_1_click()
{
    ui->w_USB_1->setStyleSheet("background-color: rgb(232, 232, 232);");
//    ui->choose_usb_1->setStyleSheet("background-image: url(:/images/resources/images/ui/SHGCSetUp/choose.png);");
    ui->w_TCP_1->setStyleSheet("background-color: rgb(255, 255, 255);");
//    ui->choose_tcp_1->setStyleSheet(" ");
    ui->w_UDP_1->setStyleSheet("background-color: rgb(255, 255, 255);");
//    ui->choose_udp_1->setStyleSheet(" ");
    ui->w_blueth_1->setStyleSheet("background-color: rgb(255, 255, 255);");
    ui->choose_bluetooth_1->setStyleSheet(" ");
    ui->show_con_way->setText("USB");
    ui->widget_TCP->hide();
    ui->widget_UDP->hide();
    ui->widget_USB->show();
    ui->widget_USB->setStyleSheet("background-color: rgb(255, 255, 255);");

    //扫描存在的串口
    qDebug()<<"扫描存在的串口:";
    ui->aircraft_SerialPort->clear();
    foreach(const QSerialPortInfo &info,QSerialPortInfo::availablePorts())
    {
        QSerialPort ser;
        ser.setPort(info);
        /*判断串口能否打开*/
        if(ser.open(QIODevice::ReadWrite))
        {
            ui->aircraft_SerialPort->addItem(ser.portName());
            qDebug()<<"ser.portName()13234564:"<<ser.portName();
        }
        ser.close();
    }
}


//选中TCP连接方式
void SHGCMainSetup::on_TCP_1_click()
{
    ui->w_USB_1->setStyleSheet("background-color: rgb(255, 255, 255);");
//    ui->choose_usb_1->setStyleSheet(" ");
    ui->w_TCP_1->setStyleSheet("background-color: rgb(232, 232, 232);");
//    ui->choose_tcp_1->setStyleSheet("background-image: url(:/images/resources/images/ui/SHGCSetUp/choose.png);");
    ui->w_UDP_1->setStyleSheet("background-color: rgb(255, 255, 255);");
//    ui->choose_udp_1->setStyleSheet(" ");
    ui->w_blueth_1->setStyleSheet("background-color: rgb(255, 255, 255);");
    ui->choose_bluetooth_1->setStyleSheet(" ");
    ui->show_con_way->setText("TCP");
    ui->widget_TCP->show();
    ui->widget_UDP->hide();
    ui->widget_USB->hide();
    ui->widget_TCP->setStyleSheet("background-color: rgb(255, 255, 255);");
}

//选中UDP连接-服务器端口
void SHGCMainSetup::on_UDP_Serve_1_click()
{
    ui->w_USB_1->setStyleSheet("background-color: rgb(255, 255, 255);");
//    ui->choose_usb_1->setStyleSheet(" ");
    ui->w_TCP_1->setStyleSheet("background-color: rgb(255, 255, 255);");
//    ui->choose_tcp_1->setStyleSheet(" ");
    ui->w_UDP_1->setStyleSheet("background-color: rgb(232, 232, 232);");
//    ui->choose_udp_1->setStyleSheet("background-image: url(:/images/resources/images/ui/SHGCSetUp/choose.png);");
    ui->w_blueth_1->setStyleSheet("background-color: rgb(255, 255, 255);");
    ui->choose_bluetooth_1->setStyleSheet(" ");
    ui->show_con_way->setText("UDP");
    ui->widget_TCP->hide();
    ui->widget_UDP->show();
    ui->widget_USB->hide();
    ui->widget_UDP->setStyleSheet("background-color: rgb(255, 255, 255);");
}

//选中蓝牙连接
void SHGCMainSetup::on_bluetooth_1_click()
{
    ui->w_USB_1->setStyleSheet("background-color: rgb(255, 255, 255);");
//    ui->choose_usb_1->setStyleSheet(" ");
    ui->w_TCP_1->setStyleSheet("background-color: rgb(255, 255, 255);");
//    ui->choose_tcp_1->setStyleSheet(" ");
    ui->w_UDP_1->setStyleSheet("background-color: rgb(255, 255, 255);");
//    ui->choose_udp_1->setStyleSheet(" ");
    ui->w_blueth_1->setStyleSheet("background-color: rgb(232, 232, 232);");
    ui->choose_bluetooth_1->setStyleSheet("background-image: url(:/images/resources/images/ui/SHGCSetUp/choose.png);");
    ui->show_con_way->setText("BlueTooth");
    ui->widget_TCP->hide();
    ui->widget_UDP->hide();
    ui->widget_USB->hide();
}

//打开USB界面填写端口和波特率
void SHGCMainSetup::on_btn_USBInput_1_clicked()
{
    //    ui->setup_widget->hide();
    //    ui->data_connect_widget_Default->hide();
    //    ui->tts_widget->hide();
    ui->widget_TCP->hide();
    ui->widget_UDP->hide();
    ui->widget_USB->show();
}

//打开TCP界面填写IP和端口
void SHGCMainSetup::on_btn_TCPInput_1_clicked()
{
    //    ui->setup_widget->hide();
    //    ui->data_connect_widget_Default->hide();
    //    ui->tts_widget->hide();
    ui->widget_TCP->show();
    ui->widget_UDP->hide();
    ui->widget_USB->hide();
}

//打开UDP界面填写IP和端口
void SHGCMainSetup::on_btn_UDPInput_1_clicked()
{
    //    ui->setup_widget->hide();
    //    ui->data_connect_widget_Default->hide();
    //    ui->data_connect_widget_2->show();
    //    ui->tts_widget->hide();
    ui->widget_TCP->hide();
    ui->widget_UDP->show();
    ui->widget_USB->hide();

}

//关闭设置界面
void SHGCMainSetup::on_set_return_1_clicked()
{
    emit closeSetup();

    //保存设置参数
    preservingSettingParameters();
}


//返回设置界面
void SHGCMainSetup::on_set_return_up_to_setting_clicked()
{
    //    ui->setup_widget->show();
    //    ui->data_connect_widget_Default->hide();
    //    ui->data_connect_widget_UDP_On->hide();
    //    ui->data_connect_widget_USB_On->hide();
    //    ui->data_connect_widget_TCP_On->hide();
    //    ui->data_connect_widget_Bluetooth_On->hide();
    //    ui->tts_widget->hide();
    ui->stackedWidget->setCurrentIndex(0);
}





//关闭USB、TCP、UDP编辑界面
//void SHGCMainSetup::on_btn_USBInput_2_clicked()
//{
//    ui->data_connect_widget_Default->show();
//    ui->data_connect_widget_UDP_On->hide();
//    ui->setup_widget->hide();
//    ui->data_connect_widget_USB_On->hide();
//    ui->data_connect_widget_TCP_On->hide();
//    ui->data_connect_widget_Bluetooth_On->hide();
//    ui->tts_widget->hide();
//}


//打开语音播报种类界面
void SHGCMainSetup::on_data_conn_log_14_click()
{
    //    ui->tts_widget->show();
    //    ui->data_connect_widget_Default->hide();
    //    ui->data_connect_widget_UDP_On->hide();
    //    ui->setup_widget->hide();
    //    ui->data_connect_widget_USB_On->hide();
    //    ui->data_connect_widget_TCP_On->hide();
    //    ui->data_connect_widget_Bluetooth_On->hide();
    ui->stackedWidget->setCurrentIndex(3);
}


//打开地图管理界面
void SHGCMainSetup::on_widget_79_click()
{

    //    ui->setup_widget->hide();
    //    ui->data_connect_widget_Default->hide();
    //    ui->dm_widget->show();
    //    ui->city_list_widget->hide();
    //    ui->tts_widget->hide();
    ui->stackedWidget->setCurrentIndex(4);
    ui->widget_down->hide();
    ui->widget_downing->hide();
    ui->btn_Download_1->setStyleSheet("background-color: rgb(255, 255, 255);color: rgb(0, 0, 0);font: 14pt;");
    ui->btn_Citylist_1->setStyleSheet("background-color: rgb(0, 170, 255);color: rgb(255, 255, 255);font: 14pt;");
}


//关闭USB、TCP、UDP编辑界面
//void SHGCMainSetup::on_btn_TCPInput_2_clicked()
//{
//    ui->data_connect_widget_Default->show();
//    ui->data_connect_widget_UDP_On->hide();
//    ui->setup_widget->hide();
//    ui->data_connect_widget_USB_On->hide();
//    ui->data_connect_widget_TCP_On->hide();
//    ui->data_connect_widget_Bluetooth_On->hide();
//    ui->tts_widget->hide();
//}
//关闭USB、TCP、UDP编辑界面
//void SHGCMainSetup::on_btn_UDPInput_2_clicked()
//{
//    ui->data_connect_widget_Default->show();
//    ui->data_connect_widget_UDP_On->hide();
//    ui->setup_widget->hide();
//    ui->data_connect_widget_USB_On->hide();
//    ui->data_connect_widget_TCP_On->hide();
//    ui->data_connect_widget_Bluetooth_On->hide();
//    ui->tts_widget->hide();
//}

//选择视频本地保存路径并显示在lineEdit中
void SHGCMainSetup::on_btn_saveVideoPath_clicked()
{
    QString dirPath = QDir::toNativeSeparators(QFileDialog::getExistingDirectory(this,tr("choosepath"),"/QGC-APP",QFileDialog::ShowDirsOnly
                                                                                 | QFileDialog::DontResolveSymlinks));
    ui->saveVideoPath->setText(dirPath);
}

//选择航线本地保存路径并显示在lineEdit中
void SHGCMainSetup::on_btn_saveMissonPath_clicked()
{
    QString dirPath = QDir::toNativeSeparators(QFileDialog::getExistingDirectory(this,tr("choosepath"),"/QGC-APP",QFileDialog::ShowDirsOnly
                                                                                 | QFileDialog::DontResolveSymlinks));
    ui->saveMissonPath->setText(dirPath);
}

//点击相应的默认按钮，播报间隔恢复到默认值10s
void SHGCMainSetup::on_btn_broadcast_tip_2_clicked()
{
    ui->voice_tip_slider_2->setValue(5);
    ui->voice_tip_slider_2->m_displayLabel->setText(QString::number(10)+"s");
    //    ui->voice_tip_slider_2->m_displayLabel->adjustSize();
    //    ui->voice_tip_slider_2->m_displayLabel->setScaledContents(true);
    ui->voice_tip_slider_2->m_displayLabel->move((ui->voice_tip_slider_2->width()-ui->voice_tip_slider_2->m_displayLabel->width()*5/4)*ui->voice_tip_slider_2->value()/(ui->voice_tip_slider_2->maximum()-ui->voice_tip_slider_2->minimum()),-1);

}


//点击相应的默认按钮，低电量提醒恢复到默认值20%
void SHGCMainSetup::on_btn_elec_tip_2_clicked()
{
    ui->elec_tip_slider_2->setValue(15);
    ui->elec_tip_slider_2->n_displayLabel->setText(QString::number(20)+"%");
    //    ui->elec_tip_slider_2->n_displayLabel->adjustSize();
    //    ui->elec_tip_slider_2->n_displayLabel->setScaledContents(true);
    ui->elec_tip_slider_2->n_displayLabel->move((ui->elec_tip_slider_2->width()-ui->elec_tip_slider_2->n_displayLabel->width()*5/4)*ui->elec_tip_slider_2->value()/(ui->elec_tip_slider_2->maximum()-ui->elec_tip_slider_2->minimum()),-1);
    ui->elec_tip_slider_2->n_displayLabel->setStyleSheet("color:#FE5723;");

}

//设置飞机失联后动作为返航
void SHGCMainSetup::on_btn_return_back_2_clicked()
{
    ui->wiget_return_back_2->setStyleSheet("background-image: url(:/images/resources/images/ui/SHGCSetUp/return.png);");
    ui->btn_return_back_2->setStyleSheet("QPushButton{background:url(); background-repeat: no-repeat;background-position:left;border:none;color: rgb(255, 255, 255);}");
    ui->wiget_go_on_2->setStyleSheet("background-image: url(:/images/resources/images/ui/SHGCSetUp/goon.png);");
    ui->btn_go_on_2->setStyleSheet("QPushButton{background:url(); background-repeat: no-repeat;background-position:left;border:none;color: rgb(3, 168, 247);}");

    ui->widget_continueFlyingExplain->hide();
    ui->widget_turnBackExplain->show();
}

//设置飞机失联后动作为继续飞行
void SHGCMainSetup::on_btn_go_on_2_clicked()
{
    ui->wiget_return_back_2->setStyleSheet("background-image: url(:/images/resources/images/ui/SHGCSetUp/goon.png);");
    ui->btn_return_back_2->setStyleSheet("QPushButton{background:url(); background-repeat: no-repeat;background-position:left;border:none;color: rgb(3, 168, 247);}");
    ui->wiget_go_on_2->setStyleSheet("background-image: url(:/images/resources/images/ui/SHGCSetUp/return.png);");
    ui->btn_go_on_2->setStyleSheet("QPushButton{background:url(); background-repeat: no-repeat;background-position:left;border:none;color: rgb(255, 255, 255);}");

    ui->widget_turnBackExplain->hide();
    ui->widget_continueFlyingExplain->show();
}

//设置返航高度
void SHGCMainSetup::on_return_height_sub_clicked()
{
    QString strOut = ui->return_height_2->text();
    int length = strOut.toInt();

    length--;
    if(length < 0)
    {
        length = 0;
    }

    QString strIn = QString::number(length);
    ui->return_height_2->setText(strIn);
}

void SHGCMainSetup::on_return_height_add_clicked()
{
    QString strOut = ui->return_height_2->text();
    int length = strOut.toInt();

    length++;

    QString strIn = QString::number(length);
    ui->return_height_2->setText(strIn);
}



////打开已下载
//void SHGCMainSetup::on_btn_downloaded_clicked()
//{

//    if(btn_downloading_clicked == true)
//    {
//        ui->widget_downing->show();
//    }
//    else
//    {
//        ui->widget_downing->hide();
//    }

//    if(btn_downloaded_clicked==false)
//    {
//        ui->widget_down->show();
//        QIcon button_ico(":/images/resources/images/ui/SHGCSetUp/listend.png");
//        ui->btn_downloaded->setIcon(button_ico);
//        ui->btn_downloading->setIconSize(QSize(50,50));
//        btn_downloaded_clicked=true;
//    }
//    else
//    {
//        ui->widget_down->hide();
//        QIcon button_ico(":/images/resources/images/ui/SHGCSetUp/");
//        ui->btn_downloaded->setIcon(button_ico);
//        ui->btn_downloaded->setIconSize(QSize(50,50));
//        btn_downloaded_clicked = false;
//    }
//}


////关闭地图管理界面
//void SHGCMainSetup::on_set_return_8_clicked()
//{
////    ui->dm_widget->hide();
////    ui->city_list_widget->hide();
////    ui->tts_widget->hide();
////    ui->data_connect_widget_Default->hide();
////    ui->setup_widget->show();
//    ui->stackedWidget->setCurrentIndex(0);
//}


//void SHGCMainSetup::on_set_return_9_clicked()
//{
////    ui->data_connect_widget_Default->hide();
////    ui->setup_widget->hide();
////    ui->dm_widget->show();
////    ui->city_list_widget->hide();
////    ui->tts_widget->hide();
//    ui->stackedWidget->setCurrentIndex(0);
//}


//点击下载管理按钮
void SHGCMainSetup::on_btn_Download_1_clicked()
{
    //    ui->data_connect_widget_Default->hide();
    //    ui->setup_widget->hide();
    //    ui->city_list_widget->hide();
    //    ui->tts_widget->hide();
    //    ui->dm_widget->show();
    ui->stackedWidget->setCurrentIndex(4);
    ui->btn_Download_1->setStyleSheet("background-color: rgb(255, 255, 255);color: rgb(0, 0, 0);font: 14pt;");
    ui->btn_Citylist_1->setStyleSheet("background-color: rgb(0, 170, 255);color: rgb(255, 255, 255);font: 14pt;");
}


//点击城市列表按钮
void SHGCMainSetup::on_btn_Citylist_1_clicked()
{
    ui->stackedWidget->setCurrentIndex(5);
    ui->btn_Citylist_2->setStyleSheet("background-color: rgb(255, 255, 255);color: rgb(0, 0, 0);font: 14pt;");
    ui->btn_Download_2->setStyleSheet("background-color: rgb(0, 170, 255);color: rgb(255, 255, 255);font: 14pt;");
    //    ui->data_connect_widget_Default->hide();
    //    ui->setup_widget->hide();
    //    ui->dm_widget->hide();
    //    ui->city_list_widget->show();
    //    ui->tts_widget->hide();

}


//点击下载管理
void SHGCMainSetup::on_btn_Download_2_clicked()
{
    //    ui->btn_Download_1->setStyleSheet("background-color: rgb(255, 255, 255);color: rgb(0, 0, 0);font: 14pt;");
    //    ui->btn_Citylist_1->setStyleSheet("background-color: rgb(0, 170, 255);color: rgb(255, 255, 255);font: 14pt;");
    //    ui->data_connect_widget_Default->hide();
    //    ui->setup_widget->hide();
    //    ui->city_list_widget->hide();
    //    ui->dm_widget->show();
    //    ui->tts_widget->hide();
    ui->stackedWidget->setCurrentIndex(4);
    ui->btn_Download_1->setStyleSheet("background-color: rgb(255, 255, 255);color: rgb(0, 0, 0);font: 14pt;");
    ui->btn_Citylist_1->setStyleSheet("background-color: rgb(0, 170, 255);color: rgb(255, 255, 255);font: 14pt;");

}


//点击城市列表
void SHGCMainSetup::on_btn_Citylist_2_clicked()
{
    //    ui->btn_Citylist_2->setStyleSheet("background-color: rgb(255, 255, 255);color: rgb(0, 0, 0);font: 14pt;");
    //    ui->btn_Download_2->setStyleSheet("background-color: rgb(0, 170, 255);color: rgb(0, 0, 0);font: 14pt;");
    //    ui->data_connect_widget_Default->hide();
    //    ui->setup_widget->hide();
    //    ui->dm_widget->hide();
    //    ui->city_list_widget->show();
    //    ui->tts_widget->hide();
    ui->stackedWidget->setCurrentIndex(5);
    ui->btn_Citylist_2->setStyleSheet("background-color: rgb(255, 255, 255);color: rgb(0, 0, 0);font: 14pt;");
    ui->btn_Download_2->setStyleSheet("background-color: rgb(0, 170, 255);color: rgb(255, 255, 255);font: 14pt;");
}


//点击已下载
void SHGCMainSetup::on_btn_download_clicked()
{
    if(btn_downing_clicked == true)
    {
        ui->widget_downing->show();
    }
    else
    {
        ui->widget_downing->hide();
    }

    if(btn_download_clicked==false)
    {
        ui->widget_down->show();
        QIcon button_ico(":/images/resources/images/ui/SHGCSetUp/listend.png");
        ui->btn_download->setIcon(button_ico);
        ui->btn_downing->setIconSize(QSize(50,50));
        btn_download_clicked=true;
    }
    else
    {
        ui->widget_down->hide();
        QIcon button_ico(":/images/resources/images/ui/SHGCSetUp/liststart.png");
        ui->btn_download->setIcon(button_ico);
        ui->btn_download->setIconSize(QSize(50,50));
        btn_download_clicked = false;
    }
}

//点击下载中
void SHGCMainSetup::on_btn_downing_clicked()
{
    if(btn_download_clicked == true)
    {
        ui->widget_down->show();
    }else{
        ui->widget_down->hide();
    }

    if(btn_downing_clicked == false)
    {
        ui->widget_downing->show();
        QIcon button_ico(":/images/resources/images/ui/SHGCSetUp/listend.png");
        ui->btn_downing->setIcon(button_ico);
        ui->btn_downing->setIconSize(QSize(50,50));
        btn_downing_clicked = true;
    }else{
        ui->widget_downing->hide();
        QIcon button_ico(":/images/resources/images/ui/SHGCSetUp/liststart.png");
        ui->btn_downing->setIcon(button_ico);
        ui->btn_downing->setIconSize(QSize(50,50));
        btn_downing_clicked = false;
    }
}


void SHGCMainSetup::WordList()
{
    //    查询省份数据库整张表，使用连接名ConnectMapDatabase获取该连接
    QSqlDatabase mdb= QSqlDatabase::database("ConnectMapDatabase");
    QSqlQuery query(mdb);
    qDebug() << tr("ConnectMapDatabase");
    query.exec("select * from t_address_province");
    query.first();
    while(query.next())
    {
        //        qDebug() << query.value(2).toStringList();
        provincelist << query.value(2).toStringList();
        qDebug() << provincelist;
    }
    ////    int pnumRows;


    //    查询城市数据库整张表，使用连接名ConnectionCDatabase获取该连接
    //    QSqlDatabase cdb= QSqlDatabase::database("ConnectionCDatabase");
    //    QSqlQuery queryc(cdb);

    //    if (cdb.open()) {
    //        qDebug() << "城市数据库连接成功";
    //    }

    //    queryc.exec("select * from t_address_city");
    //    queryc.next();
    //    while(queryc.next())
    //    {
    //        citylist << queryc.value(2).toStringList();
    //    }

    int numRows;

    // 先判断该数据库驱动是否支持QuerySize特性，如果支持，则可以使用size()函数，
    // 如果不支持，那么就使用其他方法来获取总行数
    if (mdb.driver()->hasFeature(QSqlDriver::QuerySize))
    {
        qDebug() << "has feature: query size";
        numRows = query.size();
    }
    else
    {
        qDebug() << "no feature: query size";
        query.last();
        numRows = query.at() + 1;
    }
    qDebug() << "省份数据库总行数: " << numRows;

    //    int i;

    query.seek(33);

    //     获取当前行的记录
    QSqlRecord record = query.record();

    //        获取索引为i的字段，即第i+1个字段
    QSqlField field = record.field(33);

    QString name = record.value("name").toString();

    //        qDebug() << name;
    qDebug() << field.value().toString();

    //        provincelist.append(field.value().toString());
    //        provincelist << name;

    //        QStringList m;
    //        m << "a" <<"b"<<"b"<<"c"<<"c"<<"c";
    //        qDebug() << m;



    //     指向索引为1的记录，即第二条记录
    //    query.seek(3);

    // 返回当前索引值
    qDebug() << "current index: " << query.at();

    //     获取当前行的记录
    //    QSqlRecord record = query.record();

    //     获取记录中“id”和“name”两个字段的值
    //    int id = record.value("id").toInt();
    //    QString name = record.value("name").toString();
    //    qDebug() << "id: " << id << "name: " << name;

    // 获取索引为2的字段，即第三个字段
    //   QSqlField field = record.field(2);

    // 输出字段名和字段值，结果为“name”和“MaLiang”
    //    qDebug() << "second field: " << field.name()
    //             << "field value: " << field.value().toString();


    //    查询乡镇数据库整张表，使用连接名ConnectionTDatabase获取该连接
    //    QSqlDatabase tdb= QSqlDatabase::database("ConnectionTDatabase");
    //    QSqlQuery queryt(tdb);
    //    if (tdb.open()) {
    //        qDebug() << "乡镇数据库连接成功";
    //    }
    //    queryt.exec("select * from t_address_town");
    //    while(queryt.next())
    //    {
    //        qDebug() << queryt.value(2).toStringList();
    //    }

    //    query.exec("select * from table t_address_province");



    //    QVariantList ids;

    //    QVariantList names;


    //    provinceList <<"北京市"<<"天津市"<<"河北省"<<"山西省"<<"内蒙古自治区"<<"辽宁省"<<"吉林省"<<"黑龙江省"<<"上海市"
    //                <<"江苏省"<<"浙江省"<<"安徽省"<<"福建省"<<"江西省"<<"山东省"<<"河南省"<<"湖北省"<<"湖南省"<<"广东省"
    //               <<"广西壮族自治区"<<"海南省"<<"重庆市"<<"四川省"<<"贵州省"<<"云南省"<<"西藏自治区"<<"陕西省"<<"甘肃省"
    //              <<"青海省"<<"宁夏回族自治区"<<"新疆维吾尔自治区"<<"台湾省"<<"香港特别行政区"<<"澳门特别行政区";
    //    QSqlQuery q("SELECT * FROM t_address_province");

    //    QSqlRecord rec = q.record();

    //    while(query.next())
    //    {
    //       provinceList = query.value(2).toString();

    //       ProvinceList.append(provinceList);


    ////       ProvinceList<<provinceList;

    //       qDebug()<< ProvinceList;
    //    }

    //    ProvinceList<<provinceList;

    //    qDebug()<< provinceList;



    //    query.exec("SELECT * FROM t_address_city");

    //    while(query.next())
    //    {
    //       cityList = query.value(2).toString();

    //       CityList<<cityList;

    ////       qDebug()<< CityList;
    //    }

    //    query.exec("SELECT * FROM t_address_town");

    //    while(query.next())
    //    {
    //       townList = query.value(2).toString();

    //       TownList<<townList;

    ////      qDebug()<< TownList;
    //    }

    //    QSqlQuery query(mdb);

    ////    provinceList <<"北京市"<<"天津市"<<"河北省"<<"山西省"<<"内蒙古自治区"<<"辽宁省"<<"吉林省"<<"黑龙江省"<<"上海市"
    ////                <<"江苏省"<<"浙江省"<<"安徽省"<<"福建省"<<"江西省"<<"山东省"<<"河南省"<<"湖北省"<<"湖南省"<<"广东省"
    ////               <<"广西壮族自治区"<<"海南省"<<"重庆市"<<"四川省"<<"贵州省"<<"云南省"<<"西藏自治区"<<"陕西省"<<"甘肃省"
    ////              <<"青海省"<<"宁夏回族自治区"<<"新疆维吾尔自治区"<<"台湾省"<<"香港特别行政区"<<"澳门特别行政区";


    //       ProvinceList.append(provinceList);


    //       ProvinceList<<provinceList;

    //       qDebug()<< provincelist;
    //    }

    //    ProvinceList<<provinceList;

    //    qDebug()<< provinceList;



    //    queryc.exec("SELECT * FROM t_address_city");

    //    while(queryc.next())
    //    {
    //       citylist = queryc.value(2).toString();

    ////       CityList<<cityList;

    //       qDebug()<< citylist;
    //    }

    //    queryt.exec("SELECT * FROM t_address_town");

    //    while(queryt.next())
    //    {
    //       townlist = query.value(2).toString();

    ////       TownList<<townList;

    ////      qDebug()<< TownList;
    //    }

    //    mdb.close();
    //    cityList<<query.value(2).toStringList();
    //    QString name = query.value(2).toString();
    //    QString name ="湖北省 广东省 ";
    //    provincelist << QString("name:%1 ").arg(name);
    //    citylist << QString("name:%1 ").arg(name);
    //    townlist << QString("name:%1").arg(name);
    //    query.exec("SELECT * FROM t_address_province WHERE name");
    //    while(query.next())
    //    {
    //        QString name = query.value(2).toString();

    //        provincelist << QString("name:%1 ").arg(name);

    //        qDebug()<< provincelist;
    //    }

    //    query.exec("SELECT * FROM t_address_city WHERE name");
    //    while(query.next())
    //    {
    //        QString name = query.value(2).toString();

    //        citylist.append(QString("name:%1 ").arg(name));
    //    }

    //    query.exec("SELECT * FROM t_address_town WHERE name");
    //    while(query.next())
    //    {
    //        QString name = query.value(2).toString();

    //        townlist << QString("name:%1").arg(name);
    //    }
    //    query.execBatch();
}


//点击搜索按钮槽函数
void SHGCMainSetup::on_pSearchButton_clicked()
{
    QString strText=ui->m_pSearchLineEdit->text();


}


void SHGCMainSetup::on_m_pSearchLineEdit_textEdited(const QString &)
{
    WordList();
    QCompleter *completer = new QCompleter(provincelist);
    ui->m_pSearchLineEdit->setCompleter(completer);
}

void SHGCMainSetup::resizeEvent(QResizeEvent *event)
{
    m_autoResizeHandler->doAutoResize();
}

//选择流测仪、水位计连接方式 界面
void SHGCMainSetup::on_data_conn_log_FV_WLG_click()
{
    ui->stackedWidget->setCurrentIndex(2);
    ui->widget_USB_2->setStyleSheet("background-color: rgb(255, 255, 255);");

    //    //流速计波特率 默认 19200
    //    ui->VM_BaudRate->setCurrentIndex(4);
    //    //水位计波特率 默认 9600
    //    ui->WLG_BaudRate->setCurrentIndex(3);

    //    //远程TCP服务器IP 默认 219.128.125.86
    //    ui->TCP_IP_FV_WLG->setText("219.128.125.86");
    //    //远程TCP服务器端口 默认 61111
    //    ui->TCP_Port_FV_WLG->setText("61111");


    //加载设置参数配置
//    loadSettingParameters();

    //设置流速仪串口号
    disconnect(ui->VM_SerialPort,SIGNAL(currentTextChanged(QString)),this,SIGNAL(sendToVMcomPort(QString)));
    connect(ui->VM_SerialPort,SIGNAL(currentTextChanged(QString)),this,SIGNAL(sendToVMcomPort(QString)));

    //设置流速仪波特率
    disconnect(ui->VM_BaudRate,SIGNAL(currentTextChanged(QString)),this,SIGNAL(sendToVMcomRate(QString)));
    connect(ui->VM_BaudRate,SIGNAL(currentTextChanged(QString)),this,SIGNAL(sendToVMcomRate(QString)));

    //设置水位计串口号
    disconnect(ui->WLG_SerialPort,SIGNAL(currentTextChanged(QString)),this,SIGNAL(sendToWLGcomPort(QString)));
    connect(ui->WLG_SerialPort,SIGNAL(currentTextChanged(QString)),this,SIGNAL(sendToWLGcomPort(QString)));

    //设置水位计波特率
    disconnect(ui->WLG_BaudRate,SIGNAL(currentTextChanged(QString)),this,SIGNAL(sendToWLGcomRate(QString)));
    connect(ui->WLG_BaudRate,SIGNAL(currentTextChanged(QString)),this,SIGNAL(sendToWLGcomRate(QString)));

    //设置远程TCP服务器IP
    disconnect(ui->TCP_IP_FV_WLG,SIGNAL(textChanged(QString)),this,SIGNAL(sendToVM_WLGIP(QString)));
    connect(ui->TCP_IP_FV_WLG,SIGNAL(textChanged(QString)),this,SIGNAL(sendToVM_WLGIP(QString)));

    //设置远程TCP服务器端口
    disconnect(ui->TCP_Port_FV_WLG,SIGNAL(textChanged(QString)),this,SIGNAL(sendToVM_WLGPort(QString)));
    connect(ui->TCP_Port_FV_WLG,SIGNAL(textChanged(QString)),this,SIGNAL(sendToVM_WLGPort(QString)));

    //设置是否为调试模式
    connect(this->ui->debugMode_switch,SIGNAL(checkedChanged(bool)),this,SIGNAL(sendToFV_WLG_DebugMode(bool)));
}

//流测仪、水位计连接方式 界面 返回按钮被按下
void SHGCMainSetup::on_set_return_6_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}

void SHGCMainSetup::preservingSettingParameters()
{
    //创建保存ini文件的目录
    bool ok;
    QString str = QCoreApplication::applicationDirPath();
    qDebug()<<"str dir:"<<str;
    QDir dir(str + "/ini");
    if(dir.exists())
    {
        ok = true;
        qDebug() << "dir exist!";
    }
    else
    {
        ok = dir.mkdir(str + "/ini");//只创建一级子目录，即必须保证上级目录存在
        qDebug() << "dir not exists, generate!";
    }

    QString path = "ini/SettingParameters.ini";
    QSettings settings(path, QSettings::IniFormat);
    settings.beginGroup("UAVOption");
    settings.setValue("UAV_SerialPortFromUSB", ui->aircraft_SerialPort->currentText());
    settings.setValue("UAV_BaudRateFromUSB", ui->aircraft_BaudRate->currentText());
    settings.setValue("UAV_IPFromTCP", ui->TCP_IP_UAV->text());
    settings.setValue("UAV_PortFromTCP",ui->TCP_Port_UAV->text());
    settings.setValue("UAV_IPFromUDP",ui->UDP_IP_UAV->text());
    settings.setValue("UAV_PortFromUDP",ui->UDP_Port_UAV->text());
    settings.setValue("UAV_Number",ui->UAV_Number->text());
    settings.endGroup();
    settings.beginGroup("VM_WLGOption");
    settings.setValue("VM_SerialPortFromUSB",ui->VM_SerialPort->currentText());
    settings.setValue("VM_BaudRateFromUSB",ui->VM_BaudRate->currentText());
    settings.setValue("WLG_SerialPortFromUSB",ui->WLG_SerialPort->currentText());
    settings.setValue("WLG_BaudRateFromUSB",ui->WLG_BaudRate->currentText());
    settings.setValue("IP_FV_WLGFromTCP",ui->TCP_IP_FV_WLG->text());
    settings.setValue("Port_FV_WLGFromTCP",ui->TCP_Port_FV_WLG->text());
    settings.setValue("FV_WLG_DebugMode",ui->debugMode_switch->getChecked());
    settings.setValue("FV_WLG_MaxDeviationValue",ui->MaxDeviationValue_Edit->text());
    settings.endGroup();
}

void SHGCMainSetup::loadSettingParameters()
{
    QString path ="ini/SettingParameters.ini";
    QSettings settings(path, QSettings::IniFormat);

    UAV_SerialPort = settings.value("UAVOption/UAV_SerialPortFromUSB").toString();
    UAV_BaudRate = settings.value("UAVOption/UAV_BaudRateFromUSB").toString();
    UAV_TCP_IP = settings.value("UAVOption/UAV_IPFromTCP").toString();
    UAV_TCP_Port = settings.value("UAVOption/UAV_PortFromTCP").toString();
    UAV_UDP_IP = settings.value("UAVOption/UAV_IPFromUDP").toString();
    UAV_UDP_Port = settings.value("UAVOption/UAV_PortFromUDP").toString();
    UAV_Number = settings.value("UAVOption/UAV_Number").toString();

    VM_SerialPort = settings.value("VM_WLGOption/VM_SerialPortFromUSB").toString();
    VM_BaudRate = settings.value("VM_WLGOption/VM_BaudRateFromUSB").toString();
    WLG_SerialPort = settings.value("VM_WLGOption/WLG_SerialPortFromUSB").toString();
    WLG_BaudRate = settings.value("VM_WLGOption/WLG_BaudRateFromUSB").toString();
    FV_WLG_TCP_IP = settings.value("VM_WLGOption/IP_FV_WLGFromTCP").toString();
    FV_WLG_TCP_Port = settings.value("VM_WLGOption/Port_FV_WLGFromTCP").toString();
    str_FV_WLG_DebugMode = settings.value("VM_WLGOption/FV_WLG_DebugMode").toString();
    str_FV_WLG_MaxDeviationValue = settings.value("VM_WLGOption/FV_WLG_MaxDeviationValue").toString();

    if(UAV_SerialPort != nullptr)
    {
         ui->aircraft_SerialPort->setCurrentText(UAV_SerialPort);
    }
    if(UAV_BaudRate != nullptr)
    {
        ui->aircraft_BaudRate->setCurrentText(UAV_BaudRate);
    }
    if(UAV_TCP_IP != nullptr)
    {
        ui->TCP_IP_UAV->setText(UAV_TCP_IP);
    }
    if(UAV_TCP_Port != nullptr)
    {
        ui->TCP_Port_UAV->setText(UAV_TCP_Port);
    }
    if(UAV_UDP_IP != nullptr)
    {
        ui->UDP_IP_UAV->setText(UAV_UDP_IP);
    }
    if(UAV_UDP_Port != nullptr)
    {
        ui->UDP_Port_UAV->setText(UAV_UDP_Port);
    }
    if(UAV_Number != nullptr)
    {
        ui->UAV_Number->setText(UAV_Number);
    }



    if(VM_SerialPort != nullptr)
    {
        ui->VM_SerialPort->setCurrentText(VM_SerialPort);
    }
    if(VM_BaudRate != nullptr)
    {
        ui->VM_BaudRate->setCurrentText(VM_BaudRate);
    }
    if(WLG_SerialPort != nullptr)
    {
        ui->WLG_SerialPort->setCurrentText(WLG_SerialPort);
    }
    if(WLG_BaudRate != nullptr)
    {
        ui->WLG_BaudRate->setCurrentText(WLG_BaudRate);
    }
    if(FV_WLG_TCP_IP != nullptr)
    {
        ui->TCP_IP_FV_WLG->setText(FV_WLG_TCP_IP);
    }
    if(FV_WLG_TCP_Port != nullptr)
    {
        ui->TCP_Port_FV_WLG->setText(FV_WLG_TCP_Port);
    }
    if(str_FV_WLG_DebugMode != nullptr)
    {
        if(str_FV_WLG_DebugMode == "true")
        {
            bool_FV_WLG_DebugMode = true;
        }
        else if(str_FV_WLG_DebugMode == "false")
        {
            bool_FV_WLG_DebugMode = false;
        }
        update();
    }
    if(str_FV_WLG_MaxDeviationValue != nullptr)
    {
        ui->MaxDeviationValue_Edit->setText(str_FV_WLG_MaxDeviationValue);
    }
}

void SHGCMainSetup::sendSettingParameters()
{
    //加载设置参数配置
//    loadSettingParameters();

    emit sendToAircraftcomPort(ui->aircraft_SerialPort->currentText());
    emit sendToAircraftcomRate(ui->aircraft_BaudRate->currentText());
    emit sendToAircraftNumber(ui->UAV_Number->text());

    emit sendToVMcomPort(ui->VM_SerialPort->currentText());
    emit sendToVMcomRate(ui->VM_BaudRate->currentText());
    emit sendToWLGcomPort(ui->WLG_SerialPort->currentText());
    emit sendToWLGcomRate(ui->WLG_BaudRate->currentText());
    emit sendToVM_WLGIP(ui->TCP_IP_FV_WLG->text());
    emit sendToVM_WLGPort(ui->TCP_Port_FV_WLG->text());
    emit sendToFV_WLG_DebugMode(bool_FV_WLG_DebugMode);
}

//void SHGCMainSetup::AutomaticAdditionSerialPort()
//{
//    //扫描存在的串口
//    qDebug()<<"扫描存在的串口:";
//    ui->VM_SerialPort->clear();
//    ui->WLG_SerialPort->clear();
//    foreach(const QSerialPortInfo &info,QSerialPortInfo::availablePorts())
//    {
//        QSerialPort ser;
//        ser.setPort(info);
//        /*判断串口能否打开*/
//        if(ser.open(QIODevice::ReadWrite))
//        {
//            ui->aircraft_SerialPort->addItem(ser.portName());
//            ui->VM_SerialPort->addItem(ser.portName());
//            ui->WLG_SerialPort->addItem(ser.portName());
//        }
//        ser.close();
//    }
//}


void SHGCMainSetup::on_tcpLink_clicked()
{
    QString Ip = ui->TCP_IP_UAV->text();
    QString Port = ui->TCP_Port_UAV->text();

    emit LinkTcp(Ip,Port);
}

void SHGCMainSetup::on_UDPLink_clicked()
{
    QString ip = ui->UDP_IP_UAV->text();
    QString port = ui->UDP_Port_UAV->text();
    emit linkUdp(ip,port);
}

void SHGCMainSetup::on_serialLink_clicked()
{
    QString port = ui->aircraft_SerialPort->currentText();
    QString baud = ui->aircraft_BaudRate->currentText();
    emit linkSerial(port,baud);
}
