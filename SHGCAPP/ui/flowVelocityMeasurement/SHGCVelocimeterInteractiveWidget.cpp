#include "SHGCVelocimeterInteractiveWidget.h"
#include "ui_SHGCVelocimeterInteractiveWidget.h"

//#include "qgcDataLink/qgcvmdatacom.h"
//#include "qgcapplicationtipswidget.h"

#include <QCheckBox>
#include <QDebug>

#include <QCoreApplication>

#include <QFile>
#include <QDebug>
#include <QTimer>

#include <QDir>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonParseError>
#include <QJsonValue>

#include <QSysInfo>
#include <algorithm>
#include <cstring>

#include <QDateTime>

#include <cstdlib>
#include <iostream>
#include <fstream>

#include <QFileDialog>

using namespace std;

bool SHGCVelocimeterInteractiveWidget::checkBoxCHE = false;

static SHGCVelocimeterInteractiveWidget* _instance = nullptr;

SHGCVelocimeterInteractiveWidget* SHGCVelocimeterInteractiveWidget::_create(QWidget *parent)
{
    _instance = new SHGCVelocimeterInteractiveWidget(parent);
    return _instance;
}

SHGCVelocimeterInteractiveWidget* SHGCVelocimeterInteractiveWidget::instance(void)
{
    return _instance;
}

void SHGCVelocimeterInteractiveWidget::deleteInstance(void)
{
    delete this;
}

SHGCVelocimeterInteractiveWidget::SHGCVelocimeterInteractiveWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SHGCVelocimeterInteractiveWidget)
{
    ui->setupUi(this);

    //    ui->tips_label->setStyleSheet("color: rgb(51,51,51); font: 16px \"思源黑体\"");

    initqTimer();//初始化定时器
    initTableWidget_realTime();//初始化表格属性
    initTableWidget_FV();//初始化表格属性
    //    initTableWidget_WLG();//初始化表格属性

    //    getDeviceNum();//获取无人机设备编号
    connect(_SHGCMainSetup,SIGNAL(sendToAircraftNumber(QString)),this,SLOT(getDeviceNum(QString)));

    //设置是否为调试模式
    connect(_SHGCMainSetup,SIGNAL(sendToFV_WLG_DebugMode(bool)),this,SLOT(setFV_WLG_DebugMode(bool)));

    //接受传过来的流速最大偏差值
    connect(_SHGCMainSetup,SIGNAL(sendMaxDeviationValue(QString)),this,SLOT(acceptMaxDeviationValue(QString)));

    loadSettingParameters();//加载设置相关配置文件 获取数据过滤系数


    m_autoResizeHandler=new AutoResize(this,this->rect().width(),this->rect().height());
    m_autoResizeHandler->setAutoResizeFlag(
                AutoResize::INCLUDE_VELOCIMETERINTERACTIVE|
                AutoResize::INCLUDE_WIDGET|
                AutoResize::INCLUDE_PUSHBUTTON|
                AutoResize::INCLUDE_CHECKBOX|
                AutoResize::INCLUDE_LABEL|
                AutoResize::INCLUDE_LINEEDIT|
                AutoResize::INCLUDE_TABLEWIDGET
                );
    m_autoResizeHandler->pushAllResizeItem();

    //设置tcp连接的IP和Port
    connect(_SHGCMainSetup,SIGNAL(sendToVM_WLGIP(QString)),this,SLOT(changeIp(QString)));
    connect(_SHGCMainSetup,SIGNAL(sendToVM_WLGPort(QString)),this,SLOT(changePort(QString)));
    //若"雷达水流测速仪"交互页面 被展开 则tcp客户端建立连接
    disconnect(_MainWindow,SIGNAL(SHGCVelocimeterInteractiveWidget_isShow()),this,SLOT(tcpClientConnecting()));
    connect(_MainWindow,SIGNAL(SHGCVelocimeterInteractiveWidget_isShow()),this,SLOT(tcpClientConnecting()));

    initDisplayHistoricalData();//初始化时展示历史数据

    //    connect(_MianWindow,SIGNAL(receiveMeasureStates_fromWLG(bool)),this,SLOT(whetherToPopUpPrompts(bool)));

    /*************************水流测速仪*****************************/
    //更改测速仪测量按钮状态
    connect(_MainWindow,SIGNAL(FV_pushButtonUnableToClick()),this,SLOT(setFV_PushButtonUnableToClick()));
    connect(_MainWindow,SIGNAL(FV_pushButtonClickable()),this,SLOT(setFV_pushButtonClickable()));

    connect(_MainWindow, SIGNAL(openVMDataComSuccess()),this,SLOT(changeBackground_blue()));
    connect(_MainWindow, SIGNAL(openVMDataComFail()),this,SLOT(changeBackground_red()));

    //初始化表格属性
    //    connect(p, SIGNAL(getTheLatestValue(bool)),this,SLOT(initTableWidget_FV()));
    //解析临时JSON文件 生成正确的JSON格式文件
    connect(_MainWindow, SIGNAL(getTheLatestValue(bool)),this,SLOT(handleFVJSONFile(bool)));
    //读取JSON文件数据 并展示数据到 QTableWidget
    disconnect(_MainWindow, SIGNAL(getTheLatestValue(bool)),this,SLOT(analysisFVJSONFile(bool)));
    connect(_MainWindow, SIGNAL(getTheLatestValue(bool)),this,SLOT(analysisFVJSONFile(bool)));
    //展示数据到 QTableWidget_realTime
    disconnect(_MainWindow, SIGNAL(getTheLatestValue(bool)),this,SLOT(setFVTheLatestValue_forTableWidget_realTime(bool)));
    connect(_MainWindow, SIGNAL(getTheLatestValue(bool)),this,SLOT(setFVTheLatestValue_forTableWidget_realTime(bool)));
    //展示数据到 QTableWidget
    //    connect(p, SIGNAL(getTheLatestValue(bool)),this,SLOT(setFVTheLatestValue_forTableWidget(bool)));
    //若文件超过10天 则清空
    connect(_MainWindow, SIGNAL(getTheLatestValue(bool)),this,SLOT(clearFVJSONFile(bool)));

    /*************************水位计*****************************/
    //更改水位计测量按钮状态
    connect(_MainWindow,SIGNAL(WLG_pushButtonUnableToClick()),this,SLOT(setWLG_pushButtonUnableToClick()));
    connect(_MainWindow,SIGNAL(WLG_pushButtonClickable()),this,SLOT(setWLG_pushButtonClickable()));

    connect(_MainWindow, SIGNAL(openWLGDataComSuccess()),this,SLOT(changeWLGBackground_blue()));
    connect(_MainWindow, SIGNAL(openWLGDataComFail()),this,SLOT(changeWLGBackground_red()));

    //初始化表格属性
    //    connect(p, SIGNAL(WLG_getTheLatestValue(bool)),this,SLOT(initTableWidget_WLG()));
    //解析临时JSON文件 生成正确的JSON格式文件
    connect(_MainWindow, SIGNAL(WLG_getTheLatestValue(bool)),this,SLOT(handleWLGJSONFile(bool)));
    //读取JSON文件数据
    connect(_MainWindow, SIGNAL(WLG_getTheLatestValue(bool)),this,SLOT(analysisWLGJSONFile(bool)));
    //展示数据到 QLable_realTime
    connect(_MainWindow, SIGNAL(WLG_getTheLatestValue(bool)),this,SLOT(setWLGTheLatestValue_forLable_realTime(bool)));
    //若文件超过10天 则清空
    connect(_MainWindow, SIGNAL(WLG_getTheLatestValue(bool)),this,SLOT(clearWLGJSONFile(bool)));


    QFrame *frame1 = new QFrame(this);
    frame1->setFrameShape(QFrame::HLine);        // 水平分割线
    frame1->setFrameShadow(QFrame::Sunken);
    frame1->setStyleSheet("border:0.5px solid rgb(139,139,139);");
    frame1-> setGeometry(QRect(10,77,465,1));    //设置默认尺寸

    QFrame *frame2 = new QFrame(this);
    frame2->setFrameShape(QFrame::HLine);        // 水平分割线
    frame2->setFrameShadow(QFrame::Sunken);
    frame2->setStyleSheet("border:0.5px solid rgb(139,139,139);");
    frame2-> setGeometry(QRect(10,175,465,1));    //设置默认尺寸

    //当窗口打开后，默认 checkBox为未勾选状态，lineEdit为不可输入状态
    ui->checkBox->setCheckState(Qt::Unchecked);
    ui->time_lineEdit->setEnabled(false);

    //设置lineEdit属性
    ui->time_lineEdit->setAlignment(Qt::AlignCenter);
    ui->time_lineEdit->setStyleSheet("background-image: url(:/images/resources/images/ui/flowVelocityMeasurement/inputBoxNot.png);"
                                     "color: rgb(51,51,51); font: 18px \"思源黑体\"");
    // 这个行编辑只接受从1到600的整数
    QRegExp regExp("^([1-9]|[6-9][0-9]|[1-5][0-9][0-9]|600)$");
    ui->time_lineEdit->setValidator(new QRegExpValidator( regExp, this ));//第一、第二个参数为范围，第三个参数为父对象，指定父对象Qt会自动回收空间
    ui->time_lineEdit->setText("1");
}


