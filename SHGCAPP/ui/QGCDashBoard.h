#pragma once

#include <QWidget>
#include <QTimer>

#include "Vehicle.h"
#include "AutoResize.h"


namespace Ui {
class QGCDashBoard;
}

class QGCDashBoard : public QWidget
{
    Q_OBJECT

public:
    explicit QGCDashBoard(QWidget *parent = nullptr);
    ~QGCDashBoard();

    static QGCDashBoard* instance(void);

    /// @brief Creates the MainWindow singleton. Should only be called once by QGCApplication.
    static QGCDashBoard* _create(QWidget *parent);

    /// @brief Deletes the MainWindow singleton
    void deleteInstance(void);

private:
    Ui::QGCDashBoard *ui;
    QTimer *DashBoardStart;
    AutoResize *m_autoResizeHandler;
    void resizeEvent(QResizeEvent * event);

    Vehicle* _vehicle;
public:
    float roll;
    float pitch;

public slots:
    void DashTimeStart();
};
