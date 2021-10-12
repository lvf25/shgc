#include "clickedWidget.h"
#include "MainWindow.h"

clickedWidget::clickedWidget(QWidget* parent) :
    QWidget(parent)
{

}

clickedWidget::~clickedWidget()
{

}

//为clickedWidget界面添加鼠标点击事件
void clickedWidget::mousePressEvent(QMouseEvent * e)
{
    if(e->button() == Qt::LeftButton)
    {
        emit widget_clicked();
    }
}






