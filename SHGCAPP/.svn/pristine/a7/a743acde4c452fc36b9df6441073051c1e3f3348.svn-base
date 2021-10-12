#ifndef ELECPOINTER_H
#define ELECPOINTER_H

#include <QSlider>
#include <QLabel>
#include <QMouseEvent>

class elecPointer : public QSlider
{
public:
    elecPointer(QWidget *parent=0);
    ~elecPointer();

protected:
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseReleaseEvent(QMouseEvent *event);
    virtual void mouseMoveEvent(QMouseEvent *event);
    virtual void wheelEvent(QWheelEvent * event);
    void elecTipTime();

public:
    QLabel* n_displayLabel;
};

#endif // ELECPOINTER_H