SHGCVelocimeterInteractiveWidget::~SHGCVelocimeterInteractiveWidget()
{
    if(tcpsocket->isOpen())
    {
        tcpClientDisConnecting();//tcp客户端断开连接
    }

    _instance = nullptr;

    if (qtimer->isActive())
    {
        qtimer->stop();
        delete qtimer;
        qtimer = nullptr;
    }

    delete ui;
}

//void qgcVelocimeterInteractiveWidget::whetherToPopUpPrompts(bool b)
//{
//    tipsStates = b;
//}

//void qgcVelocimeterInteractiveWidget::TextInitializationOK(bool b)
//{
//    textOK = b;
//}

void SHGCVelocimeterInteractiveWidget::acceptMaxDeviationValue(QString value)
{
    d_FV_WLG_MaxDeviationValue = value.toDouble();
}

void SHGCVelocimeterInteractiveWidget::initDisplayHistoricalData()
{
    analysisFVJSONFile(true);
//        setFVTheLatestValue_forTableWidget_realTime(true);//展示数据到 QTableWidget_realTime
    setDisplayHistoricalData("jLogs/flowVelocity.json");

    //    analysisWLGJSONFile(true);
    //    setWLGTheLatestValue_forLable_realTime(true);
}

/*************************水流测速仪*****************************/

//更改测速仪测量按钮状态 不可点击
void SHGCVelocimeterInteractiveWidget::setFV_PushButtonUnableToClick()
{
    ui->connectFV_pushButton->setEnabled(false);
    ui->connectFV_pushButton->setStyleSheet("border-image: url(:/images/resources/images/ui/flowVelocityMeasurement/CantClick_FV.png);");
}

//更改测速仪测量按钮状态 可点击
void SHGCVelocimeterInteractiveWidget::setFV_pushButtonClickable()
{
    ui->connectFV_pushButton->setEnabled(true);
    ui->connectFV_pushButton->setStyleSheet("QPushButton{border-image: url(:/images/resources/images/ui/flowVelocityMeasurement/connectionSuccessfulAndSend.png);}"
                                            "QPushButton:hover{border-image: url(:/images/resources/images/ui/flowVelocityMeasurement/connectionSuccessfulAndSend.png);}"
                                            "QPushButton:pressed{border-image: url(:/images/resources/images/ui/flowVelocityMeasurement/connectionSuccessfulAndSend_2.png);}");
}

void SHGCVelocimeterInteractiveWidget::initTableWidget_FV()
{ 
    //关闭tableWidget_WLG
    ui->tableWidget_WLG->close();
    //显示tableWidget_FV
    ui->tableWidget_FV->show();

    //    ui->tableWidget_FV->resize(600, h_desktop);//设置初始化时的表格大小
    ui->tableWidget_FV->setColumnCount(6); //设置列数为6
    ui->tableWidget_FV->setRowCount(200); //设置行数为200

    //记录字符串到字符串列表
    QStringList listHeaders;
    //添加字符串到字符串列表
    listHeaders << tr("序号") << tr("时间") << tr("流速原始\n值 m/s") << tr("校准\n系数")
                << tr("流速校准\n值 m/s") << tr("经纬度");
    //设置水平表头
    ui->tableWidget_FV->setHorizontalHeaderLabels(listHeaders);

    //隐藏列表头
    ui->tableWidget_FV->verticalHeader()->setVisible(false);
    //设置行表头及其字体的颜色、网格线颜色
    ui->tableWidget_FV->horizontalHeader()->setStyleSheet( "QHeaderView::section {background-color: transparent;"
                                                           "font:12pt 'Source Han Sans CN';color: rgb(181,181,181);"
                                                           "border:0px solid transparent;}");

    ui->tableWidget_FV->setFrameShape(QFrame::NoFrame); //设置无边框
    ui->tableWidget_FV->setShowGrid(false); //不显示格子线

    //    ui->tableWidget_FV->horizontalHeader()->setStretchLastSection(true); //设置表头充满表格的宽度
    ui->tableWidget_FV->horizontalHeader()->setFixedHeight(50); //设置表头的高度为50
    ui->tableWidget_FV->verticalHeader()->setDefaultSectionSize(50); //设置行高为50

    ui->tableWidget_FV->setEditTriggers(QAbstractItemView::NoEditTriggers); //设置不可编辑
    ui->tableWidget_FV->setSelectionMode(QAbstractItemView::NoSelection); //设置不可多选 (Ctrl、Shift和Ctrl+A)
    ui->tableWidget_FV->horizontalHeader()->setHighlightSections(false); //点击表时不对表头行高亮（获取焦点）

    ui->tableWidget_FV->setFont(QFont("Source Han Sans CN",12,3));//设置字体

    ui->tableWidget_FV->setColumnWidth(0,55);//设置第一列的列宽
    ui->tableWidget_FV->setColumnWidth(1,105);//设置第二列的列宽
    ui->tableWidget_FV->setColumnWidth(2,70);//设置第三列的列宽
    ui->tableWidget_FV->setColumnWidth(3,60);//设置第四列的列宽
    ui->tableWidget_FV->setColumnWidth(4,70);//设置第五列的列宽
    ui->tableWidget_FV->setColumnWidth(5,105);//设置第六列的列宽
}

void SHGCVelocimeterInteractiveWidget::initTableWidget_realTime()
{
    ui->tableWidget_realTime->setColumnCount(5); //设置列数为6
    ui->tableWidget_realTime->setRowCount(1); //设置行数为1

    //记录字符串到字符串列表
    QStringList listHeaders;
    //添加字符串到字符串列表
    listHeaders << tr("时间") << tr("流速原始\n值 m/s") << tr("校准\n系数")
                << tr("流速校准\n值 m/s") << tr("经纬度");
    //设置水平表头
    ui->tableWidget_realTime->setHorizontalHeaderLabels(listHeaders);

    //隐藏列表头
    ui->tableWidget_realTime->verticalHeader()->setVisible(false);
    //设置行表头及其字体的颜色、网格线颜色
    ui->tableWidget_realTime->horizontalHeader()->setStyleSheet( "QHeaderView::section {background-color: transparent;"
                                                                 "font:12pt 'Source Han Sans CN';color: rgb(181,181,181);"
                                                                 "border:0px solid transparent;}");

    ui->tableWidget_realTime->setFrameShape(QFrame::NoFrame); //设置无边框
    ui->tableWidget_realTime->setShowGrid(false); //不显示格子线

    //    ui->tableWidget_realTime->horizontalHeader()->setStretchLastSection(true); //设置表头充满表格的宽度
    ui->tableWidget_realTime->horizontalHeader()->setFixedHeight(45); //设置表头的高度为45
    ui->tableWidget_realTime->verticalHeader()->setDefaultSectionSize(45); //设置行高为45

    ui->tableWidget_realTime->setEditTriggers(QAbstractItemView::NoEditTriggers); //设置不可编辑
    ui->tableWidget_realTime->setSelectionMode(QAbstractItemView::NoSelection); //设置不可多选 (Ctrl、Shift和Ctrl+A)
    ui->tableWidget_realTime->horizontalHeader()->setHighlightSections(false); //点击表时不对表头行高亮（获取焦点）

    ui->tableWidget_realTime->setFont(QFont("Source Han Sans CN",13,63));//设置字体

    ui->tableWidget_realTime->setColumnWidth(0,110);//设置第1列的列宽
    ui->tableWidget_realTime->setColumnWidth(1,80);//设置第2列的列宽
    ui->tableWidget_realTime->setColumnWidth(2,75);//设置第3列的列宽
    ui->tableWidget_realTime->setColumnWidth(3,80);//设置第4列的列宽
    ui->tableWidget_realTime->setColumnWidth(4,120);//设置第5列的列宽

    //    if( ui->tableWidget_realTime->item(0,0) != nullptr ||
    //            ui->tableWidget_realTime->item(0,1) != nullptr ||
    //            ui->tableWidget_realTime->item(0,2) != nullptr ||
    //            ui->tableWidget_realTime->item(0,3) != nullptr ||
    //            ui->tableWidget_realTime->item(0,4) != nullptr)
    //    {
    //        ui->tableWidget_realTime->clearContents();//清空表格中的内容（不包含表头）
    //    }
}


void SHGCVelocimeterInteractiveWidget::changeBackground_red()
{
    ui->connectFV_pushButton->setStyleSheet("border-image: url(:/images/resources/images/ui/flowVelocityMeasurement/Unconnected.png);");
    FV_connectOK = false;
}

