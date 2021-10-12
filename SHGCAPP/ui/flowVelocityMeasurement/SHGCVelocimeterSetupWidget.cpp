#include "SHGCVelocimeterSetupWidget.h"
#include "ui_SHGCVelocimeterSetupWidget.h"

#include "SHGCFvmApplicationTipsWidget.h"

#include <QRegExpValidator>
#include <QFrame>
#include <QTimer>
#include <QDebug>
#include <QSettings>
#include <QDir>

//#include "mainleftwindow.h"//临时方案

static SHGCVelocimeterSetUpWidget* _instance = nullptr;

SHGCVelocimeterSetUpWidget* SHGCVelocimeterSetUpWidget::_create(QWidget *parent)
{
    _instance = new SHGCVelocimeterSetUpWidget(parent);
    return _instance;
}

SHGCVelocimeterSetUpWidget* SHGCVelocimeterSetUpWidget::instance(void)
{
    return _instance;
}

void SHGCVelocimeterSetUpWidget::deleteInstance(void)
{
    delete this;
}

SHGCVelocimeterSetUpWidget::SHGCVelocimeterSetUpWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SHGCVelocimeterSetUpWidget)
{
    ui->setupUi(this);

    initqTimer();

    m_autoResizeHandler=new AutoResize(this,this->rect().width(),this->rect().height());
    m_autoResizeHandler->setAutoResizeFlag(
            AutoResize::INCLUDE_VELOCIMETERSETUP|
            AutoResize::INCLUDE_WIDGET|
            AutoResize::INCLUDE_PUSHBUTTON|
            AutoResize::INCLUDE_LABEL|
            AutoResize::INCLUDE_LINEEDIT|
            AutoResize::INCLUDE_TABLEWIDGET|
            AutoResize::INCLUDE_SCROLLAREA
            );
    m_autoResizeHandler->pushAllResizeItem();

    //实例化 提示框 类
    SHGCFvmApplicationTipsWidget *_SHGCFvmApplicationTipsWidget = SHGCFvmApplicationTipsWidget::_create(this);
    _SHGCFvmApplicationTipsWidget->close();

    QFrame *frame1 = new QFrame(this);
    frame1->setFrameShape(QFrame::HLine);        // 水平分割线
    frame1->setFrameShadow(QFrame::Sunken);
    frame1->setStyleSheet("border:0.5px solid rgb(139,139,139);");
    frame1->setGeometry(QRect(10,60,465,1));    //设置默认尺寸

    //只接受大于0的整数或小数
    QRegExp regExp("^([1-9]\\d*(\\.\\d*[1-9])?)|(0\\.\\d*[1-9])$");

//*************************************华丽的分割线**********************************************

//==============================设置greaterThan_lineEdit属性=================================
    ui->greaterThan_lineEdit->setAlignment(Qt::AlignCenter);
    ui->greaterThan_lineEdit->setStyleSheet("background-image: url(:/images/resources/images/ui/flowVelocityMeasurement/inputBoxOK.png);"
                                "color: rgb(51,51,51); font: 18px \"思源黑体\"");
    ui->greaterThan_lineEdit->setValidator(new QRegExpValidator( regExp, this ));//第一、第二个参数为范围，第三个参数为父对象，指定父对象Qt会自动回收空间
//==============================设置lessThan_lineEdit属性=================================
    ui->lessThan_lineEdit->setAlignment(Qt::AlignCenter);
    ui->lessThan_lineEdit->setStyleSheet("background-image: url(:/images/resources/images/ui/flowVelocityMeasurement/inputBoxOK.png);"
                                "color: rgb(51,51,51); font: 18px \"思源黑体\"");
    // 这个行编辑只接受大于0的整数或小数
    ui->lessThan_lineEdit->setValidator(new QRegExpValidator( regExp, this ));//第一、第二个参数为范围，第三个参数为父对象，指定父对象Qt会自动回收空间
//==============================设置coefficient_lineEdit属性=================================
    ui->coefficient_lineEdit->setAlignment(Qt::AlignCenter);
    ui->coefficient_lineEdit->setStyleSheet("background-image: url(:/images/resources/images/ui/flowVelocityMeasurement/inputBoxOK.png);"
                                "color: rgb(51,51,51); font: 18px \"思源黑体\"");
    // 这个行编辑只接受大于0的整数或小数
    ui->coefficient_lineEdit->setValidator(new QRegExpValidator( regExp, this ));//第一、第二个参数为范围，第三个参数为父对象，指定父对象Qt会自动回收空间


//*************************************华丽的分割线**********************************************

//==============================设置greaterThan_lineEdit_2属性=================================
     ui->greaterThan_lineEdit_2->setAlignment(Qt::AlignCenter);
     ui->greaterThan_lineEdit_2->setStyleSheet("background-image: url(:/images/resources/images/ui/flowVelocityMeasurement/inputBoxOK.png);"
                                    "color: rgb(51,51,51); font: 18px \"思源黑体\"");
     ui->greaterThan_lineEdit_2->setValidator(new QRegExpValidator( regExp, this ));//第一、第二个参数为范围，第三个参数为父对象，指定父对象Qt会自动回收空间
//==============================设置lessThan_lineEdit_2属性=================================
     ui->lessThan_lineEdit_2->setAlignment(Qt::AlignCenter);
     ui->lessThan_lineEdit_2->setStyleSheet("background-image: url(:/images/resources/images/ui/flowVelocityMeasurement/inputBoxOK.png);"
                                    "color: rgb(51,51,51); font: 18px \"思源黑体\"");
     // 这个行编辑只接受大于0的整数或小数
     ui->lessThan_lineEdit_2->setValidator(new QRegExpValidator( regExp, this ));//第一、第二个参数为范围，第三个参数为父对象，指定父对象Qt会自动回收空间
//==============================设置coefficient_lineEdit_2属性=================================
     ui->coefficient_lineEdit_2->setAlignment(Qt::AlignCenter);
     ui->coefficient_lineEdit_2->setStyleSheet("background-image: url(:/images/resources/images/ui/flowVelocityMeasurement/inputBoxOK.png);"
                                    "color: rgb(51,51,51); font: 18px \"思源黑体\"");
     // 这个行编辑只接受大于0的整数或小数
     ui->coefficient_lineEdit_2->setValidator(new QRegExpValidator( regExp, this ));//第一、第二个参数为范围，第三个参数为父对象，指定父对象Qt会自动回收空间

//*************************************华丽的分割线**********************************************

//==============================设置greaterThan_lineEdit_3属性=================================
      ui->greaterThan_lineEdit_3->setAlignment(Qt::AlignCenter);
      ui->greaterThan_lineEdit_3->setStyleSheet("background-image: url(:/images/resources/images/ui/flowVelocityMeasurement/inputBoxOK.png);"
                                            "color: rgb(51,51,51); font: 18px \"思源黑体\"");
      ui->greaterThan_lineEdit_3->setValidator(new QRegExpValidator( regExp, this ));//第一、第二个参数为范围，第三个参数为父对象，指定父对象Qt会自动回收空间
//==============================设置lessThan_lineEdit_3属性=================================
      ui->lessThan_lineEdit_3->setAlignment(Qt::AlignCenter);
      ui->lessThan_lineEdit_3->setStyleSheet("background-image: url(:/images/resources/images/ui/flowVelocityMeasurement/inputBoxOK.png);"
                                            "color: rgb(51,51,51); font: 18px \"思源黑体\"");
      // 这个行编辑只接受大于0的整数或小数
      ui->lessThan_lineEdit_3->setValidator(new QRegExpValidator( regExp, this ));//第一、第二个参数为范围，第三个参数为父对象，指定父对象Qt会自动回收空间
//==============================设置coefficient_lineEdit_3属性=================================
      ui->coefficient_lineEdit_3->setAlignment(Qt::AlignCenter);
      ui->coefficient_lineEdit_3->setStyleSheet("background-image: url(:/images/resources/images/ui/flowVelocityMeasurement/inputBoxOK.png);"
                                            "color: rgb(51,51,51); font: 18px \"思源黑体\"");
      // 这个行编辑只接受大于0的整数或小数
      ui->coefficient_lineEdit_3->setValidator(new QRegExpValidator( regExp, this ));//第一、第二个参数为范围，第三个参数为父对象，指定父对象Qt会自动回收空间

      loadHistoricalData();//加载历史数据

//      connect(mainLeftWindow::Instance(),SIGNAL(sendBtnData(QString)),this,SLOT(preservingHistoricalData()));//临时方案

      MainWindow * _MainWindow = MainWindow::instance();
      //获取到正确的数据,保存该状态
      connect(_MainWindow,SIGNAL(transmitDataReceiveStatus_formFV()),this,SLOT(saveFV_ValueStates()));
}

