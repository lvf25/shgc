#include "SHGCFvmApplicationTipsWidget.h"
#include "ui_SHGCFvmApplicationTipsWidget.h"


static SHGCFvmApplicationTipsWidget* _instance = nullptr;

SHGCFvmApplicationTipsWidget* SHGCFvmApplicationTipsWidget::_create(QWidget *parent)
{
    _instance = new SHGCFvmApplicationTipsWidget(parent);
    return _instance;
}

SHGCFvmApplicationTipsWidget* SHGCFvmApplicationTipsWidget::instance(void)
{
    return _instance;
}

void SHGCFvmApplicationTipsWidget::deleteInstance(void)
{
    delete this;
}


SHGCFvmApplicationTipsWidget::SHGCFvmApplicationTipsWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SHGCFvmApplicationTipsWidget)
{
    ui->setupUi(this);

    m_autoResizeHandler=new AutoResize(this,this->rect().width(),this->rect().height());
    m_autoResizeHandler->setAutoResizeFlag(
            AutoResize::INCLUDE_FVMAPPLICATIONTIPS|
            AutoResize::INCLUDE_WIDGET|
            AutoResize::INCLUDE_LABEL
            );
    m_autoResizeHandler->pushAllResizeItem();

    ui->label->setAlignment(Qt::AlignCenter);
    ui->label->setStyleSheet("color: rgb(255,0,0); font: 18px \"思源黑体\"");
    ui->label->setText("首次应用，若流速原始值为空...\n设置无效时，请有值后再试！");

    connect(_MianWindow,SIGNAL(changeText_isUnknown()),this,SLOT(changeLabelText_isUnknown()));
    connect(_MianWindow,SIGNAL(changeText_isOk()),this,SLOT(changeLabelText_isOk()));
    connect(_MianWindow,SIGNAL(changeText_isError()),this,SLOT(changeLabelText_isError()));
    connect(_MianWindow,SIGNAL(showTipsWidget(bool)),this,SLOT(changeLabelText_isWait(bool)));

     qDebug()<<"SHGCFvmApplicationTipsWidget 构造函数！";
}

SHGCFvmApplicationTipsWidget::~SHGCFvmApplicationTipsWidget()
{
    _instance = nullptr;
    delete ui;
}

void SHGCFvmApplicationTipsWidget::changeLabelText_isUnknown()
{
    emit textStates(false);
    ui->label->setAlignment(Qt::AlignCenter);
    ui->label->setStyleSheet("color: rgb(51,51,51); font: 18px \"思源黑体\"");
    ui->label->setText("首次应用，若当前未进行首次测量...\n设置无效时，请流速有值后再试！");

    //槽函数结束之前先断开信号和槽 避免槽函数多次重复调用
//    disconnect(_MianWindow,SIGNAL(changeText_isOk()),this,SLOT(changeLabelText_isOk()));
}


void SHGCFvmApplicationTipsWidget::changeLabelText_isOk()
{
    emit textStates(false);
    ui->label->setAlignment(Qt::AlignCenter);
    ui->label->setStyleSheet("color: rgb(51,51,51); font: 18px \"思源黑体\"");
    ui->label->setText("应用成功");

    //槽函数结束之前先断开信号和槽 避免槽函数多次重复调用
//    disconnect(_MianWindow,SIGNAL(changeText_isOk()),this,SLOT(changeLabelText_isOk()));
}

void SHGCFvmApplicationTipsWidget::changeLabelText_isError()
{
    emit textStates(false);;
    ui->label->setAlignment(Qt::AlignCenter);
    ui->label->setStyleSheet("color: rgb(51,51,51); font: 18px \"思源黑体\"");
    ui->label->setText("参数违规，请重新设置");

    //槽函数结束之前先断开信号和槽 避免槽函数多次重复调用
//    disconnect(_MianWindow,SIGNAL(changeText_isError()),this,SLOT(changeLabelText_isError()));
}


void SHGCFvmApplicationTipsWidget::changeLabelText_isWait(bool b)
{
    emit textStates(true);
    if(b == true)
    {
        ui->label->setAlignment(Qt::AlignCenter);
        ui->label->setStyleSheet("color: rgb(51,51,51); font: 18px \"思源黑体\"");
        ui->label->setText("正在等待水位计\n返回数据，请稍后再试！");
    }
    //槽函数结束之前先断开信号和槽 避免槽函数多次重复调用
//    disconnect(_MianWindow,SIGNAL(showTipsWidget(bool)),this,SLOT(changeLabelText_isWait(bool)));
}