void SHGCVelocimeterInteractiveWidget::changeBackground_blue()
{
    //    ui->connectFV_widget->setStyleSheet("background-image: url(:/images/resources/images/ui/flowVelocityMeasurement/connectionSuccessfulAndSend.png);}");

    ui->connectFV_pushButton->setStyleSheet("QPushButton{border-image: url(:/images/resources/images/ui/flowVelocityMeasurement/connectionSuccessfulAndSend.png);}"
                                            "QPushButton:hover{border-image: url(:/images/resources/images/ui/flowVelocityMeasurement/connectionSuccessfulAndSend.png);}"
                                            "QPushButton:pressed{border-image: url(:/images/resources/images/ui/flowVelocityMeasurement/connectionSuccessfulAndSend_2.png);}");
    FV_connectOK = true;
}

void SHGCVelocimeterInteractiveWidget::on_connectFV_pushButton_clicked()
{
    if(FV_connectOK == false)
    {
        emit connectFV_pushButton_isClicked_conn();//连接
    }
    else
    {
        emit connectFV_pushButton_isClicked_send();//发送
    }
}

void SHGCVelocimeterInteractiveWidget::on_checkBox_stateChanged(int arg1)
{
    if (arg1 == Qt::Checked)//选中
    {
        ui->time_lineEdit->setEnabled(true);
        ui->time_lineEdit->setStyleSheet("background-image: url(:/images/resources/images/ui/flowVelocityMeasurement/inputBoxOK.png);"
                                         "color: rgb(51,51,51); font: 18px \"思源黑体\"");

        checkBoxCHE = true;

        emit checkBoxIsChecked();
    }

    if(arg1 == Qt::Unchecked)//未选中
    {
        ui->time_lineEdit->setEnabled(false);
        ui->time_lineEdit->setStyleSheet("background-image: url(:/images/resources/images/ui/flowVelocityMeasurement/inputBoxNot.png);"
                                         "color: rgb(51,51,51); font: 18px \"思源黑体\"");

        checkBoxCHE = false;

        emit checkBoxIsUnchecked();
    }

}

void SHGCVelocimeterInteractiveWidget::on_confirmTheChange_pushButton_clicked()
{
    strSec = ui->time_lineEdit->text();

    if(checkBoxCHE == true && strSec != nullptr && WLG_connectOK == true)
    {
        emit intervalMeasurement_changeTime(strSec);
    }

    //    connect(qgcapplicationTipsWidget::Instance(this),SIGNAL(textStates(bool)),this,SLOT(TextInitializationOK(bool)));

    //    if(checkBoxCHE == true && tipsStates == true )//&& textOK == true
    //    {
    //        int w_applicationTipsWidget = this->width() / 2 - qgcapplicationTipsWidget::Instance(this)->width() / 2;
    //        int h_applicationTipsWidget = this->height() / 2 - qgcapplicationTipsWidget::Instance(this)->height() / 2;

    //        qgcapplicationTipsWidget::Instance(this)->move(w_applicationTipsWidget,h_applicationTipsWidget);
    //        qgcapplicationTipsWidget::Instance(this)->show();

    //        connect(qtimer, SIGNAL(timeout()), this, SLOT(close_qgcapplicationTipsWidget()));
    //        qtimer->start(3000);
    //    }
}

//void qgcVelocimeterInteractiveWidget::close_qgcapplicationTipsWidget()
//{
//    if(qgcapplicationTipsWidget::Instance(this) != nullptr)
//    {
//        qgcapplicationTipsWidget::Instance(this)->close();
//    }
//}

void SHGCVelocimeterInteractiveWidget::on_setUp_pushButton_clicked()
{
    emit setUp_pushButton_isClicked();
}

//将临时文件解析成正确的JSON格式并保存
void SHGCVelocimeterInteractiveWidget::handleFVJSONFile(bool b)
{
    if(b == true)
    {
        string file_path = "jLogs/flowVelocity_temp.json";//文件路径
        string out_path = "jLogs/flowVelocity.json";//文件路径
        string str = "";
        string::size_type pos = 0;
        ifstream instream;
        ofstream outstream;

        instream.open(file_path);
        if(!instream)
        {
            qDebug()<<"handle instream file open error"<<endl;
        }

        outstream.open(out_path);
        if(!outstream)
        {
            qDebug()<<"handle outstream file open error"<<endl;
        }
        outstream.clear();//写之前先清空文件

        while(getline(instream,str))
        {
            pos = str.find("]");//查找字符在string中第一次出现的位置
            if (pos ==string::npos)//判断是否存在“]”这个字符
            {
                //            cout<<"Can not find \"]\" string !"<<endl;
            }
            else
            {
                str.replace(pos,1,",");//用','替换']'
                outstream<<str<<endl;

                //            cout<<"find \"]\" ok !"<<endl;
                continue;
            }

            pos = str.find("[");//查找字符在string中第一次出现的位置
            if (pos ==string::npos)//判断是否存在“[”这个字符
            {
                //            cout<<"Can not find \"[\" string !"<<endl;
            }
            else
            {
                str.erase(pos); //删除"["
                outstream<<str<<endl;

                //            cout<<"find \"[\" ok !"<<endl;
                continue;
            }
            outstream<<str<<endl;
        }

        outstream.seekp(0,ios::beg);
        outstream<<"[";

        outstream.seekp(-3,ios::end);
        outstream<<"]"<<endl;

        instream.close();
        outstream.close();
    }
}

