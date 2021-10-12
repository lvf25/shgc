#ifndef SLIDERPOINTER_H
#define SLIDERPOINTER_H
#include <QSlider>
#include <QLabel>
#include <QMouseEvent>

class sliderPointer : public QSlider
{
public:
    sliderPointer(QWidget *parent=0);
    ~sliderPointer();

protected:
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseReleaseEvent(QMouseEvent *event);
    virtual void mouseMoveEvent(QMouseEvent *event);
    virtual void wheelEvent(QWheelEvent * event);
    void voiceTipTime();

public:
    QLabel* m_displayLabel;
};

#endif // SLIDERPOINTER_H
