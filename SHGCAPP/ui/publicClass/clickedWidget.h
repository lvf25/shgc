#ifndef CLICKEDWIDGET_H
#define CLICKEDWIDGET_H

#include <QWidget>
//#include "MainWindow.h"


class clickedWidget : public QWidget
{
    Q_OBJECT

public:
    explicit clickedWidget(QWidget* parent);
    ~clickedWidget();

public:
    void mousePressEvent(QMouseEvent* e);

signals:
    void widget_clicked();

};

#endif // CLICKEDWIDGET_H