//读取并解析JSON数据
void SHGCVelocimeterInteractiveWidget::analysisFVJSONFile(bool b)
{

    if(b == true)
    {
        //打开JSON文件
        QFile file("jLogs/flowVelocity.json");
        if(!file.open(QIODevice::ReadOnly)) {
            qDebug() << "analysis File open error";
        } else {
            qDebug() <<"analysis File open!";
        }

        QJsonParseError jsonParserError;
        QJsonDocument   jsonDocument = QJsonDocument::fromJson( file.readAll(), &jsonParserError );

        qDebug()<<"jsonParserError:"<<jsonParserError.error;


        //        ui->tableWidget_FV->clearContents();//清空表格中的内容（不包含表头）
        int li_row = 0;

        if ( !jsonDocument.isNull() &&
             jsonParserError.error == QJsonParseError::NoError )
        {
            //        qDebug() << "文件解析成功\n";

            jsonArray_FV = jsonDocument.array();
            if ( jsonDocument.isArray() )
            {
                //            QJsonObject jsonObject = jsonDocument.object();

                for ( int i = 0; i < jsonArray_FV.size(); i++ )
                {
                    if ( jsonArray_FV[ i ].isObject() )
                    {
                        QJsonObject jsonObjectPost = jsonArray_FV[ i ].toObject();
                        if ( jsonObjectPost.contains( "01_id" ) &&
                             jsonObjectPost.contains( "02_time" ) &&
                             jsonObjectPost.contains( "03_date" ) &&
                             jsonObjectPost.contains( "04_org_flowvely" ) &&
                             jsonObjectPost.contains( "05_calcoe" ) &&
                             jsonObjectPost.contains( "06_cal_flowvely" ) &&
                             jsonObjectPost.contains( "07_var_lng" ) &&
                             jsonObjectPost.contains( "08_var_lat" ) &&
                             jsonObjectPost.contains( "09_ave_flowvely" ) &&
                             jsonObjectPost.contains( "10_SNR" ) &&
                             jsonObjectPost.contains( "11_WLG_Height" ) &&
                             jsonObjectPost.contains( "12_UAV_Height" ) &&
                             jsonObjectPost.value( "01_id" ).isDouble() &&
                             jsonObjectPost.value( "02_time" ).isString() &&
                             jsonObjectPost.value( "03_date" ).isString() &&
                             jsonObjectPost.value( "04_org_flowvely" ).isDouble() &&
                             jsonObjectPost.value( "05_calcoe" ).isDouble() &&
                             jsonObjectPost.value( "06_cal_flowvely" ).isDouble() &&
                             jsonObjectPost.value( "07_var_lng" ).isString() &&
                             jsonObjectPost.value( "08_var_lat" ).isString() &&
                             jsonObjectPost.value( "09_ave_flowvely" ).isDouble() &&
                             jsonObjectPost.value( "10_SNR" ).isDouble() &&
                             jsonObjectPost.value( "11_WLG_Height" ).isDouble() &&
                             jsonObjectPost.value( "12_UAV_Height" ).isString()
                             )
                        {
                            //                        qDebug() << "posts[" << i << "] :\n";
                            //                        qDebug() << "01_id is "<< jsonObjectPost.value( "01_id" ).toInt();
                            //                        qDebug() << "02_time is "<< jsonObjectPost.value( "02_time" ).toString();
                            //                        qDebug() << "03_date is "<< jsonObjectPost.value( "03_date" ).toString();
                            //                        qDebug() << "04_org_flowvely is "<< jsonObjectPost.value( "04_org_flowvely" ).toDouble();
                            //                        qDebug() << "05_calcoe is "<< jsonObjectPost.value( "05_calcoe" ).toDouble();
                            //                        qDebug() << "06_cal_flowvely is "<< jsonObjectPost.value( "06_cal_flowvely" ).toDouble();
                            //                        qDebug() << "07_var_lng is "<< jsonObjectPost.value( "07_var_lng" ).toString();
                            //                        qDebug() << "08_var_lat is "<< jsonObjectPost.value( "08_var_lat" ).toString();

                            id_FV = QString::number(jsonObjectPost.value( "01_id" ).toInt(),10,0);
                            time_FV = jsonObjectPost.value( "02_time" ).toString();
                            date_FV = jsonObjectPost.value( "03_date" ).toString();
                            org_flowvely_FV = QString::number(jsonObjectPost.value( "04_org_flowvely" ).toDouble(),10,2);
                            calcoe_FV = QString::number(jsonObjectPost.value( "05_calcoe" ).toDouble(),10,2);
                            cal_flowvely_FV = QString::number(jsonObjectPost.value( "06_cal_flowvely" ).toDouble(),10,2);
                            var_lng_FV = jsonObjectPost.value( "07_var_lng" ).toString();
                            var_lat_FV =jsonObjectPost.value( "08_var_lat" ).toString();
                            ave_flowvely_FV = QString::number(jsonObjectPost.value( "09_ave_flowvely" ).toDouble(),10,2);
                            SNR_FV = QString::number(jsonObjectPost.value( "10_SNR" ).toDouble(),10,2);
                            WLG_Height = QString::number(jsonObjectPost.value( "11_WLG_Height" ).toDouble(),10,3);
                            UAV_Height =jsonObjectPost.value( "12_UAV_Height" ).toString();

                            //初始化TableWidget_FV
                            initTableWidget_FV();

                            if(initTableWidgeStatus == true)
                            {
                                ui->tableWidget_FV->clearContents();//清空表格中的内容（不包含表头）
                                initTableWidgeStatus = false;
                                qDebug()<<"initTableWidgeStatus == true";
                            }

//                            qDebug()<<"d_FV_WLG_MaxDeviationValue:"<<d_FV_WLG_MaxDeviationValue;
                            if(org_flowvely_temp != 0.0)
                            {
                                if(org_flowvely_FV.toDouble() - org_flowvely_temp  >= d_FV_WLG_MaxDeviationValue ||
                                        org_flowvely_temp - org_flowvely_FV.toDouble() >= d_FV_WLG_MaxDeviationValue)
                                {
                                    //不执行任何操作
//                                    qDebug()<<"--------------return!"<< org_flowvely_temp <<" "<<org_flowvely_FV;
                                }
                                else
                                {
//                                    qDebug()<<"------List-------"<< org_flowvely_temp<<" "<<li_row <<" "<<org_flowvely_FV;

                                    //确保第一条数据不被过滤掉
                                    if(returnValueIs_firstStates == 0 || returnValueIs_firstStates == 1)
                                    {
                                        org_flowvely_temp = 0.00;
//                                        qDebug()<<"----------------firstStates 1:"<<returnValueIs_firstStates;
                                    }
                                    else
                                    {
//                                        qDebug()<<"----------------firstStates 2:"<<returnValueIs_firstStates;
                                        org_flowvely_temp = org_flowvely_FV.toDouble();//保存上一次的流速值
                                    }

                                    //设置文字居中对齐
                                    QTableWidgetItem *item0 = new QTableWidgetItem(QString::number(li_row));
//                                    QTableWidgetItem *item0 = new QTableWidgetItem(id_FV);
                                    item0->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
                                    QTableWidgetItem *item1 = new QTableWidgetItem(time_FV+ "\n" + date_FV);
                                    item1->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
                                    QTableWidgetItem *item2 = new QTableWidgetItem(org_flowvely_FV);
                                    item2->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
                                    QTableWidgetItem *item3 = new QTableWidgetItem(calcoe_FV);
                                    item3->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
                                    QTableWidgetItem *item4 = new QTableWidgetItem(cal_flowvely_FV);
                                    item4->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
                                    QTableWidgetItem *item5 = new QTableWidgetItem(var_lng_FV + "\n" + var_lat_FV);
                                    item5->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);

                                    //设置字体颜色
                                    item0->setTextColor(QColor(255,255,255));
                                    item1->setTextColor(QColor(255,255,255));
                                    item2->setTextColor(QColor(255,255,255));
                                    item3->setTextColor(QColor(255,255,255));
                                    item4->setTextColor(QColor(255,255,255));
                                    item5->setTextColor(QColor(255,255,255));

                                    ui->tableWidget_FV->setItem(li_row, 0, item0);
                                    ui->tableWidget_FV->setItem(li_row, 1, item1);
                                    ui->tableWidget_FV->setItem(li_row, 2, item2);
                                    ui->tableWidget_FV->setItem(li_row, 3, item3);
                                    ui->tableWidget_FV->setItem(li_row, 4, item4);
                                    ui->tableWidget_FV->setItem(li_row, 5, item5);

                                    li_row ++;
                                }
                            }
                            else
                            {
                                //确保第一条数据不被过滤掉
                                if(returnValueIs_firstStates == 0 || returnValueIs_firstStates == 1)
                                {
                                    org_flowvely_temp = 0.00;
//                                    qDebug()<<"----------------firstStates 3:"<<returnValueIs_firstStates;
                                }
                                else
                                {
//                                    qDebug()<<"----------------firstStates 4:"<<returnValueIs_firstStates;
                                    org_flowvely_temp = org_flowvely_FV.toDouble();//保存上一次的流速值
                                }

                                //设置文字居中对齐
                                QTableWidgetItem *item0 = new QTableWidgetItem(QString::number(li_row));
//                                QTableWidgetItem *item0 = new QTableWidgetItem(id_FV);
                                item0->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
                                QTableWidgetItem *item1 = new QTableWidgetItem(time_FV+ "\n" + date_FV);
                                item1->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
                                QTableWidgetItem *item2 = new QTableWidgetItem(org_flowvely_FV);
                                item2->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
                                QTableWidgetItem *item3 = new QTableWidgetItem(calcoe_FV);
                                item3->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
                                QTableWidgetItem *item4 = new QTableWidgetItem(cal_flowvely_FV);
                                item4->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
                                QTableWidgetItem *item5 = new QTableWidgetItem(var_lng_FV + "\n" + var_lat_FV);
                                item5->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);

                                //设置字体颜色
                                item0->setTextColor(QColor(255,255,255));
                                item1->setTextColor(QColor(255,255,255));
                                item2->setTextColor(QColor(255,255,255));
                                item3->setTextColor(QColor(255,255,255));
                                item4->setTextColor(QColor(255,255,255));
                                item5->setTextColor(QColor(255,255,255));

                                ui->tableWidget_FV->setItem(li_row, 0, item0);
                                ui->tableWidget_FV->setItem(li_row, 1, item1);
                                ui->tableWidget_FV->setItem(li_row, 2, item2);
                                ui->tableWidget_FV->setItem(li_row, 3, item3);
                                ui->tableWidget_FV->setItem(li_row, 4, item4);
                                ui->tableWidget_FV->setItem(li_row, 5, item5);

                                li_row ++;
                            }

                        }
                    }
                }
            }
        }
        else {
            qDebug()<<"文件解析失败！";
        }
        ui->tableWidget_FV->setRowCount(li_row);
        ui->tableWidget_FV->scrollToBottom();
        file.close();
    }

    //TCP客户端数据打包并发送
    if(_slotconnectedsuccess == true)
    {
        //        tcpClientDataPackingAndSending_FV();

        regularlySendStates = false;

        tcpClientDataPackingAndSending();
    }

    returnValueIs_firstStates ++;
}

