#ifndef AUTOSAVESLIDER_H
#define AUTOSAVESLIDER_H

#include <QSlider>
#include <QLabel>
#include <QMouseEvent>

class autoSaveSlider : public QSlider
{
public:
    autoSaveSlider(QWidget *parent=0);
    ~autoSaveSlider();

protected:
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseMoveEvent(QMouseEvent *event);
    virtual void wheelEvent(QWheelEvent * event);
};

#endif // AUTOSAVESLIDER_H
