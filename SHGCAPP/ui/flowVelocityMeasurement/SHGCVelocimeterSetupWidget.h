#ifndef SHGCVELOCIMETERSETUPWIDGET_H
#define SHGCVELOCIMETERSETUPWIDGET_H

#include <QWidget>
#include <AutoResize.h>

namespace Ui {
class SHGCVelocimeterSetUpWidget;
}

class SHGCVelocimeterSetUpWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SHGCVelocimeterSetUpWidget(QWidget *parent = nullptr);
    ~SHGCVelocimeterSetUpWidget();

    static SHGCVelocimeterSetUpWidget* instance(void);

    /// @brief Deletes the VelocimeterSetUp singleton
    void deleteInstance(void);

    /// @brief Creates the VelocimeterSetUp singleton. Should only be called once by SHGCApplication.
    static SHGCVelocimeterSetUpWidget* _create(QWidget *parent);

    bool state_2Or3 = false;

//    void preservingHistoricalData();
    void loadHistoricalData();

private slots:
    void on_return_pushButton_clicked();

    void on_close_pushButton_clicked();

    void on_reduce_pushButton_clicked();

    void on_plus_pushButton_clicked();

    void on_application_pushButton_clicked();
    void close_qgcapplicationTipsWidget();

//    void TextInitializationOK(bool);//更改提示文字完成

    void preservingHistoricalData();

    void saveFV_ValueStates();

signals:
    void return_pushButton_isClicked();
    void close_pushButton_isClicked();

    void application_pushButton_isClicked_1(double,double,double);
    void application_pushButton_isClicked_2(double,double,double);
    void application_pushButton_isClicked_3(double,double,double);

    void changeText_Ok();//更改提示框文字状态为 "应用成功"
    void changeText_Error();//更改提示框文字状态为 "参数违规，请重新设置"
    void changeText_Unknown();//更改提示框文字状态为 "首次应用，若当前未进行首次测量...\n设置无效时，请流速有值后再试！"

private:
    Ui::SHGCVelocimeterSetUpWidget *ui;

    QTimer *qtimer;
    void initqTimer();

    bool textOK = false;

    AutoResize *m_autoResizeHandler;
    void resizeEvent(QResizeEvent *event);

    bool getTheRightValue_formFV = false;
};

#endif // SHGCVELOCIMETERSETUPWIDGET_H
