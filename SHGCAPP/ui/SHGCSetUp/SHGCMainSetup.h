#pragma execution_character_set("utf-8")

#ifndef QGCMAINSETUP_H
#define QGCMAINSETUP_H

#include <QWidget>
#include <QDialog>
#include <QMutex>
#include "clickedWidget.h"
#include "sliderPointer.h"
#include "elecPointer.h"
#include "switchbutton.h"
//#include <MapDatabase.h>

#include "AutoResize.h"


namespace Ui {
class SHGCMainSetup;
}

class SHGCMainSetup : public QDialog
{
    Q_OBJECT
//    Q_PROPERTY(QString map READ map WRITE setmap NOTIFY mapchange)
//    Q_PROPERTY(QString videoAdress READ videoAdress WRITE setVideoAdress NOTIFY videoAdressChanged)

public:
    explicit SHGCMainSetup(QWidget *parent = nullptr);
    ~SHGCMainSetup();

    static SHGCMainSetup* instance(void);

    /// @brief Deletes the MainSetup singleton
    void deleteInstance(void);

    /// @brief Creates the MainSetup singleton. Should only be called once by SHGCApplication.
    static SHGCMainSetup* _create(QWidget *parent);

public:
    static SHGCMainSetup *self;
    void WordList();
    //    QVariantList ProvinceList;
    //    QStringList CityList;
    //    QStringList TownList;
    QStringList provincelist;
    QStringList citylist;
    QStringList townlist;
//    QStringList m;

    AutoResize *m_autoResizeHandler;
    void resizeEvent(QResizeEvent * event);


private slots:

    void initSwitchButton();

    void on_USB_1_click();

    void on_TCP_1_click();

    void on_UDP_Serve_1_click();

    void on_bluetooth_1_click();

    void on_set_return_1_clicked();

    void on_set_return_up_to_setting_clicked();

//    void on_cW_USB_click();

//    void on_cW_TCP_click();

//    void on_cW_UDP_click();

//    void on_cW_bluetooth_click();

//    void on_data_conn_log10_click();

    void on_btn_USBInput_1_clicked();

    void on_btn_TCPInput_1_clicked();

    void on_btn_UDPInput_1_clicked();

//    void on_btn_USBInput_2_clicked();

//    void on_btn_TCPInput_2_clicked();

//    void on_btn_UDPInput_2_clicked();

    void updateInfo(const QString&);

    void on_btn_saveVideoPath_clicked();

    void on_btn_saveMissonPath_clicked();

    void on_return_height_sub_clicked();

    void on_return_height_add_clicked();

    void on_data_conn_log_15_click();

    void on_data_conn_log_14_click();

//    void on_label_103_linkActivated(const QString &link);

//    void on_data_conn_log_11_click();

//    void on_data_conn_log_9_click();

//    void on_data_conn_log_10_click();

//    void on_label_91_linkActivated(const QString &link);

//    void on_UDP_Serve_2_click();

//    void on_choose_tcp_1_linkActivated(const QString &link);

//    void on_tts_widget_click();

//    void on_data_conn_log_9_click();

    void on_widget_79_click();

//    void on_set_return_8_clicked();

//    void on_set_return_9_clicked();

    void on_btn_Download_1_clicked();

    void on_btn_Citylist_1_clicked();

    void on_btn_Download_2_clicked();

    void on_btn_Citylist_2_clicked();

    void on_btn_download_clicked();

    void on_btn_downing_clicked();

    void on_pSearchButton_clicked();

//    void on_set_return_6_clicked();

//    void on_set_return_7_clicked();

//    void on_set_return_3_clicked();


    void on_m_pSearchLineEdit_textEdited(const QString &arg1);

    void on_btn_return_back_2_clicked();

    void on_btn_go_on_2_clicked();

    void on_data_conn_log_FV_WLG_click();

    void on_set_return_6_clicked();

    void on_btn_broadcast_tip_2_clicked();

    void on_btn_elec_tip_2_clicked();

    void sendSettingParameters();
    void on_tcpLink_clicked();

    void on_UDPLink_clicked();

    void on_serialLink_clicked();

private:

    Ui::SHGCMainSetup *ui;

    void init_setup();

    static bool btn_downloaded_clicked;
    static bool btn_downloading_clicked;
    static bool btn_download_clicked;
    static bool btn_downing_clicked;
    static bool cW_TCP_3;

    void GetDiskSpace(QString, double&, double&);

    QTimer *m_timer;
//    QLineEdit *saveMap;
//    QLineEdit *saveVideoPlane;
//    QLineEdit *saveVideoLocal;
    double dTotalBytes, dFreeBytes;


    QString map_saved = "12";
    QString p_video_saved = "12";
    QString l_video_saved = "12";

//    void AutomaticAdditionSerialPort();

    QString UAV_SerialPort = "";
    QString UAV_BaudRate = "";
    QString UAV_TCP_IP = "";
    QString UAV_TCP_Port = "";
    QString UAV_UDP_IP = "";
    QString UAV_UDP_Port = "";
    QString UAV_Number = "1";

    QString VM_SerialPort = "";
    QString VM_BaudRate = "";
    QString WLG_SerialPort = "";
    QString WLG_BaudRate = "";
    QString FV_WLG_TCP_IP = "";
    QString FV_WLG_TCP_Port = "";
    QString str_FV_WLG_DebugMode = "";

    bool bool_FV_WLG_DebugMode = false;

    void preservingSettingParameters();
    void loadSettingParameters();

    QString str_FV_WLG_MaxDeviationValue = "0.30";

signals:
    void closeSetup();

    void sendToVMcomPort(QString);

    void sendToVMcomRate(QString);

    void sendToWLGcomPort(QString);

    void sendToWLGcomRate(QString);

    void sendToVM_WLGIP(QString);

    void sendToVM_WLGPort(QString);

    void sendToAircraftcomPort(QString);

    void sendToAircraftcomRate(QString);

    void sendToAircraftNumber(QString);

    void sendToFV_WLG_DebugMode(bool);

    void sendMaxDeviationValue(QString);

    void LinkTcp(QString ip,QString port);
    void linkUdp(QString ip,QString port);

    void linkSerial(QString,QString);
};

#endif // QGCMAINSETUP_H