void SHGCVelocimeterInteractiveWidget::setFVTheLatestValue_forTableWidget_realTime(bool b)
{  
    //初始化TableWidget_realTime
    initTableWidget_realTime();

    if(b == true)
    {
        //        qDebug()<<"-------start---"<< org_flowvely_temp <<" "<<org_flowvely_FV;

        if(org_flowvely_temp != 0.0)
        {
            if(org_flowvely_FV.toDouble() - org_flowvely_temp  >= d_FV_WLG_MaxDeviationValue ||
                    org_flowvely_temp - org_flowvely_FV.toDouble() >= d_FV_WLG_MaxDeviationValue
                    )
            {
                //不执行任何操作
//                qDebug()<<"--------------return!"<< org_flowvely_temp <<" "<<org_flowvely_FV;
            }
            else
            {
//                qDebug()<<"--------------else!"<< org_flowvely_temp <<" "<<org_flowvely_FV;
                ui->tableWidget_realTime->clearContents();//清空表格中的内容（不包含表头）

                //确保第一条数据不被过滤掉
                if(returnValueIs_firstStates == 0 || returnValueIs_firstStates == 1)
                {
                    org_flowvely_temp = 0.00;
//                    qDebug()<<"----------------firstStates 3:"<<returnValueIs_firstStates;
                }
                else
                {
//                    qDebug()<<"----------------firstStates 4:"<<returnValueIs_firstStates;
                    org_flowvely_temp = org_flowvely_FV.toDouble();//保存上一次的流速值
                }

                //设置文字居中对齐
                QTableWidgetItem *item0 = new QTableWidgetItem(time_FV+ "\n" + date_FV);
                item0->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
                QTableWidgetItem *item1 = new QTableWidgetItem(org_flowvely_FV);
                item1->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
                QTableWidgetItem *item2 = new QTableWidgetItem(calcoe_FV);
                item2->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
                QTableWidgetItem *item3 = new QTableWidgetItem(cal_flowvely_FV);
                item3->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
                QTableWidgetItem *item4 = new QTableWidgetItem(var_lng_FV + "\n" + var_lat_FV);
                item4->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);

                //设置字体颜色
                item0->setTextColor(QColor(255,255,255));
                item1->setTextColor(QColor(255,255,255));
                item2->setTextColor(QColor(255,255,255));
                item3->setTextColor(QColor(255,255,255));
                item4->setTextColor(QColor(255,255,255));

                ui->tableWidget_realTime->setItem(0, 0, item0);
                ui->tableWidget_realTime->setItem(0, 1, item1);
                ui->tableWidget_realTime->setItem(0, 2, item2);
                ui->tableWidget_realTime->setItem(0, 3, item3);
                ui->tableWidget_realTime->setItem(0, 4, item4);
            }
        }
        else
        {
            //确保第一条数据不被过滤掉
            if(returnValueIs_firstStates == 0 || returnValueIs_firstStates == 1)
            {
                org_flowvely_temp = 0.00;
//                qDebug()<<"----------------firstStates 3:"<<returnValueIs_firstStates;
            }
            else
            {
//                qDebug()<<"----------------firstStates 4:"<<returnValueIs_firstStates;
                org_flowvely_temp = org_flowvely_FV.toDouble();//保存上一次的流速值
            }

            //设置文字居中对齐
            QTableWidgetItem *item0 = new QTableWidgetItem(time_FV+ "\n" + date_FV);
            item0->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
            QTableWidgetItem *item1 = new QTableWidgetItem(org_flowvely_FV);
            item1->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
            QTableWidgetItem *item2 = new QTableWidgetItem(calcoe_FV);
            item2->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
            QTableWidgetItem *item3 = new QTableWidgetItem(cal_flowvely_FV);
            item3->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
            QTableWidgetItem *item4 = new QTableWidgetItem(var_lng_FV + "\n" + var_lat_FV);
            item4->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);

            //设置字体颜色
            item0->setTextColor(QColor(255,255,255));
            item1->setTextColor(QColor(255,255,255));
            item2->setTextColor(QColor(255,255,255));
            item3->setTextColor(QColor(255,255,255));
            item4->setTextColor(QColor(255,255,255));

            ui->tableWidget_realTime->setItem(0, 0, item0);
            ui->tableWidget_realTime->setItem(0, 1, item1);
            ui->tableWidget_realTime->setItem(0, 2, item2);
            ui->tableWidget_realTime->setItem(0, 3, item3);
            ui->tableWidget_realTime->setItem(0, 4, item4);
        }
    }
    else
    {
        //若接收到的数据不完整 下次写数据之前先清空
        if( ui->tableWidget_realTime->item(0,0) != nullptr ||
                ui->tableWidget_realTime->item(0,1) != nullptr ||
                ui->tableWidget_realTime->item(0,2) != nullptr ||
                ui->tableWidget_realTime->item(0,3) != nullptr ||
                ui->tableWidget_realTime->item(0,4) != nullptr)
        {
            ui->tableWidget_realTime->clearContents();//清空表格中的内容（不包含表头）
        }
    }
}

void SHGCVelocimeterInteractiveWidget::setDisplayHistoricalData(QString filePath)
{
    initTableWidgeStatus = true;

    //打开JSON文件
    QFile file(filePath);
    if(!file.open(QIODevice::ReadOnly)) {
        qDebug() << "analysis File open error";
    } else {
        qDebug() <<"analysis File open!";
    }

    QJsonParseError jsonParserError;
    QJsonDocument   jsonDocument = QJsonDocument::fromJson( file.readAll(), &jsonParserError );

    qDebug()<<"jsonParserError:"<<jsonParserError.error;

    int li_row = 0;

    if ( !jsonDocument.isNull() &&
         jsonParserError.error == QJsonParseError::NoError )
    {
        //        qDebug() << "文件解析成功\n";

        jsonArray_FV = jsonDocument.array();
        if ( jsonDocument.isArray() )
        {
            //            QJsonObject jsonObject = jsonDocument.object();

            for ( int i = 0; i < jsonArray_FV.size(); i++ )
            {
                if ( jsonArray_FV[ i ].isObject() )
                {
                    QJsonObject jsonObjectPost = jsonArray_FV[ i ].toObject();
                    if ( jsonObjectPost.contains( "01_id" ) &&
                         jsonObjectPost.contains( "02_time" ) &&
                         jsonObjectPost.contains( "03_date" ) &&
                         jsonObjectPost.contains( "04_org_flowvely" ) &&
                         jsonObjectPost.contains( "05_calcoe" ) &&
                         jsonObjectPost.contains( "06_cal_flowvely" ) &&
                         jsonObjectPost.contains( "07_var_lng" ) &&
                         jsonObjectPost.contains( "08_var_lat" ) &&
                         jsonObjectPost.contains( "09_ave_flowvely" ) &&
                         jsonObjectPost.contains( "10_SNR" ) &&
                         jsonObjectPost.contains( "11_WLG_Height" ) &&
                         jsonObjectPost.contains( "12_UAV_Height" ) &&
                         jsonObjectPost.value( "01_id" ).isDouble() &&
                         jsonObjectPost.value( "02_time" ).isString() &&
                         jsonObjectPost.value( "03_date" ).isString() &&
                         jsonObjectPost.value( "04_org_flowvely" ).isDouble() &&
                         jsonObjectPost.value( "05_calcoe" ).isDouble() &&
                         jsonObjectPost.value( "06_cal_flowvely" ).isDouble() &&
                         jsonObjectPost.value( "07_var_lng" ).isString() &&
                         jsonObjectPost.value( "08_var_lat" ).isString() &&
                         jsonObjectPost.value( "09_ave_flowvely" ).isDouble() &&
                         jsonObjectPost.value( "10_SNR" ).isDouble() &&
                         jsonObjectPost.value( "11_WLG_Height" ).isDouble() &&
                         jsonObjectPost.value( "12_UAV_Height" ).isString()
                         )
                    {
                        id_FV_History = QString::number(jsonObjectPost.value( "01_id" ).toInt(),10,0);
                        time_FV_History = jsonObjectPost.value( "02_time" ).toString();
                        date_FV_History = jsonObjectPost.value( "03_date" ).toString();
                        org_flowvely_FV_History = QString::number(jsonObjectPost.value( "04_org_flowvely" ).toDouble(),10,2);
                        calcoe_FV_History = QString::number(jsonObjectPost.value( "05_calcoe" ).toDouble(),10,2);
                        cal_flowvely_FV_History = QString::number(jsonObjectPost.value( "06_cal_flowvely" ).toDouble(),10,2);
                        var_lng_FV_History = jsonObjectPost.value( "07_var_lng" ).toString();
                        var_lat_FV_History =jsonObjectPost.value( "08_var_lat" ).toString();
                        ave_flowvely_FV_History = QString::number(jsonObjectPost.value( "09_ave_flowvely" ).toDouble(),10,2);
                        SNR_FV_History = QString::number(jsonObjectPost.value( "10_SNR" ).toDouble(),10,2);
                        WLG_Height_History = QString::number(jsonObjectPost.value( "11_WLG_Height" ).toDouble(),10,3);
                        UAV_Height_History =jsonObjectPost.value( "12_UAV_Height" ).toString();

                        //初始化TableWidget_FV
                        initTableWidget_FV();

                        if(initTableWidgeStatus == true)
                        {
                            ui->tableWidget_FV->clearContents();//清空表格中的内容（不包含表头）
                            initTableWidgeStatus = false;
                        }

                        //设置文字居中对齐
                        QTableWidgetItem *item0 = new QTableWidgetItem(id_FV_History);
                        item0->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
                        QTableWidgetItem *item1 = new QTableWidgetItem(time_FV_History+ "\n" + date_FV_History);
                        item1->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
                        QTableWidgetItem *item2 = new QTableWidgetItem(org_flowvely_FV_History);
                        item2->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
                        QTableWidgetItem *item3 = new QTableWidgetItem(calcoe_FV_History);
                        item3->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
                        QTableWidgetItem *item4 = new QTableWidgetItem(cal_flowvely_FV_History);
                        item4->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
                        QTableWidgetItem *item5 = new QTableWidgetItem(var_lng_FV_History + "\n" + var_lat_FV_History);
                        item5->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);

                        //设置字体颜色
                        item0->setTextColor(QColor(255,255,255));
                        item1->setTextColor(QColor(255,255,255));
                        item2->setTextColor(QColor(255,255,255));
                        item3->setTextColor(QColor(255,255,255));
                        item4->setTextColor(QColor(255,255,255));
                        item5->setTextColor(QColor(255,255,255));

                        //                        ui->tableWidget_FV->setItem(jsonArray_FV.size() -i - 1, 0, item0);
                        ui->tableWidget_FV->setItem(li_row, 0, item0);
                        ui->tableWidget_FV->setItem(li_row, 1, item1);
                        ui->tableWidget_FV->setItem(li_row, 2, item2);
                        ui->tableWidget_FV->setItem(li_row, 3, item3);
                        ui->tableWidget_FV->setItem(li_row, 4, item4);
                        ui->tableWidget_FV->setItem(li_row, 5, item5);

                        li_row ++;
                    }
                }
            }
        }
        historicalData_JsonArray_FV_Size = jsonArray_FV.size();
    }
    else {
        qDebug()<<"文件解析失败！";
    }
