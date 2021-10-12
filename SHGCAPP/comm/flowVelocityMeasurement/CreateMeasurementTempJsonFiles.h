#ifndef CREATEMEASUREMENTTEMPJSONFILES_H
#define CREATEMEASUREMENTTEMPJSONFILES_H

#include <QObject>
#include <QDebug>
#include <QThread>
#include <QtSerialPort/QSerialPort>

#include "MainWindow.h"

class CreateMeasurementTempJsonFiles : public QObject
{
    Q_OBJECT
public:
    explicit CreateMeasurementTempJsonFiles(QObject *parent = nullptr);
    ~CreateMeasurementTempJsonFiles();

    static CreateMeasurementTempJsonFiles* instance(void);
    void deleteInstance(void);
    static CreateMeasurementTempJsonFiles* _create();

    //字节数组转16进制字符串
    static QString ByteArrayToHexStr(QByteArray data)
    {
        QString temp="";
        QString hex=data.toHex();
        for (int i=0;i<hex.length();i=i+2)
        {
            temp+=hex.mid(i,2)+" ";
        }
        return temp.trimmed().toUpper();
    }


    QString OX(int n)
    {
        char s[8] = { 0 };
        QString strTemp ="";
        QString str ="";

        int a, i = 0, k;
        do
        {
            a = n & 15;//取后四位的二进制位
            n >>= 4;//n右移四位
            n &= 0xffffffff;
            s[i] = (a <= 9) ? a + 48 : a + 55;
            i++;
        } while (n);

        for (k = i - 1; k >= 0; k--)
        {
            strTemp = strTemp + s[k];

            if(i == 1)
            {
                str = '0' + strTemp;
            }
        }

        return str;
    }
public:
    int i = 0;
    int j = 0;

    QString HEXrdStr = "";

    //获取主窗口,接收其传过来的信号
    MainWindow *_MainWindow = MainWindow::instance();

    QString flowspeedOfTheAverageValue = "";
    double d_flowspeedOfTheAverageValue = 0.0;
    QString flowspeedOfTheOriginalValue = "";
    double d_flowspeedOfTheOriginalValue = 0.0;
    QString SNRValue = "";
    double d_SNRValue = 0.0;

    QString var_lng = "";
    QString var_lat = "";
    QString var_alt = "";

    double calibrationCoefficient = 1.0;

    QByteArray alone_waterLevelGaugeBuf;//测量结果
    QString str_alone_waterLevelGaugeBuf = "";//测量结果
    QString str_waterLevelValue = "";//解析结果
    double double_waterLevelValue = 0.0;//解析结果

    double d_flowspeedOfTheOriginalValue_temp = 0.0;

signals:
    void receivetheMessage(bool);
    void WLG_receivetheMessage(bool);
public slots:
    //获取经纬度
    void getLatData();
    void getLonData();
    //获取高度值
    void getAltData();

    void getCalibrationCoefficient_1(double,double,double);
    void getCalibrationCoefficient_2(double,double,double);
    void getCalibrationCoefficient_3(double,double,double);

    void FV_handle_queryResults(QString);//处理查询结果
    void WLG_handle_queryResults(QString);

private:
    void generateJSONFile();//生成JSON文件 数据临时保存在 flowVelocity_temp.json 文件
    void generateWLGJSONFile();


};

#endif // CREATEMEASUREMENTTEMPJSONFILES_H
