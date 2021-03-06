#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSettings>
#include <QList>
#include <QtQuickWidgets/QQuickWidget>

#include "QGCApplication.h"
#include "LinkManager.h"
#include "LinkInterface.h"
#include "QGCMAVLinkLogPlayer.h"

#include "Vehicle.h"
#include "QGCDashBoard.h"
#include "leftMainWindow.h"
#include "SHGCMainSetup.h"
#include "SHGCUpLoadWidget.h"
#include "point_edit_widget.h"
#include "TCPLink.h"
#include "UDPLink.h"
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    Q_PROPERTY(bool                 flightmonitoring         READ flightmonitoring                                        NOTIFY flightmonitoringChanged)
    Q_PROPERTY(bool                 RoutePlan                READ RoutePlan                                        NOTIFY RoutePlanChanged)

    enum linkTypeToUAS{
        LinkTypeSerial,
        LinkTypeUdp,
        LinkTypeTcp
    };
    Q_ENUMS(LinkType)


    bool            flightmonitoring        () { return _flightmonitoring;}
    bool            RoutePlan               () { return _RoutePlan;}
    static MainWindow* instance(void);


    /// @brief Deletes the MainWindow singleton
    void deleteInstance(void);

    /// @brief Creates the MainWindow singleton. Should only be called once by QGCApplication.
    static MainWindow* _create();

    /// @return Root qml object of main window QML
    QObject* rootQmlObject(void);


    // Called from MainWindow.qml when the user accepts the window close dialog
    void _reallyClose(void);


protected:
    void connectCommonActions();
    QSettings settings;

    void loadSettings();
    void storeSettings();
//    QGCMAVLinkLogPlayer* logPlayer;

public:
//    QGCMAVLinkLogPlayer* getLogPlayer()
//    {
////        return logPlayer;
//    }

private:
    Ui::MainWindow *ui;
    virtual  void resizeEvent(QResizeEvent *event) override;
    QObject *item;
    void _buildCommonWidgets(void);

    bool                    _lowPowerMode;           ///< If enabled, QGC reduces the update rates of all widgets
    bool                    _showStatusBar;

    QString _getWindowGeometryKey();
    bool    _forceClose;

    int w;
    int h;
    QSize scrSize;
    leftMainWindow*         leftWindow       =      nullptr;
//    point_Edit_Widget*      pointEdit        =      nullptr;


    void _saveMainWindowSettings();
    void creatMapWindow();

    //创建Action
    void actionFlightMonit();
    void actionRoutePlan();
    void actionAdjust();
    void actionSetUp();

    bool _flightmonitoring = true;
    bool _RoutePlan = false;
    int  _linkVehicle;

    SHGCUpLoadWidget* _uploadWidget;

    void FV_WLGConnected();//流测仪_水位计 连接

    int device_Num = 0;
    void saveDeviceID();

    QString g_port = "";
    QString g_baudRate = "";

    QString _ip;
    QString _port;
    linkTypeToUAS _linkType;

    SerialLink* _seriallink;
    TCPLink* _tcpLink;
    UDPLink* _udpLink;

    int initialization_isMainWindow_width;
    int initialization_isMainWindow_height;
    int resize_isMainWindow_width;
    int resize_isMainWindow_height;

public:
    float resize_Coefficient_width;
    float resize_Coefficient_height;


signals:
//    void connectUAVSerial(const QString portName, int Baud);

    void initStatusChanged(const QString& message, int alignment, const QColor &color);

    void reallyClose(void);

    void flightmonitoringChanged();

    void RoutePlanChanged();

    void showFlightMonitRectange();

    void showRoutePlanRectange();

    void showActionAdjust();

    void connectFV_serialPort();

    void connectWLG_serialPort();

    void qtimerStop();

    void changefv_WLGQmlBool();

    void single_send_Instructions_FV();

    void single_send_Instructions_WLG();

    void FV_pushButtonUnableToClick();

    void FV_pushButtonClickable();

    void WLG_pushButtonUnableToClick();

    void WLG_pushButtonClickable();

    void WLG_receiveMeasureStates(bool);

    void FV_receiveMeasureStates(bool);

    void showTipsWidget(bool);

    void receiveMeasureStates_fromWLG(bool);

    void transferMeasurementResults_formFV(QString);

    void transferMeasurementResults_formWLG(QString);

    void interval_Measurement_changeTime(QString);

    void single_Measurement();

    void openVMDataComSuccess();

    void openVMDataComFail();

    void getTheLatestValue(bool);

    void openWLGDataComSuccess();

    void openWLGDataComFail();

    void WLG_getTheLatestValue(bool);

    void setCalibrationCoefficient_1(double,double,double);

    void setCalibrationCoefficient_2(double,double,double);

    void setCalibrationCoefficient_3(double,double,double);

    void changeText_isUnknown();

    void changeText_isOk();

    void changeText_isError();

    void SHGCVelocimeterInteractiveWidget_isShow();

    void getSettingParameters();

    void recoveryFlowVelocimeterCanBeMeasured();
    void recoveryWaterLevelGaugeCanBeMeasured();

    void suspendMeasurement_WLG();

    void transmitDataReceiveStatus_formFV();

//    void setAircraftCenter_fromVelocimeter(bool);

public slots:
//    void connectUAVSerial();

    /** @brief Update the window name */
    void configureWindowName();

    void _vehicleAdded(Vehicle* vehicle);

    void _vehicleRemoved(Vehicle* vehicle);

    void _showLeftWidget();

    void createMyAction(QString );

    void quitSetup();

    void openLogReplay();

    void showUpload();

    void showVelocimeterInteractiveWidget();
    void closeVelocimeterInteractiveWidget();
    void showVelocimeterSetUpWidget();

    void setAircraftcomPort(QString port);
    void setAircraftcomRate(QString rate);
    void connectUAVSerialLink(bool str);
    void Point_Edit_Widget(QString seq_no);

    void linkTcp          (QString,QString);
    void linkUdp          (QString,QString);
    void linkSerial       (QString,QString);

};

#endif // MAINWINDOW_H