//    qDebug()<<"------------WLG_Height-00:"<<WLG_Height_History;
    //    initLable_WLG();//初始化Lable_WLG
    //    ui->label_WLG->setText(WLG_Height_History + "m");//设置飞机距水面高度 初始值

    ui->tableWidget_FV->setRowCount(li_row);
    ui->tableWidget_FV->scrollToBottom();

    file.close();

//    qDebug()<<"------------WLG_Height:"<<WLG_Height_History;

}

//文件数据超过10天 则清空
void SHGCVelocimeterInteractiveWidget::clearFVJSONFile(bool b)
{
    if(b == true)
    {
        QJsonObject jsonObjectPost_first = jsonArray_FV[ 0 ].toObject();
        QString date_first= jsonObjectPost_first.value( "03_date" ).toString();
        QString date_first_strDay = date_first.mid(8,2);//8
        int date_first_intDay = date_first_strDay.toInt();


        QJsonObject jsonObjectPost_last = jsonArray_FV[ jsonArray_FV.size() - 1 ].toObject();
        QString date_last = jsonObjectPost_last.value( "03_date" ).toString();
        QString date_last_strDay = date_last.mid(8,2);
        int date_last_intDay = date_last_strDay.toInt();

        if(((date_last_intDay >= date_first_intDay) &&(date_last_intDay - date_first_intDay) > 10) ||
                ((date_last_intDay < date_first_intDay) && ((31-date_first_intDay) + date_last_intDay) > 10))
        {
            QFile file_temp("jLogs/flowVelocity_temp.json");
            if(file_temp.open(QIODevice::WriteOnly))
            {
                file_temp.flush();
                qDebug()<<"file_temp clear";
            }
            file_temp.close();

            QFile file("jLogs/flowVelocity.json");
            if(file_temp.open(QIODevice::WriteOnly))
            {
                file.flush();
                qDebug()<<"file clear";
            }
            file.close();

        }
    }
}

/*************************水位计*****************************/

//更改水位计测量按钮状态 不可点击
void SHGCVelocimeterInteractiveWidget::setWLG_pushButtonUnableToClick()
{
    if(WLG_connectOK == true)
    {
        ui->connectWLG_pushButton->setEnabled(false);
        ui->connectWLG_pushButton->setStyleSheet("border-image: url(:/images/resources/images/ui/flowVelocityMeasurement/CantClick_WLG.png);");
    }
}

//更改水位计测量按钮状态 可点击
void SHGCVelocimeterInteractiveWidget::setWLG_pushButtonClickable()
{
    ui->connectWLG_pushButton->setEnabled(true);
    ui->connectWLG_pushButton->setStyleSheet("QPushButton{border-image: url(:/images/resources/images/ui/flowVelocityMeasurement/Con_waterLevelGauge.png);}"
                                             "QPushButton:hover{border-image: url(:/images/resources/images/ui/flowVelocityMeasurement/Con_waterLevelGauge.png);}"
                                             "QPushButton:pressed{border-image: url(:/images/resources/images/ui/flowVelocityMeasurement/Con_waterLevelGauge_2.png);}");
}

void SHGCVelocimeterInteractiveWidget::initLable_WLG()
{
    ui->label_WLG->setFont(QFont("Source Han Sans CN",16,87));//设置字体
    ui->label_WLG->setStyleSheet("color:rgb(255,255,255);");

    if( ui->label_WLG->text() != nullptr)
    {
        ui->label_WLG->clear();//清空lable中的数据
    }
}

void SHGCVelocimeterInteractiveWidget::on_connectWLG_pushButton_clicked()
{
    if(WLG_connectOK == false)
    {
        emit connectWLG_pushButton_isClicked_conn();
    }
    else
    {
        emit connectWLG_pushButton_isClicked_send();
    }
}

void SHGCVelocimeterInteractiveWidget::changeWLGBackground_red()
{
    ui->connectWLG_pushButton->setStyleSheet("border-image: url(:/images/resources/images/ui/flowVelocityMeasurement/Unc_waterLevelGauge.png);");
    WLG_connectOK = false;

}

void SHGCVelocimeterInteractiveWidget::changeWLGBackground_blue()
{
    //    ui->connectWLG_pushButton->setStyleSheet("border-image: url(:/images/resources/images/ui/flowVelocityMeasurement/Con_waterLevelGauge.png);");

    ui->connectWLG_pushButton->setStyleSheet("QPushButton{border-image: url(:/images/resources/images/ui/flowVelocityMeasurement/Con_waterLevelGauge.png);}"
                                             "QPushButton:hover{border-image: url(:/images/resources/images/ui/flowVelocityMeasurement/Con_waterLevelGauge.png);}"
                                             "QPushButton:pressed{border-image: url(:/images/resources/images/ui/flowVelocityMeasurement/Con_waterLevelGauge_2.png);}");
    WLG_connectOK = true;
}

void SHGCVelocimeterInteractiveWidget::handleWLGJSONFile(bool b)
{
    if(b == true)
    {
        string file_path = "jLogs/waterLevelGauge_temp.json";//文件路径
        string out_path = "jLogs/waterLevelGauge.json";//文件路径
        string str = "";
        string::size_type pos = 0;
        ifstream instream;
        ofstream outstream;

        instream.open(file_path);
        if(!instream)
        {
            qDebug()<<"handle instream file open error"<<endl;
        }

        outstream.open(out_path);
        if(!outstream)
        {
            qDebug()<<"handle outstream file open error"<<endl;
        }
        outstream.clear();//写之前先清空文件

        while(getline(instream,str))
        {
            pos = str.find("]");//查找字符在string中第一次出现的位置
            if (pos ==string::npos)//判断是否存在“]”这个字符
            {
                //            cout<<"Can not find \"]\" string !"<<endl;
            }
            else
            {
                str.replace(pos,1,",");//用','替换']'
                outstream<<str<<endl;

                //            cout<<"find \"]\" ok !"<<endl;
                continue;
            }

            pos = str.find("[");//查找字符在string中第一次出现的位置
            if (pos ==string::npos)//判断是否存在“[”这个字符
            {
                //            cout<<"Can not find \"[\" string !"<<endl;
            }
            else
            {
                str.erase(pos); //删除"["
                outstream<<str<<endl;

                //            cout<<"find \"[\" ok !"<<endl;
                continue;
            }
            outstream<<str<<endl;
        }

        outstream.seekp(0,ios::beg);
        outstream<<"[";

        outstream.seekp(-3,ios::end);
        outstream<<"]"<<endl;

        instream.close();
        outstream.close();
    }
}

