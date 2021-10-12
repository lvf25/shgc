#ifndef SHGCVELOCIMETERINTERACTIVEWIDGET_H
#define SHGCVELOCIMETERINTERACTIVEWIDGET_H

#include <QWidget>
#include <QMouseEvent>
#include <QTableView>
#include <QSqlQuery>
#include <QSqlTableModel>
#include <QSqlRecord>
#include <QModelIndex>
#include <QMutex>

#include <QJsonArray>

#include <QMessageBox>
#include <QTableWidgetItem>
#include <QTcpSocket>

#include "MainWindow.h"

namespace Ui {
class SHGCVelocimeterInteractiveWidget;
}

class SHGCVelocimeterInteractiveWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SHGCVelocimeterInteractiveWidget(QWidget *parent = nullptr);
    ~SHGCVelocimeterInteractiveWidget();

    static SHGCVelocimeterInteractiveWidget* instance(void);

    /// @brief Deletes the VelocimeterInteractive singleton
    void deleteInstance(void);

    /// @brief Creates the VelocimeterInteractive singleton. Should only be called once by SHGCApplication.
    static SHGCVelocimeterInteractiveWidget* _create(QWidget *parent);

private slots:

    void changeBackground_red();
    void changeBackground_blue();

    void on_connectFV_pushButton_clicked();

    void on_checkBox_stateChanged(int arg1);

    void on_setUp_pushButton_clicked();

    void on_confirmTheChange_pushButton_clicked();

    void handleFVJSONFile(bool);
    void analysisFVJSONFile(bool);
    void setFVTheLatestValue_forTableWidget_realTime(bool);
//    void setFVTheLatestValue_forTableWidget(bool);

    void clearFVJSONFile(bool);

    void on_connectWLG_pushButton_clicked();

    void changeWLGBackground_blue();
    void changeWLGBackground_red();

    void handleWLGJSONFile(bool);
    void analysisWLGJSONFile(bool);
    void setWLGTheLatestValue_forLable_realTime(bool);
    void clearWLGJSONFile(bool);

    //更改测速仪测量按钮状态
    void setFV_PushButtonUnableToClick();
    void setFV_pushButtonClickable();
    //更改水位计测量按钮状态
    void setWLG_pushButtonUnableToClick();
    void setWLG_pushButtonClickable();

    //    void whetherToPopUpPrompts(bool);//是否需要弹出提示
    //    void close_qgcapplicationTipsWidget();//关闭提示
    //    void TextInitializationOK(bool);//更改提示文字完成

    void changeIp(QString);
    void changePort(QString);
    void slotconnectedsuccess();

    void tcpClientConnecting();

    void getDeviceNum(QString);

    void setFV_WLG_DebugMode(bool);

    void on_importFile_clicked();

    void on_sendData_clicked();

    void regularlySendData();

    void acceptMaxDeviationValue(QString);

signals:
    void connectFV_pushButton_isClicked_conn();
    void connectFV_pushButton_isClicked_send();
    void checkBoxIsChecked();
    void checkBoxIsUnchecked();
    void setUp_pushButton_isClicked();

    void intervalMeasurement_changeTime(QString);

    void connectWLG_pushButton_isClicked_conn();
    void connectWLG_pushButton_isClicked_send();

    void sendSignal_setAircraftCenter(bool);

private:
    Ui::SHGCVelocimeterInteractiveWidget *ui;


    MainWindow *_MainWindow = MainWindow::instance();
    SHGCMainSetup *_SHGCMainSetup = SHGCMainSetup::instance();

    AutoResize *m_autoResizeHandler;

    void initTableWidget_FV();
    void initLable_WLG();
    void initTableWidget_realTime();

    void initDisplayHistoricalData();
//    void getDisplayHistoricalData_forTableWidge();
//    void setDisplayHistoricalData_forTableWidge();
    void setDisplayHistoricalData(QString);
    //    void sqlInit();
    //    void upDataSql();

    bool initTableWidgeStatus = false;
    int historicalData_JsonArray_FV_Size = 0;

    QSqlTableModel *pModel;
    QSqlDatabase db;

    bool FV_connectOK = false;

    bool WLG_connectOK = false;

    //    QString strTemp = "";
    QString strSec = "";

    QString id_FV = "";
    QString time_FV = "";
    QString date_FV = "";
    QString org_flowvely_FV = "";
    QString calcoe_FV = "";
    QString cal_flowvely_FV = "";
    QString var_lng_FV = "";
    QString var_lat_FV = "";
    QString ave_flowvely_FV = "";
    QString SNR_FV = "";
    QString WLG_Height = "";
    QString UAV_Height = "";

    QJsonArray jsonArray_FV;

    QString id_WLG = "";
    QString time_WLG = "";
    QString date_WLG = "";
    QString waterLevel_WLG = "";
    QString var_lng_WLG = "";
    QString var_lat_WLG = "";
    QString var_alt_WLG = "";

    QString device_Num = "";

    void getDeviceNum();

    QJsonArray jsonArray_WLG;

    int i = 0;
    int j = 0;

    QMessageBox *messageBox;

    bool ret = false;
    QTimer *qtimer;
    void initqTimer();

    bool tipsStates = false;
    bool textOK = false;

    void tcpClientDataPackingAndSending();
//    void tcpClientDataPackingAndSending_FV();
//    void tcpClientDataPackingAndSending_WLG();
    void tcpClientDisConnecting();

    //创建一个通信套接字，用来和服务器进行通信
    QTcpSocket *tcpsocket = new QTcpSocket;
    QString sendData = "";
    QString g_tcpIP = "";
    QString g_tcpPort = "";

    bool _slotconnectedsuccess = false;

    QString fileName = "";

    int size = 0;

    double org_flowvely_temp = 0.0;

    double d_FV_WLG_MaxDeviationValue = 0.30;

    void loadSettingParameters();

    QString id_FV_History = "";
    QString time_FV_History = "";
    QString date_FV_History = "";
    QString org_flowvely_FV_History = "";
    QString calcoe_FV_History = "";
    QString cal_flowvely_FV_History = "";
    QString var_lng_FV_History = "";
    QString var_lat_FV_History = "";
    QString ave_flowvely_FV_History = "";
    QString SNR_FV_History = "";
    QString WLG_Height_History = "";
    QString UAV_Height_History ="";

    QString id_FV_regularly = "";
    QString time_FV_regularly = "";
    QString date_FV_regularly = "";
    QString org_flowvely_FV_regularly = "";
    QString calcoe_FV_regularly = "";
    QString cal_flowvely_FV_regularly = "";
    QString var_lng_FV_regularly = "";
    QString var_lat_FV_regularly = "";
    QString ave_flowvely_FV_regularly = "";
    QString SNR_FV_regularly = "";
    QString WLG_Height_regularly = "";
    QString UAV_Height_regularly = "";

    bool regularlySendStates = false;

    int returnValueIs_firstStates = 0;//判断是否是程序启动后的前两次读取数据 用于将前两次保存数据的上一次的流速值(org_flowvely_temp)设为 0.00

    bool SendDataInProgress = false;

public:
    static bool checkBoxCHE;

    /*
    QString HEXsdStr="";
    QString HEXrdStr="";

    void showRawData();
    void showParsedData();
    QString changefromHex_to_ascii(QString str);
    */
};

#endif // SHGCVELOCIMETERINTERACTIVEWIDGET_H