SHGCVelocimeterSetUpWidget::~SHGCVelocimeterSetUpWidget()
{
    _instance = nullptr;

    SHGCFvmApplicationTipsWidget *_SHGCFvmApplicationTipsWidget = SHGCFvmApplicationTipsWidget::instance();
    if(_SHGCFvmApplicationTipsWidget)
    {
        _SHGCFvmApplicationTipsWidget->deleteInstance();
    }

    //析构时保存历史数据
    preservingHistoricalData();

    if (qtimer->isActive())
    {
        qtimer->stop();
        delete qtimer;
        qtimer = nullptr;
    }

    delete ui;
}

void SHGCVelocimeterSetUpWidget::saveFV_ValueStates()
{
    getTheRightValue_formFV = true;
}

void SHGCVelocimeterSetUpWidget::resizeEvent(QResizeEvent *event)
{
    SHGCFvmApplicationTipsWidget* _SHGCFvmApplicationTipsWidget = SHGCFvmApplicationTipsWidget::instance();
    if(_SHGCFvmApplicationTipsWidget != nullptr)
    {
//        _SHGCFvmApplicationTipsWidget->resize(320,65);

//        int w_applicationTipsWidget = this->width() / 2 - _SHGCFvmApplicationTipsWidget->width() / 2;
//        int h_applicationTipsWidget = this->height() / 2 - _SHGCFvmApplicationTipsWidget->height() / 2;
//        _SHGCFvmApplicationTipsWidget->move(w_applicationTipsWidget,h_applicationTipsWidget);
//        AutoResize::ResizeParentWidget(this,_SHGCFvmApplicationTipsWidget,AutoResize::INCLUDE_FVMAPPLICATIONTIPS);//设置界面自适应
    }
}

