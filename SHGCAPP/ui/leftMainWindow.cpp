#include "leftMainWindow.h"
#include "ui_leftMainWindow.h"
#include <QSettings>
#include "QGCApplication.h"

static leftMainWindow* _instance = nullptr;   ///< @brief MainWindow singleton

leftMainWindow* leftMainWindow::_create(QWidget *parent)
{
    _instance = new leftMainWindow(parent);
    return _instance;
}

leftMainWindow* leftMainWindow::instance(void)
{
    return _instance;
}

void leftMainWindow::deleteInstance(void)
{
    delete this;
}


leftMainWindow::leftMainWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::leftMainWindow)
{
    ui->setupUi(this);

    this->setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint);
    this->setStyleSheet("QWidget#leftMainWindow{background-color: rgba(0, 45, 65, 0.8);}");
    this->setStyleSheet("QLable{background-color: rgba(0, 45, 65, 0);}");
}

leftMainWindow::~leftMainWindow()
{
    _instance = nullptr;
    delete ui;
}


//点击空白关闭窗口
bool leftMainWindow::event(QEvent *event)
{
    if (event->type() == QEvent::ActivationChange)
    {
        if(QGCApplication::activeWindow() != this)
        {
            this->close();
        }
    }
    return QWidget::event(event);
}

void leftMainWindow::on_btnFlightMonit_clicked()
{
    emit sendBtnData("btnFlightMonit");

}

void leftMainWindow::on_btnRoutePlan_clicked()
{
    emit sendBtnData("btnRoutePlan");

}

void leftMainWindow::on_btnActionAdjust_clicked()
{
    emit sendBtnData("btnActionAdjust");
}

void leftMainWindow::on_btnActionSetUp_clicked()
{
    emit sendBtnData("btnActionSetUp");
}