void SHGCVelocimeterInteractiveWidget::analysisWLGJSONFile(bool b)
{
    if(b == true)
    {
        //打开JSON文件
        QFile file("jLogs/waterLevelGauge.json");
        if(!file.open(QIODevice::ReadOnly)) {
            qDebug() << "analysis File open error";
        } else {
            qDebug() <<"analysis File open!";
        }

        QJsonParseError jsonParserError;
        QJsonDocument   jsonDocument = QJsonDocument::fromJson( file.readAll(), &jsonParserError );

        qDebug()<<"jsonParserError:"<<jsonParserError.error;

        if ( !jsonDocument.isNull() &&
             jsonParserError.error == QJsonParseError::NoError )
        {
            //                    qDebug() << "文件解析成功\n";

            jsonArray_WLG = jsonDocument.array();
            if ( jsonDocument.isArray() )
            {
                //            QJsonObject jsonObject = jsonDocument.object();

                for ( int i = 0; i < jsonArray_WLG.size(); i++ )
                {
                    if ( jsonArray_WLG[ i ].isObject() )
                    {
                        QJsonObject jsonObjectPost = jsonArray_WLG[ i ].toObject();
                        if ( jsonObjectPost.contains( "01_id" ) &&
                             jsonObjectPost.contains( "02_time" ) &&
                             jsonObjectPost.contains( "03_date" ) &&
                             jsonObjectPost.contains( "04_waterLevel" ) &&
                             jsonObjectPost.contains( "05_var_lng" ) &&
                             jsonObjectPost.contains( "06_var_lat" ) &&
                             jsonObjectPost.value( "01_id" ).isDouble() &&
                             jsonObjectPost.value( "02_time" ).isString() &&
                             jsonObjectPost.value( "03_date" ).isString() &&
                             jsonObjectPost.value( "04_waterLevel" ).isDouble() &&
                             jsonObjectPost.value( "05_var_lng" ).isString() &&
                             jsonObjectPost.value( "06_var_lat" ).isString() &&
                             jsonObjectPost.value( "07_var_alt" ).isString()
                             )
                        {
                            id_WLG = QString::number(jsonObjectPost.value( "01_id" ).toInt(),10,0);
                            time_WLG = jsonObjectPost.value( "02_time" ).toString();
                            date_WLG = jsonObjectPost.value( "03_date" ).toString();
                            waterLevel_WLG = QString::number(jsonObjectPost.value( "04_waterLevel" ).toDouble(),10,3);
                            var_lng_WLG = jsonObjectPost.value( "05_var_lng" ).toString();
                            var_lat_WLG = jsonObjectPost.value( "06_var_lat" ).toString();
                            var_alt_WLG = jsonObjectPost.value( "07_var_alt" ).toString();
                        }
                    }
                }
            }
        }
        else {
            qDebug()<<"文件解析失败！";
        }

        file.close();
    }

    //    //TCP客户端数据打包并发送
    //    if(_slotconnectedsuccess == true)
    //    {
    //        tcpClientDataPackingAndSending_WLG();
    //    }
}

//文件数据超过10天 则清空
void SHGCVelocimeterInteractiveWidget::clearWLGJSONFile(bool b)
{
    if(b == true)
    {
        QJsonObject jsonObjectPost_first = jsonArray_WLG[ 0 ].toObject();
        QString date_first= jsonObjectPost_first.value( "03_date" ).toString();
        QString date_first_strDay = date_first.mid(8,2);//8
        int date_first_intDay = date_first_strDay.toInt();


        QJsonObject jsonObjectPost_last = jsonArray_WLG[ jsonArray_WLG.size() - 1 ].toObject();
        QString date_last = jsonObjectPost_last.value( "03_date" ).toString();
        QString date_last_strDay = date_last.mid(8,2);
        int date_last_intDay = date_last_strDay.toInt();

        if(((date_last_intDay >= date_first_intDay) &&(date_last_intDay - date_first_intDay) > 10) ||
                ((date_last_intDay < date_first_intDay) && ((31-date_first_intDay) + date_last_intDay) > 10))
        {
            QFile file_temp("jLogs/waterLevelGauge_temp.json");
            if(file_temp.open(QIODevice::WriteOnly))
            {
                file_temp.flush();
                qDebug()<<"file_temp clear";
            }
            file_temp.close();

            QFile file("jLogs/waterLevelGauge.json");
            if(file_temp.open(QIODevice::WriteOnly))
            {
                file.flush();
                qDebug()<<"file clear";
            }
            file.close();

        }
    }
}

void SHGCVelocimeterInteractiveWidget::setWLGTheLatestValue_forLable_realTime(bool b)
{
    //初始化Lable_WLG
    initLable_WLG();

    if(b == true)
    {
        ui->label_WLG->setText(waterLevel_WLG + "m");
    }
    else
    {
        //若接收到的数据不完整 则下次写入之前先清空
        if( ui->label_WLG->text() != nullptr)
        {
            ui->label_WLG->clear();//清空lable中的数据
        }
    }
}

//初始化定时器
void SHGCVelocimeterInteractiveWidget::initqTimer()
{
    this->qtimer = new QTimer(this);
}

void SHGCVelocimeterInteractiveWidget::changeIp(QString ip)
{
    this->g_tcpIP = ip;
    qDebug()<<"啊哈哈哈哈 g_tcpIP:"<<g_tcpIP;
}

void SHGCVelocimeterInteractiveWidget::changePort(QString port)
{
    this->g_tcpPort = port;
    qDebug()<<"呦呵呵呵呵 g_tcpPort:"<<g_tcpPort;
}

void SHGCVelocimeterInteractiveWidget::tcpClientConnecting()
{
    if(/*!tcpsocket->isOpen() &&*/ _slotconnectedsuccess != true)
    {
        //和服务器进行连接
        tcpsocket->connectToHost(g_tcpIP,g_tcpPort.toInt(),QTcpSocket::ReadWrite);
        //    tcpsocket->connectToHost("192.168.1.94",60000,QTcpSocket::ReadWrite);
        //    tcpsocket->connectToHost("127.0.0.1",9527,QTcpSocket::ReadWrite);
        //和服务器连接成功能会触发connected信号
        disconnect(tcpsocket, &QTcpSocket::connected, this, &SHGCVelocimeterInteractiveWidget::slotconnectedsuccess);
        connect(tcpsocket, &QTcpSocket::connected, this, &SHGCVelocimeterInteractiveWidget::slotconnectedsuccess);

        qDebug()<<"创建和服务器的连接！";
    }
}

//用来处理连接成功的信号
void SHGCVelocimeterInteractiveWidget::slotconnectedsuccess()
{
    qDebug()<<"和服务器连接成功！";
    _slotconnectedsuccess = true;

    emit sendSignal_setAircraftCenter(true);
}


void SHGCVelocimeterInteractiveWidget::tcpClientDisConnecting()
{
    if(tcpsocket->isOpen())
    {
        tcpsocket->disconnectFromHost();
        tcpsocket->close();
        delete tcpsocket;
        tcpsocket = nullptr;
    }
}

void SHGCVelocimeterInteractiveWidget::tcpClientDataPackingAndSending()
{
    QString strSTX = "STX";
    QString strETX = "ETX";


    if(regularlySendStates == true)
    {
        sendData = strSTX + "|" + "S+H" + "|" + device_Num + "|" + time_FV_regularly  + "|" + date_FV_regularly  + "|" + org_flowvely_FV_regularly  + "|" + calcoe_FV_regularly  + "|" + cal_flowvely_FV_regularly  + "|" + ave_flowvely_FV_regularly  + "|" + var_lng_FV_regularly  + "|" + var_lat_FV_regularly  + "|" + WLG_Height_regularly  + "|" + UAV_Height_regularly  + "|" + "12" + "|" + strETX + "\n";
    }
    else
    {
        sendData = strSTX + "|" + "S+H" + "|" + device_Num + "|" + time_FV + "|" + date_FV + "|" + org_flowvely_FV + "|" + calcoe_FV + "|" + cal_flowvely_FV + "|" + ave_flowvely_FV + "|" + var_lng_FV + "|" + var_lat_FV + "|" + WLG_Height + "|" + UAV_Height + "|" + "12" + "|" + strETX + "\n";
    }

    tcpsocket->write(sendData.toUtf8().data());
    qDebug()<<"向服务器写 S+H 数据完成！:"<<sendData.toUtf8().data();
}

//void SHGCVelocimeterInteractiveWidget::tcpClientDataPackingAndSending_FV()
//{
//    QString strSTX = "STX";
//    QString strETX = "ETX";

//    sendData = strSTX + "|" + "S" + "|" + device_Num + "|" + time_FV + "|" + date_FV + "|" + org_flowvely_FV + "|" + calcoe_FV + "|" + cal_flowvely_FV + "|" + var_lng_FV + "|" + var_lat_FV + "|" + ave_flowvely_FV + "|" + "09" + "|" + strETX + "\n";
//    tcpsocket->write(sendData.toUtf8().data());
//    qDebug()<<"向服务器写 S 数据完成！:"<<sendData.toUtf8().data();
//}

//void SHGCVelocimeterInteractiveWidget::tcpClientDataPackingAndSending_WLG()
//{
//    QString strSTX = "STX";
//    QString strETX = "ETX";