void SHGCVelocimeterSetUpWidget::on_return_pushButton_clicked()
{
    emit return_pushButton_isClicked();

    preservingHistoricalData();//保存历史数据
}

void SHGCVelocimeterSetUpWidget::on_close_pushButton_clicked()
{
    emit close_pushButton_isClicked();

    preservingHistoricalData();//保存历史数据
}

void SHGCVelocimeterSetUpWidget::on_reduce_pushButton_clicked()
{

}

void SHGCVelocimeterSetUpWidget::on_plus_pushButton_clicked()
{

}

void SHGCVelocimeterSetUpWidget::on_application_pushButton_clicked()
{

//*************************************华丽的分割线**********************************************
    double greaterThan = (ui->greaterThan_lineEdit->text()).toDouble();
    double lessThan = (ui->lessThan_lineEdit->text()).toDouble();
    double coefficient = (ui->coefficient_lineEdit->text()).toDouble();

    if(greaterThan >= 0.0 && greaterThan <= lessThan
            )
    {
        ui->coefficient_lineEdit->setEnabled(true);
        ui->coefficient_lineEdit->setAlignment(Qt::AlignCenter);
        ui->coefficient_lineEdit->setStyleSheet("background-image: url(:/images/resources/images/ui/flowVelocityMeasurement/inputBoxOK.png);"
                                              "color: rgb(51,51,51); font: 18px \"思源黑体\"");
        ui->greaterThan_lineEdit_2->setEnabled(true);
        ui->greaterThan_lineEdit_2->setAlignment(Qt::AlignCenter);
        ui->greaterThan_lineEdit_2->setStyleSheet("background-image: url(:/images/resources/images/ui/flowVelocityMeasurement/inputBoxOK.png);"
                                              "color: rgb(51,51,51); font: 18px \"思源黑体\"");
        ui->lessThan_lineEdit_2->setEnabled(true);
        ui->lessThan_lineEdit_2->setAlignment(Qt::AlignCenter);
        ui->lessThan_lineEdit_2->setStyleSheet("background-image: url(:/images/resources/images/ui/flowVelocityMeasurement/inputBoxOK.png);"
                                              "color: rgb(51,51,51); font: 18px \"思源黑体\"");
        ui->coefficient_lineEdit_2->setEnabled(true);
        ui->coefficient_lineEdit_2->setAlignment(Qt::AlignCenter);
        ui->coefficient_lineEdit_2->setStyleSheet("background-image: url(:/images/resources/images/ui/flowVelocityMeasurement/inputBoxOK.png);"
                                              "color: rgb(51,51,51); font: 18px \"思源黑体\"");
        ui->greaterThan_lineEdit_3->setEnabled(true);
        ui->greaterThan_lineEdit_3->setAlignment(Qt::AlignCenter);
        ui->greaterThan_lineEdit_3->setStyleSheet("background-image: url(:/images/resources/images/ui/flowVelocityMeasurement/inputBoxOK.png);"
                                              "color: rgb(51,51,51); font: 18px \"思源黑体\"");
        ui->lessThan_lineEdit_3->setEnabled(true);
        ui->lessThan_lineEdit_3->setAlignment(Qt::AlignCenter);
        ui->lessThan_lineEdit_3->setStyleSheet("background-image: url(:/images/resources/images/ui/flowVelocityMeasurement/inputBoxOK.png);"
                                              "color: rgb(51,51,51); font: 18px \"思源黑体\"");

        ui->coefficient_lineEdit_3->setEnabled(true);
        ui->coefficient_lineEdit_3->setAlignment(Qt::AlignCenter);
        ui->coefficient_lineEdit_3->setStyleSheet("background-image: url(:/images/resources/images/ui/flowVelocityMeasurement/inputBoxOK.png);"
                                              "color: rgb(51,51,51); font: 18px \"思源黑体\"");

        ui->lessThan_lineEdit->setAlignment(Qt::AlignCenter);
        ui->lessThan_lineEdit->setStyleSheet("background-image: url(:/images/resources/images/ui/flowVelocityMeasurement/inputBoxOK.png);"
                                                  "color: rgb(51,51,51); font: 18px \"思源黑体\"");

        emit application_pushButton_isClicked_1(greaterThan,lessThan,coefficient);

        if(getTheRightValue_formFV == false)
        {
            emit changeText_Unknown();
        }
        else
        {
            emit changeText_Ok();
        }

    }
    else
    {
        ui->lessThan_lineEdit->setAlignment(Qt::AlignCenter);
        ui->lessThan_lineEdit->setStyleSheet("background-image: url(:/images/resources/images/ui/flowVelocityMeasurement/inputBoxNot.png);"
                                                  "color: rgb(255,0,0); font: 18px \"思源黑体\"");

        ui->coefficient_lineEdit->setEnabled(false);
        ui->coefficient_lineEdit->setAlignment(Qt::AlignCenter);
        ui->coefficient_lineEdit->setStyleSheet("background-image: url(:/images/resources/images/ui/flowVelocityMeasurement/inputBoxNot.png);"
                                              "color: rgb(51,51,51); font: 18px \"思源黑体\"");
        ui->greaterThan_lineEdit_2->setEnabled(false);
        ui->greaterThan_lineEdit_2->setAlignment(Qt::AlignCenter);
        ui->greaterThan_lineEdit_2->setStyleSheet("background-image: url(:/images/resources/images/ui/flowVelocityMeasurement/inputBoxNot.png);"
                                              "color: rgb(51,51,51); font: 18px \"思源黑体\"");
        ui->lessThan_lineEdit_2->setEnabled(false);
        ui->lessThan_lineEdit_2->setAlignment(Qt::AlignCenter);
        ui->lessThan_lineEdit_2->setStyleSheet("background-image: url(:/images/resources/images/ui/flowVelocityMeasurement/inputBoxNot.png);"
                                              "color: rgb(51,51,51); font: 18px \"思源黑体\"");
        ui->coefficient_lineEdit_2->setEnabled(false);
        ui->coefficient_lineEdit_2->setAlignment(Qt::AlignCenter);
        ui->coefficient_lineEdit_2->setStyleSheet("background-image: url(:/images/resources/images/ui/flowVelocityMeasurement/inputBoxNot.png);"
                                              "color: rgb(51,51,51); font: 18px \"思源黑体\"");
        ui->greaterThan_lineEdit_3->setEnabled(false);
        ui->greaterThan_lineEdit_3->setAlignment(Qt::AlignCenter);
        ui->greaterThan_lineEdit_3->setStyleSheet("background-image: url(:/images/resources/images/ui/flowVelocityMeasurement/inputBoxNot.png);"
                                              "color: rgb(51,51,51); font: 18px \"思源黑体\"");
        ui->lessThan_lineEdit_3->setEnabled(false);
        ui->lessThan_lineEdit_3->setAlignment(Qt::AlignCenter);
        ui->lessThan_lineEdit_3->setStyleSheet("background-image: url(:/images/resources/images/ui/flowVelocityMeasurement/inputBoxNot.png);"
                                              "color: rgb(51,51,51); font: 18px \"思源黑体\"");

        ui->coefficient_lineEdit_3->setEnabled(false);
        ui->coefficient_lineEdit_3->setAlignment(Qt::AlignCenter);
        ui->coefficient_lineEdit_3->setStyleSheet("background-image: url(:/images/resources/images/ui/flowVelocityMeasurement/inputBoxNot.png);"
                                              "color: rgb(51,51,51); font: 18px \"思源黑体\"");
        if(getTheRightValue_formFV == false)
        {
            emit changeText_Unknown();
        }
        else
        {
            emit changeText_Error();
        }

    }

//*************************************华丽的分割线**********************************************

    double greaterThan_2 = (ui->greaterThan_lineEdit_2->text()).toDouble();
    double lessThan_2 = (ui->lessThan_lineEdit_2->text()).toDouble();
    double coefficient_2 = (ui->coefficient_lineEdit_2->text()).toDouble();

    if(greaterThan >= 0.0 && greaterThan <= lessThan &&
            greaterThan_2 >= lessThan && greaterThan_2 <= lessThan_2
            )
    {
        ui->coefficient_lineEdit_2->setEnabled(true);
        ui->coefficient_lineEdit_2->setAlignment(Qt::AlignCenter);
        ui->coefficient_lineEdit_2->setStyleSheet("background-image: url(:/images/resources/images/ui/flowVelocityMeasurement/inputBoxOK.png);"
                                              "color: rgb(51,51,51); font: 18px \"思源黑体\"");
        ui->greaterThan_lineEdit_3->setEnabled(true);
        ui->greaterThan_lineEdit_3->setAlignment(Qt::AlignCenter);
        ui->greaterThan_lineEdit_3->setStyleSheet("background-image: url(:/images/resources/images/ui/flowVelocityMeasurement/inputBoxOK.png);"
                                              "color: rgb(51,51,51); font: 18px \"思源黑体\"");
        ui->lessThan_lineEdit_3->setEnabled(true);
        ui->lessThan_lineEdit_3->setAlignment(Qt::AlignCenter);
        ui->lessThan_lineEdit_3->setStyleSheet("background-image: url(:/images/resources/images/ui/flowVelocityMeasurement/inputBoxOK.png);"
                                              "color: rgb(51,51,51); font: 18px \"思源黑体\"");
        ui->coefficient_lineEdit_3->setEnabled(true);
        ui->coefficient_lineEdit_3->setAlignment(Qt::AlignCenter);
        ui->coefficient_lineEdit_3->setStyleSheet("background-image: url(:/images/resources/images/ui/flowVelocityMeasurement/inputBoxOK.png);"
                                              "color: rgb(51,51,51); font: 18px \"思源黑体\"");

        ui->greaterThan_lineEdit_2->setAlignment(Qt::AlignCenter);
        ui->greaterThan_lineEdit_2->setStyleSheet("background-image: url(:/images/resources/images/ui/flowVelocityMeasurement/inputBoxOK.png);"
                                                  "color: rgb(51,51,51); font: 18px \"思源黑体\"");

        ui->lessThan_lineEdit_2->setAlignment(Qt::AlignCenter);
        ui->lessThan_lineEdit_2->setStyleSheet("background-image: url(:/images/resources/images/ui/flowVelocityMeasurement/inputBoxOK.png);"
                                               "color: rgb(51,51,51); font: 18px \"思源黑体\"");

        emit application_pushButton_isClicked_2(greaterThan_2,lessThan_2,coefficient_2);
        if(getTheRightValue_formFV == false)
        {
            emit changeText_Unknown();
        }
        else
        {
            emit changeText_Ok();
        }

    }
    else
    {
       if(greaterThan_2 < lessThan)
       {
           ui->greaterThan_lineEdit_2->setAlignment(Qt::AlignCenter);
           ui->greaterThan_lineEdit_2->setStyleSheet("background-image: url(:/images/resources/images/ui/flowVelocityMeasurement/inputBoxNot.png);"
                                                     "color: rgb(255,0,0); font: 18px \"思源黑体\"");
           ui->lessThan_lineEdit_2->setEnabled(false);
           ui->lessThan_lineEdit_2->setAlignment(Qt::AlignCenter);
           ui->lessThan_lineEdit_2->setStyleSheet("background-image: url(:/images/resources/images/ui/flowVelocityMeasurement/inputBoxNot.png);"
                                                 "color: rgb(51,51,51); font: 18px \"思源黑体\"");
       }

       if(lessThan_2 < greaterThan_2)
       {
           ui->lessThan_lineEdit_2->setAlignment(Qt::AlignCenter);
           ui->lessThan_lineEdit_2->setStyleSheet("background-image: url(:/images/resources/images/ui/flowVelocityMeasurement/inputBoxNot.png);"
                                                     "color: rgb(255,0,0); font: 18px \"思源黑体\"");
       }

        ui->coefficient_lineEdit_2->setEnabled(false);
        ui->coefficient_lineEdit_2->setAlignment(Qt::AlignCenter);
        ui->coefficient_lineEdit_2->setStyleSheet("background-image: url(:/images/resources/images/ui/flowVelocityMeasurement/inputBoxNot.png);"
                                              "color: rgb(51,51,51); font: 18px \"思源黑体\"");
        ui->greaterThan_lineEdit_3->setEnabled(false);
        ui->greaterThan_lineEdit_3->setAlignment(Qt::AlignCenter);
        ui->greaterThan_lineEdit_3->setStyleSheet("background-image: url(:/images/resources/images/ui/flowVelocityMeasurement/inputBoxNot.png);"
                                              "color: rgb(51,51,51); font: 18px \"思源黑体\"");
        ui->lessThan_lineEdit_3->setEnabled(false);
        ui->lessThan_lineEdit_3->setAlignment(Qt::AlignCenter);
        ui->lessThan_lineEdit_3->setStyleSheet("background-image: url(:/images/resources/images/ui/flowVelocityMeasurement/inputBoxNot.png);"
                                              "color: rgb(51,51,51); font: 18px \"思源黑体\"");

        ui->coefficient_lineEdit_3->setEnabled(false);
        ui->coefficient_lineEdit_3->setAlignment(Qt::AlignCenter);
        ui->coefficient_lineEdit_3->setStyleSheet("background-image: url(:/images/resources/images/ui/flowVelocityMeasurement/inputBoxNot.png);"
                                              "color: rgb(51,51,51); font: 18px \"思源黑体\"");

        emit changeText_Error();
    }


//*************************************华丽的分割线**********************************************

    double greaterThan_3 = (ui->greaterThan_lineEdit_3->text()).toDouble();
    double lessThan_3 = (ui->lessThan_lineEdit_3->text()).toDouble();
    double coefficient_3 = (ui->coefficient_lineEdit_3->text()).toDouble();

    if(greaterThan >= 0.0 && greaterThan <= lessThan &&
            greaterThan_2 >= lessThan && greaterThan_2 <= lessThan_2 &&
            greaterThan_3 >= lessThan_2 && greaterThan_3 <= lessThan_3
        )
    {
        ui->coefficient_lineEdit_3->setEnabled(true);
        ui->coefficient_lineEdit_3->setAlignment(Qt::AlignCenter);
        ui->coefficient_lineEdit_3->setStyleSheet("background-image: url(:/images/resources/images/ui/flowVelocityMeasurement/inputBoxOK.png);"
                                              "color: rgb(51,51,51); font: 18px \"思源黑体\"");

        ui->greaterThan_lineEdit_3->setAlignment(Qt::AlignCenter);
        ui->greaterThan_lineEdit_3->setStyleSheet("background-image: url(:/images/resources/images/ui/flowVelocityMeasurement/inputBoxOK.png);"
                                                  "color: rgb(51,51,51); font: 18px \"思源黑体\"");

        emit application_pushButton_isClicked_3(greaterThan_3,lessThan_3,coefficient_3);

        if(getTheRightValue_formFV == false)
        {
            emit changeText_Unknown();
        }
        else
        {
            emit changeText_Ok();
        }

    }
    else
    {
        if(greaterThan_3 < lessThan_2)
        {
            ui->greaterThan_lineEdit_3->setAlignment(Qt::AlignCenter);
            ui->greaterThan_lineEdit_3->setStyleSheet("background-image: url(:/images/resources/images/ui/flowVelocityMeasurement/inputBoxNot.png);"
                                                      "color: rgb(255,0,0); font: 18px \"思源黑体\"");
            ui->lessThan_lineEdit_3->setEnabled(false);
            ui->lessThan_lineEdit_3->setAlignment(Qt::AlignCenter);
            ui->lessThan_lineEdit_3->setStyleSheet("background-image: url(:/images/resources/images/ui/flowVelocityMeasurement/inputBoxNot.png);"
                                                   "color: rgb(51,51,51); font: 18px \"思源黑体\"");
        }

        if(lessThan_3 < greaterThan_3)
        {
            ui->lessThan_lineEdit_3->setAlignment(Qt::AlignCenter);
            ui->lessThan_lineEdit_3->setStyleSheet("background-image: url(:/images/resources/images/ui/flowVelocityMeasurement/inputBoxNot.png);"
                                                              "color: rgb(255,0,0); font: 18px \"思源黑体\"");
        }

        ui->coefficient_lineEdit_3->setEnabled(false);
        ui->coefficient_lineEdit_3->setAlignment(Qt::AlignCenter);
        ui->coefficient_lineEdit_3->setStyleSheet("background-image: url(:/images/resources/images/ui/flowVelocityMeasurement/inputBoxNot.png);"
                                              "color: rgb(51,51,51); font: 18px \"思源黑体\"");

        emit changeText_Error();
    }

//*************************************华丽的分割线**********************************************

    //第一行不为空
    if(!ui->greaterThan_lineEdit->text().isEmpty() &&
            !ui->lessThan_lineEdit->text().isEmpty() &&
            !ui->coefficient_lineEdit->text().isEmpty() &&
            ui->greaterThan_lineEdit_2->text().isEmpty() &&
            ui->lessThan_lineEdit_2->text().isEmpty() &&
            ui->coefficient_lineEdit_2->text().isEmpty() &&
            ui->lessThan_lineEdit_3->text().isEmpty() &&
            ui->coefficient_lineEdit_3->text().isEmpty()
            )
    {
        ui->greaterThan_lineEdit_2->setEnabled(true);

        ui->lessThan_lineEdit_2->setEnabled(true);

        ui->coefficient_lineEdit_2->setEnabled(true);

        ui->greaterThan_lineEdit_3->setEnabled(true);

        ui->lessThan_lineEdit_3->setEnabled(true);

        ui->coefficient_lineEdit_3->setEnabled(true);

        if(getTheRightValue_formFV == false)
        {
            emit changeText_Unknown();
        }
        else
        {
            emit changeText_Ok();
        }

    }

    //第一、二行不为空
    if(!ui->greaterThan_lineEdit->text().isEmpty() &&
            !ui->lessThan_lineEdit->text().isEmpty() &&
            !ui->coefficient_lineEdit->text().isEmpty() &&
            !ui->greaterThan_lineEdit_2->text().isEmpty() &&
            !ui->lessThan_lineEdit_2->text().isEmpty() &&
            !ui->coefficient_lineEdit_2->text().isEmpty() &&
            ui->greaterThan_lineEdit_3->text().isEmpty() &&
            ui->lessThan_lineEdit_3->text().isEmpty() &&
            ui->coefficient_lineEdit_3->text().isEmpty()
            )
    {
        ui->greaterThan_lineEdit_3->setEnabled(true);

        ui->lessThan_lineEdit_3->setEnabled(true);

        ui->coefficient_lineEdit_3->setEnabled(true);

        if(getTheRightValue_formFV == false)
        {
            emit changeText_Unknown();
        }
        else
        {
            emit changeText_Ok();
        }

    }

    //第一、三行不为空
    if(!ui->greaterThan_lineEdit->text().isEmpty() &&
            !ui->lessThan_lineEdit->text().isEmpty() &&
            !ui->coefficient_lineEdit->text().isEmpty() &&
            ui->greaterThan_lineEdit_2->text().isEmpty() &&
            ui->lessThan_lineEdit_2->text().isEmpty() &&
            ui->coefficient_lineEdit_2->text().isEmpty() &&
            !ui->greaterThan_lineEdit_3->text().isEmpty() &&
            !ui->lessThan_lineEdit_3->text().isEmpty() &&
            !ui->coefficient_lineEdit_3->text().isEmpty()
            )
    {
        ui->greaterThan_lineEdit_2->setEnabled(true);

        ui->lessThan_lineEdit_2->setEnabled(true);

        ui->coefficient_lineEdit_2->setEnabled(true);

        ui->greaterThan_lineEdit_3->setEnabled(true);
        ui->greaterThan_lineEdit_3->setAlignment(Qt::AlignCenter);
        ui->greaterThan_lineEdit_3->setStyleSheet("background-image: url(:/images/resources/images/ui/flowVelocityMeasurement/inputBoxOK.png);"
                                              "color: rgb(51,51,51); font: 18px \"思源黑体\"");
        ui->lessThan_lineEdit_3->setEnabled(true);
        ui->lessThan_lineEdit_3->setAlignment(Qt::AlignCenter);
        ui->lessThan_lineEdit_3->setStyleSheet("background-image: url(:/images/resources/images/ui/flowVelocityMeasurement/inputBoxOK.png);"
                                              "color: rgb(51,51,51); font: 18px \"思源黑体\"");
        ui->coefficient_lineEdit_3->setEnabled(true);
        ui->coefficient_lineEdit_3->setAlignment(Qt::AlignCenter);
        ui->coefficient_lineEdit_3->setStyleSheet("background-image: url(:/images/resources/images/ui/flowVelocityMeasurement/inputBoxOK.png);"
                                              "color: rgb(51,51,51); font: 18px \"思源黑体\"");
        if(getTheRightValue_formFV == false)
        {
            emit changeText_Unknown();
        }
        else
        {
            emit changeText_Ok();
        }

    }

    //第二行不为空
    if(ui->greaterThan_lineEdit->text().isEmpty() &&
            ui->lessThan_lineEdit->text().isEmpty() &&
            ui->coefficient_lineEdit->text().isEmpty() &&
            !ui->greaterThan_lineEdit_2->text().isEmpty() &&
            !ui->lessThan_lineEdit_2->text().isEmpty() &&
            !ui->coefficient_lineEdit_2->text().isEmpty() &&
            ui->greaterThan_lineEdit_3->text().isEmpty() &&
            ui->lessThan_lineEdit_3->text().isEmpty() &&
            ui->coefficient_lineEdit_3->text().isEmpty()
            )
    {
        ui->greaterThan_lineEdit_3->setEnabled(true);

        ui->lessThan_lineEdit_3->setEnabled(true);

        ui->coefficient_lineEdit_3->setEnabled(true);

        state_2Or3 = true;

        if(getTheRightValue_formFV == false)
        {
            emit changeText_Unknown();
        }
        else
        {
            emit changeText_Ok();
        }

    }

    //第二、三行不为空
    if(ui->greaterThan_lineEdit->text().isEmpty() &&
            ui->lessThan_lineEdit->text().isEmpty() &&
            ui->coefficient_lineEdit->text().isEmpty() &&
            !ui->greaterThan_lineEdit_2->text().isEmpty() &&
            !ui->lessThan_lineEdit_2->text().isEmpty() &&
            !ui->coefficient_lineEdit_2->text().isEmpty() &&
            !ui->greaterThan_lineEdit_3->text().isEmpty() &&
            !ui->lessThan_lineEdit_3->text().isEmpty() &&
            !ui->coefficient_lineEdit_3->text().isEmpty()
            )
    {
        state_2Or3 = true;

        if(getTheRightValue_formFV == false)
        {
            emit changeText_Unknown();
        }
        else
        {
            emit changeText_Ok();
        }

    }

    //第三行不为空
    if(ui->greaterThan_lineEdit->text().isEmpty() &&
            ui->lessThan_lineEdit->text().isEmpty() &&
            ui->coefficient_lineEdit->text().isEmpty() &&
            ui->greaterThan_lineEdit_2->text().isEmpty() &&
            ui->lessThan_lineEdit_2->text().isEmpty() &&
            ui->coefficient_lineEdit_2->text().isEmpty() &&
            !ui->greaterThan_lineEdit_3->text().isEmpty() &&
            !ui->lessThan_lineEdit_3->text().isEmpty() &&
            !ui->coefficient_lineEdit_3->text().isEmpty()
            )
    {
        state_2Or3 = true;

        if(getTheRightValue_formFV == false)
        {
            emit changeText_Unknown();
        }
        else
        {
            emit changeText_Ok();
        }

    }

//*************************************华丽的分割线**********************************************

    if(ui->greaterThan_lineEdit->text().isEmpty() &&
            state_2Or3 != true)
    {
        ui->greaterThan_lineEdit->setAlignment(Qt::AlignCenter);
        ui->greaterThan_lineEdit->setStyleSheet("background-image: url(:/images/resources/images/ui/flowVelocityMeasurement/inputBoxNot.png);"
                                                          "color: rgb(255,0,0); font: 18px \"思源黑体\"");

        emit changeText_Error();
    }
    else
    {
        ui->greaterThan_lineEdit->setAlignment(Qt::AlignCenter);
        ui->greaterThan_lineEdit->setStyleSheet("background-image: url(:/images/resources/images/ui/flowVelocityMeasurement/inputBoxOK.png);"
                                              "color: rgb(51,51,51); font: 18px \"思源黑体\"");
    }

    if(!ui->greaterThan_lineEdit->text().isEmpty() &&
            !ui->lessThan_lineEdit->text().isEmpty() &&
            ui->coefficient_lineEdit->text().isEmpty()
            )
    {
        ui->coefficient_lineEdit->setAlignment(Qt::AlignCenter);
        ui->coefficient_lineEdit->setStyleSheet("background-image: url(:/images/resources/images/ui/flowVelocityMeasurement/inputBoxNot.png);"
                                                          "color: rgb(255,0,0); font: 18px \"思源黑体\"");

        emit changeText_Error();
    }

    if(!ui->greaterThan_lineEdit_2->text().isEmpty() &&
            !ui->lessThan_lineEdit_2->text().isEmpty() &&
            ui->coefficient_lineEdit_2->text().isEmpty()
            )
    {
        ui->coefficient_lineEdit_2->setAlignment(Qt::AlignCenter);
        ui->coefficient_lineEdit_2->setStyleSheet("background-image: url(:/images/resources/images/ui/flowVelocityMeasurement/inputBoxNot.png);"
                                                          "color: rgb(255,0,0); font: 18px \"思源黑体\"");

        emit changeText_Error();
    }

    if(!ui->greaterThan_lineEdit_3->text().isEmpty() &&
            !ui->lessThan_lineEdit_3->text().isEmpty() &&
            ui->coefficient_lineEdit_3->text().isEmpty()
            )
    {
        ui->coefficient_lineEdit_3->setAlignment(Qt::AlignCenter);
        ui->coefficient_lineEdit_3->setStyleSheet("background-image: url(:/images/resources/images/ui/flowVelocityMeasurement/inputBoxNot.png);"
                                                          "color: rgb(255,0,0); font: 18px \"思源黑体\"");

        emit changeText_Error();
    }


//*************************************华丽的分割线**********************************************
//    connect(qgcapplicationTipsWidget::Instance(this),SIGNAL(textStates(bool)),this,SLOT(TextInitializationOK(bool)));

//    if(textOK == false)
//    {

        SHGCFvmApplicationTipsWidget *_SHGCFvmApplicationTipsWidget = SHGCFvmApplicationTipsWidget::instance();
//        SHGCFvmApplicationTipsWidget *_SHGCFvmApplicationTipsWidget = SHGCFvmApplicationTipsWidget::_create(this);

        int w_applicationTipsWidget = this->width() / 2 - _SHGCFvmApplicationTipsWidget->width() / 2;
        int h_applicationTipsWidget = this->height() / 2 - _SHGCFvmApplicationTipsWidget->height() / 2;

        _SHGCFvmApplicationTipsWidget->move(w_applicationTipsWidget,h_applicationTipsWidget);
        _SHGCFvmApplicationTipsWidget->show();

        connect(qtimer, SIGNAL(timeout()), this, SLOT(close_qgcapplicationTipsWidget()));
        qtimer->start(3000);
//    }
}

