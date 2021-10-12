#ifndef POINT_EDIT_WIDGET_H
#define POINT_EDIT_WIDGET_H

#include <QWidget>
#include <QDialog>
#include <QMouseEvent>
#include <QTableView>
//#include <QSqlQuery>
//#include <QSqlTableModel>
//#include <QSqlRecord>
#include "pushbuttonitemdelegate.h"
#include <QModelIndex>
#include <QMutex>
#include <QUndoStack>
#include <QAbstractTableModel>
#include <QItemDelegate>
#include "qmlobjecttablemodel.h"
#include <QGeoCoordinate>

namespace Ui {
class point_Edit_Widget;
}

class point_Edit_Widget : public QDialog
{
    Q_OBJECT

public:
    explicit point_Edit_Widget(QWidget *parent = nullptr);
    ~point_Edit_Widget();

    static bool pressStatu;

    static point_Edit_Widget* instance(void);

    /// @brief Deletes the MainWindow singleton
    void deleteInstance(void);

    /// @brief Creates the MainWindow singleton. Should only be called once by QGCApplication.
    static point_Edit_Widget* _create(QWidget *parent);

//    void upDataSql();
    void tableViewInit();
    void initTableView();
    void initPointEditing_widget_LineEdit();
    void initReturnPoint_widget_LineEdit();
    void initGlobalSettings_widget_LineEdit();

    void initSwitchButton();
//    QPushButton *btn;
    QmlObjectTableModel* pModel;

private slots:


    void on_btn_Drag_widget_pressed();
    void on_btn_Drag_widget_released();
    void on_btn_Edit_clicked();
    void on_btn_setting_clicked();
    void on_btn_Back_point_clicked();
    void on_btn_onePoint_Edit_clicked();
    void on_btn_allPoint_Edit_clicked();
//    void onTableClicked(const QModelIndex &index);
                            //
    void _insertRows        (QModelIndex);
    void _removeRows        (QModelIndex);
    void _upRows            (QModelIndex);
    void _downRows          (QModelIndex);
//    void rowMoved(int,int,int);

//    void OnAddPointClicked(QModelIndex);
//    void OnDeletePointClicked(QModelIndex);
//    void OnUpPointClicked(QModelIndex);
//    void OnDownPointClicked(QModelIndex);
//    void itemDataChanged(QModelIndex,QModelIndex);
//    void showOperationDetails(const QModelIndex &, const QModelIndex &);
//    void changeAction(QModelIndex,int);
//    void clearPointEditing_widget_LineEdit(QVariant,QVariant);
//    void on_setTakeoffPoint_pushButton_RP_clicked();
//    void on_setMyPosition_pushButton_RP_clicked();
//    void on_manualSettings_pushButton_RP_clicked();
//    void on_US_serialNumber_pushButton_SPE_clicked();
//    void on_DS_serialNumber_pushButton_SPE_clicked();
//    void on_MS_takeoffAltitude_pushButton_SPE_clicked();
//    void on_PS_takeoffAltitude_pushButton_SPE_clicked();
//    void on_MS_relativeDistance_pushButton_SPE_clicked();
//    void on_PS_relativeDistance_pushButton_SPE_clicked();
//    void on_MS_absoluteAngle_pushButton_SPE_clicked();
//    void on_PS_absoluteAngle_pushButton_SPE_clicked();
//    void on_MS_horizontalVelocity_pushButton_SPE_clicked();
//    void on_PS_horizontalVelocity_pushButton_SPE_clicked();
//    void on_MS_verticalVelocity__pushButton_SPE_clicked();
//    void on_PS_verticalVelocity__pushButton_SPE_clicked();
//    void on_MS_slope_pushButton_SPE_clicked();
//    void on_PS_slope_pushButton_SPE_clicked();
//    void on_MS_slopeAngle_pushButton_SPE_clicked();
//    void on_PS_slopeAngle_pushButton_SPE_clicked();
//    void on_coordinatedTurn_pushButton_SPE_clicked();
//    void on_rollTurn_pushButton_SPE_clicked();
//    void on_stopTurning_pushButton_SPE_clicked();
//    void on_MS_residenceTime_pushButton_SPE_clicked();
//    void on_PS_residenceTime_pushButton_SPE_clicked();
//    void on_MS_takeoffAltitude_pushButton_GS_clicked();
//    void on_PS_takeoffAltitude_pushButton_GS_clicked();
//    void on_MS_relativeDistance_pushButton_GS_clicked();
//    void on_PS_relativeDistance_pushButton_GS_clicked();
//    void on_MS_horizontalVelocity_pushButton_GS_clicked();
//    void on_PS_horizontalVelocity_pushButton_Gs_clicked();
//    void on_MS_verticalVelocity__pushButton_GS_clicked();
//    void on_PS_verticalVelocity__pushButton_GS_clicked();
//    void on_coordinatedTurn_pushButton_GS_clicked();
//    void on_rollTurn_pushButton_GS_clicked();
//    void on_stopTurning_pushButton_GS_clicked();
//    void on_MS_residenceTime_pushButton_GS_clicked();
//    void on_PS_residenceTime_pushButton_GS_clicked();
//    void on_endOfMission_turnBack_pushButton_GS_clicked();
//    void on_endOfMission_hover_pushButton_GS_clicked();
//    void on_endOfMission_cycle_pushButton_GS_clicked();
//    void on_MS_theCycleNumberpushButton_GS_clicked();
//    void on_PS_theCycleNumber_pushButton_GS_clicked();
//    void on_endOfCycle_turnBack_pushButton_GS_clicked();
//    void on_endOfCycle_hover_pushButton_GS_clicked();
//    void on_angleExplain_pushButton_GS_clicked();
//    void on_angleSelection_pushButton_GS_clicked();
//    void on_spread_pushButton_GS_clicked();
//    void recvHomeData(QString,QString);
//    void getWaypointDataDB_setLineEdit(QVariant);
//    void on_serialNumber_lineEdit_SPE_textChanged(const QString &arg1);
//    void on_longitude_lineEdit_SPE_textChanged(const QString &arg1);
//    void on_latitude_lineEdit_SPE_textChanged(const QString &arg1);
//    void on_takeoffAltitude_lineEdit_SPE_textChanged(const QString &arg1);
//    void on_relativeDistance_lineEdit_SPE_textChanged(const QString &arg1);
//    void on_absoluteAngle_lineEdit_SPE_textChanged(const QString &arg1);
//    void on_horizontalVelocity_lineEdit_SPE_textChanged(const QString &arg1);
//    void on_verticalVelocity_lineEdit_SPE_textChanged(const QString &arg1);
//    void on_slope_lineEdit_SPE_textChanged(const QString &arg1);
//    void on_slopeAngle_lineEdit_SPE_textChanged(const QString &arg1);
//    void on_noseOrientation_lineEdit_SPE_textChanged(const QString &arg1);
//    void on_residenceTime_lineEdit_SPE_textChanged(const QString &arg1);

private:

