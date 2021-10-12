#include "autoSaveSlider.h"

autoSaveSlider::autoSaveSlider(QWidget *parent):QSlider(parent)
{
    this->setMinimum(0);
    this->setMaximum(100);
    this->setValue(15);
    this->setStyleSheet("QSlider::groove:horizontal{"
                        "background:#6D6D6D;"
                        "height:10px;"
                        "border: 1px solid #6D6D6D;}"

                        "QSlider::add-page:horizontal{"
                        "background:white;"
                        "height:10px;"
                        "border: 1px solid #6D6D6D;}"

                        "QSlider::sub-page:horizontal{"
                        "background:#6D6D6D;"
                        "height:10px;"
                        "border: 1px solid #6D6D6D;}"

                        "QSlider::handle:horizontal{"
                        "width:13px;"
                        "margin-top:-3px;"
                        "margin-bottom:-3px;}");

}

autoSaveSlider::~autoSaveSlider()
{

}

void autoSaveSlider::mousePressEvent(QMouseEvent *event)
{
    //鼠标点击时不做任何处理
}

void autoSaveSlider::mouseMoveEvent(QMouseEvent *event)
{
    //鼠标拖动时不做任何处理
}

void autoSaveSlider::wheelEvent(QWheelEvent * event)
{
    //滚轮操作不做任何处理
}




