//void SHGCVelocimeterSetUpWidget::TextInitializationOK(bool b)
//{
//    textOK = b;
//}


void SHGCVelocimeterSetUpWidget::close_qgcapplicationTipsWidget()
{
    SHGCFvmApplicationTipsWidget *_SHGCFvmApplicationTipsWidget = SHGCFvmApplicationTipsWidget::instance();

    if(_SHGCFvmApplicationTipsWidget != nullptr)
    {
        _SHGCFvmApplicationTipsWidget->close();
    }
}

//初始化定时器
void SHGCVelocimeterSetUpWidget::initqTimer()
{
    this->qtimer = new QTimer(this);
}


void SHGCVelocimeterSetUpWidget::preservingHistoricalData()
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

    QString path = "ini/cal_HistoricalData.ini";
    QString aw_No1 = ui->greaterThan_lineEdit->text();
    QString bw_No1 = ui->lessThan_lineEdit->text();
    QString cw_No1 = ui->coefficient_lineEdit->text();

    QString aw_No2 = ui->greaterThan_lineEdit_2->text();
    QString bw_No2 = ui->lessThan_lineEdit_2->text();
    QString cw_No2 = ui->coefficient_lineEdit_2->text();

    QString aw_No3 = ui->greaterThan_lineEdit_3->text();
    QString bw_No3 = ui->lessThan_lineEdit_3->text();
    QString cw_No3 = ui->coefficient_lineEdit_3->text();

    QSettings settings(path, QSettings::IniFormat);
    settings.beginGroup("DevOption");
    settings.setValue("greaterThan_1", aw_No1);
    settings.setValue("lessThan_1", bw_No1);
    settings.setValue("coefficient_1", cw_No1);

    settings.setValue("dividingLine_1", "===================");

    settings.setValue("greaterThan_2", aw_No2);
    settings.setValue("lessThan_2", bw_No2);
    settings.setValue("coefficient_2", cw_No2);

    settings.setValue("dividingLine_2", "===================");

    settings.setValue("greaterThan_3", aw_No3);
    settings.setValue("lessThan_3", bw_No3);
    settings.setValue("coefficient_3", cw_No3);
    settings.endGroup();

    qDebug()<<"ini文件保存成功！ 1";
}