//    sendData = strSTX + "|" + "H" + "|" + device_Num + "|" + time_WLG + "|" + date_WLG +"|" + waterLevel_WLG + "|" + var_lng_WLG + "|" + var_lat_WLG + "|" + var_alt_WLG + "|" + "07" + "|" + strETX + "\n";
//    tcpsocket->write(sendData.toUtf8().data());
//    qDebug()<<"向服务器写 H 数据完成！:"<<sendData.toUtf8().data();
//}

//获取无人机设备编号
//void SHGCVelocimeterInteractiveWidget::getDeviceNum()
//{
//    QString path = "ini/cal_DeviceID.ini";
//    QSettings settings(path, QSettings::IniFormat);
//    QString device_ID = "001";

//    int number = settings.value("DevOption/number").toInt();
//    for(int i = 1; i <= number; i++)
//    {
//        QString deviceID = settings.value(QString("DevOption/device_ID_%1").arg(i)).toString();
//        if(device_ID == deviceID)//当前设备ID 在配置文件里面
//        {
//            device_Num = QString::number(i);
//        }
//    }
//}

void SHGCVelocimeterInteractiveWidget::getDeviceNum(QString UAV_Number)
{
    device_Num = UAV_Number;
}

void SHGCVelocimeterInteractiveWidget::setFV_WLG_DebugMode(bool mode)
{
    if(mode == true)
    {
        ui->importFile->show();
        ui->sendData->show();
    }
    else if(mode == false)
    {
        ui->importFile->hide();
        ui->sendData->hide();
    }
}

//调试模式下 导入文件数据
void SHGCVelocimeterInteractiveWidget::on_importFile_clicked()
{
    //获取要打开文件的路径
    fileName = QFileDialog::getOpenFileName(this,
                                            tr("导入文件"),
                                            ".",
                                            tr("json日志文件(*json);"));

    if(fileName != nullptr)
    {
        setDisplayHistoricalData(fileName);//打开文件并展示数据
//        setFVTheLatestValue_forTableWidget_realTime(true);//展示数据到 QTableWidget_realTime
    }
}

//调试模式下 定时发送数据给服务器
void SHGCVelocimeterInteractiveWidget::on_sendData_clicked()
{
    if(SendDataInProgress != true)//不在发送数据
    {
        //打开JSON文件
        if(fileName == nullptr)
        {
            fileName = "jLogs/flowVelocity.json";
        }
        QFile file(fileName);
        if(!file.open(QIODevice::ReadOnly)) {
            qDebug() << "analysis File open error";
        } else {
            qDebug() <<"analysis File open!";
        }

        QJsonParseError jsonParserError;
        QJsonDocument   jsonDocument = QJsonDocument::fromJson( file.readAll(), &jsonParserError );

        qDebug()<<"jsonParserError:"<<jsonParserError.error;

        if ( !jsonDocument.isNull() &&
             jsonParserError.error == QJsonParseError::NoError )
        {
            //        qDebug() << "文件解析成功\n";

            jsonArray_FV = jsonDocument.array();
            if ( jsonDocument.isArray() )
            {
                //            QJsonObject jsonObject = jsonDocument.object();

                if(_slotconnectedsuccess == true)
                {
                    disconnect(qtimer, SIGNAL(timeout()), this, SLOT(regularlySendData()));
                    connect(qtimer, SIGNAL(timeout()), this, SLOT(regularlySendData()));
                    qtimer->start(20000);
                }
                else
                {
                    qDebug()<<"与服务器连接失败，请检查服务器配置！";
                }
            }
        }
        else {
            qDebug()<<"文件解析失败！";
        }

        file.close();
        size = 0;
    }
    else
    {
        qDebug()<<"正在发送数据中，请稍后再试！";
    }
}

void SHGCVelocimeterInteractiveWidget::regularlySendData()
{
    if(size > jsonArray_FV.size() - 1)
    {
        qtimer->stop();
        SendDataInProgress = false;
        return;
    }
    qDebug()<<"jsonArray_FV.size()"<<jsonArray_FV.size();
    if ( jsonArray_FV[ size ].isObject() )
    {
        QJsonObject jsonObjectPost = jsonArray_FV[ size ].toObject();
        if ( jsonObjectPost.contains( "01_id" ) &&
             jsonObjectPost.contains( "02_time" ) &&
             jsonObjectPost.contains( "03_date" ) &&
             jsonObjectPost.contains( "04_org_flowvely" ) &&
             jsonObjectPost.contains( "05_calcoe" ) &&
             jsonObjectPost.contains( "06_cal_flowvely" ) &&
             jsonObjectPost.contains( "07_var_lng" ) &&
             jsonObjectPost.contains( "08_var_lat" ) &&
             jsonObjectPost.contains( "09_ave_flowvely" ) &&
             jsonObjectPost.contains( "10_SNR" ) &&
             jsonObjectPost.contains( "11_WLG_Height" ) &&
             jsonObjectPost.contains( "12_UAV_Height" ) &&
             jsonObjectPost.value( "01_id" ).isDouble() &&
             jsonObjectPost.value( "02_time" ).isString() &&
             jsonObjectPost.value( "03_date" ).isString() &&
             jsonObjectPost.value( "04_org_flowvely" ).isDouble() &&
             jsonObjectPost.value( "05_calcoe" ).isDouble() &&
             jsonObjectPost.value( "06_cal_flowvely" ).isDouble() &&
             jsonObjectPost.value( "07_var_lng" ).isString() &&
             jsonObjectPost.value( "08_var_lat" ).isString() &&
             jsonObjectPost.value( "09_ave_flowvely" ).isDouble() &&
             jsonObjectPost.value( "10_SNR" ).isDouble() &&
             jsonObjectPost.value( "11_WLG_Height" ).isDouble() &&
             jsonObjectPost.value( "12_UAV_Height" ).isString()
             )
        {
            id_FV_regularly = QString::number(jsonObjectPost.value( "01_id" ).toInt(),10,0);
            time_FV_regularly = jsonObjectPost.value( "02_time" ).toString();
            date_FV_regularly = jsonObjectPost.value( "03_date" ).toString();
            org_flowvely_FV_regularly = QString::number(jsonObjectPost.value( "04_org_flowvely" ).toDouble(),10,2);
            calcoe_FV_regularly = QString::number(jsonObjectPost.value( "05_calcoe" ).toDouble(),10,2);
            cal_flowvely_FV_regularly = QString::number(jsonObjectPost.value( "06_cal_flowvely" ).toDouble(),10,2);
            var_lng_FV_regularly = jsonObjectPost.value( "07_var_lng" ).toString();
            var_lat_FV_regularly = jsonObjectPost.value( "08_var_lat" ).toString();
            ave_flowvely_FV_regularly = QString::number(jsonObjectPost.value( "09_ave_flowvely" ).toDouble(),10,2);
            SNR_FV_regularly = QString::number(jsonObjectPost.value( "10_SNR" ).toDouble(),10,2);
            WLG_Height_regularly = QString::number(jsonObjectPost.value( "11_WLG_Height" ).toDouble(),10,3);
            UAV_Height_regularly = jsonObjectPost.value( "12_UAV_Height" ).toString();

            regularlySendStates = true;

            tcpClientDataPackingAndSending();//每10s发送一条数据给服务器
            SendDataInProgress = true;//正在发送数据中

            //设置文字居中对齐
            QTableWidgetItem *item0 = new QTableWidgetItem(time_FV_regularly+ "\n" + date_FV_regularly);
            item0->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
            QTableWidgetItem *item1 = new QTableWidgetItem(org_flowvely_FV_regularly);
            item1->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
            QTableWidgetItem *item2 = new QTableWidgetItem(calcoe_FV_regularly);
            item2->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
            QTableWidgetItem *item3 = new QTableWidgetItem(cal_flowvely_FV_regularly);
            item3->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
            QTableWidgetItem *item4 = new QTableWidgetItem(var_lng_FV_regularly + "\n" + var_lat_FV_regularly);
            item4->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);

            //设置字体颜色
            item0->setTextColor(QColor(255,255,255));
            item1->setTextColor(QColor(255,255,255));
            item2->setTextColor(QColor(255,255,255));
            item3->setTextColor(QColor(255,255,255));
            item4->setTextColor(QColor(255,255,255));

            ui->tableWidget_realTime->setItem(0, 0, item0);
            ui->tableWidget_realTime->setItem(0, 1, item1);
            ui->tableWidget_realTime->setItem(0, 2, item2);
            ui->tableWidget_realTime->setItem(0, 3, item3);
            ui->tableWidget_realTime->setItem(0, 4, item4);
        }
    }
    size++;
    qDebug()<<"size:"<<size;
}

void SHGCVelocimeterInteractiveWidget::loadSettingParameters()
{
    QString path ="ini/SettingParameters.ini";
    QSettings settings(path, QSettings::IniFormat);

    d_FV_WLG_MaxDeviationValue = settings.value("VM_WLGOption/FV_WLG_MaxDeviationValue").toDouble();


}