    Ui::point_Edit_Widget *ui;

    bool event              (QEvent *event);
    void mouseMoveEvent     (QMouseEvent *event);
    int start_x;
    int w_desktop;
    int h_desktop;
    bool drag_widget    =   false;

//    bool angleExplain_pushButton_GS_clicked = false;
//    bool endOfMission_cycle_pushButton_GS = false;


//    bool seq_no_LineEdit_isChanged = false;//单点编辑 序号seq_no是否更改 true:已改变  false:未改变
//    QString SPE_waypointID;//单点编辑，航点ID
//    QString SPE_seq_no_changed;//保存单点编辑 更改后的seq_no


//    QVariant waypointID;


signals:

    void addPointItem       (double lat,double lng,int row);
    void removePointItem    (int row);
    void upPointItem        (int row);
    void downPointItem      (int row);
//    void deletePoint(QVariant);

//    void addPoint(QVariant,QVariant,QVariant);

//    void upPointData(QVariant,QVariant,QVariant);

//    void upPoint(QVariant);

//    void downPoint(QVariant);

//    //设置返航点
//    void rpToStart();
//    void rpToCurrent();
//    void rpToSelf();

//    void serialNumber_SPE_textChanged(QString);
//    void longitude_SPE_textChanged(QString);
//    void latitude_SPE_textChanged(QString);
//    void takeoffAltitude_SPE_textChanged(QString);
//    void relativeDistance_SPE_textChanged(QString);
//    void absoluteAngle_SPE_textChanged(QString);
//    void horizontalVelocity_SPE_textChanged(QString);
//    void verticalVelocity_SPE_textChanged(QString);
//    void slope_SPE_textChanged(QString);
//    void slopeAngle_SPE_textChanged(QString);
//    void noseOrientation_SPE_textChanged(QString);
//    void residenceTime_SPE_textChanged(QString);
};

#endif // POINT_EDIT_WIDGET_H