void SHGCVelocimeterSetUpWidget::loadHistoricalData()
{
    QString path ="ini/cal_HistoricalData.ini";
    QSettings settings(path, QSettings::IniFormat);

    QString ar_No1 = settings.value("DevOption/greaterThan_1").toString();
    QString br_No1 = settings.value("DevOption/lessThan_1").toString();
    QString cr_No1 = settings.value("DevOption/coefficient_1").toString();

    QString ar_No2 = settings.value("DevOption/greaterThan_2").toString();
    QString br_No2 = settings.value("DevOption/lessThan_2").toString();
    QString cr_No2 = settings.value("DevOption/coefficient_2").toString();

    QString ar_No3 = settings.value("DevOption/greaterThan_3").toString();
    QString br_No3 = settings.value("DevOption/lessThan_3").toString();
    QString cr_No3 = settings.value("DevOption/coefficient_3").toString();

    ui->greaterThan_lineEdit->setText(ar_No1);
    ui->lessThan_lineEdit->setText(br_No1);
    ui->coefficient_lineEdit->setText(cr_No1);

    ui->greaterThan_lineEdit_2->setText(ar_No2);
    ui->lessThan_lineEdit_2->setText(br_No2);
    ui->coefficient_lineEdit_2->setText(cr_No2);

    ui->greaterThan_lineEdit_3->setText(ar_No3);
    ui->lessThan_lineEdit_3->setText(br_No3);
    ui->coefficient_lineEdit_3->setText(cr_No3);

    qDebug()<<"ini文件加载成功！ 2";
}
