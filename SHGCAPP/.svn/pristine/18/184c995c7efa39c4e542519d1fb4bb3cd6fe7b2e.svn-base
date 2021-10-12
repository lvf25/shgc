#include "CreateMeasurementTempJsonFiles.h"

#include <QChar>
#include <QString>
#include <QDir>
#include <QTime>
#include <QCoreApplication>
#include <algorithm>
#include <cstring>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>

#include "Vehicle.h"

static CreateMeasurementTempJsonFiles* _instance = nullptr;

CreateMeasurementTempJsonFiles* CreateMeasurementTempJsonFiles::_create()
{
    _instance = new CreateMeasurementTempJsonFiles();
    return _instance;
}

CreateMeasurementTempJsonFiles* CreateMeasurementTempJsonFiles::instance(void)
{
    return _instance;
}

void CreateMeasurementTempJsonFiles::deleteInstance(void)
{
    delete this;
}

CreateMeasurementTempJsonFiles::CreateMeasurementTempJsonFiles(QObject *parent)
    : QObject(parent)
{
    //获取经纬度数据
//    getLatData();
//    getLonData();
    //获取无人机高度值
//    getAltData();

    //"设置校准参数"界面 "应用"按钮被按下且生效 则设置校准系数
    connect(_MainWindow,&MainWindow::setCalibrationCoefficient_1,this,&CreateMeasurementTempJsonFiles::getCalibrationCoefficient_1);
    connect(_MainWindow,&MainWindow::setCalibrationCoefficient_2,this,&CreateMeasurementTempJsonFiles::getCalibrationCoefficient_2);
    connect(_MainWindow,&MainWindow::setCalibrationCoefficient_3,this,&CreateMeasurementTempJsonFiles::getCalibrationCoefficient_3);

    //若测速仪接收到返回数据 则将数据传递过去解析
    connect(_MainWindow,&MainWindow::transferMeasurementResults_formFV,this,&CreateMeasurementTempJsonFiles::FV_handle_queryResults);
    //若水位计接收到返回数据 则将数据传递过去解析
    connect(_MainWindow,&MainWindow::transferMeasurementResults_formWLG,this,&CreateMeasurementTempJsonFiles::WLG_handle_queryResults);

}

CreateMeasurementTempJsonFiles::~CreateMeasurementTempJsonFiles()
{
    _instance = nullptr;
}

//当前飞机的纬度
void CreateMeasurementTempJsonFiles::getLatData()
{
    Vehicle* _vehicle = qgcApp()->toolbox()->multiVehicleManager()->activeVehicle();
    if(_vehicle)
    {
        VehicleGPSFactGroup *gps = (VehicleGPSFactGroup*)(_vehicle->gpsFactGroup());
        var_lat = gps->lat()->rawValueString();
    }

    qDebug()<<"-------------------------------var_lat:"<<var_lat;
}

//当前飞机的经度
void CreateMeasurementTempJsonFiles::getLonData()
{
    Vehicle* _vehicle = qgcApp()->toolbox()->multiVehicleManager()->activeVehicle();
    if(_vehicle)
    {
        VehicleGPSFactGroup *gps = (VehicleGPSFactGroup*)(_vehicle->gpsFactGroup());
        var_lng = gps->lon()->rawValueString();
    }

    qDebug()<<"-------------------------------var_lng:"<<var_lng;


}


/*====================================雷达测速仪======================================*/

//处理查询结果
void CreateMeasurementTempJsonFiles::FV_handle_queryResults(QString s)
{
    HEXrdStr = s;
    if(HEXrdStr.mid(0,5) == "01 03" && HEXrdStr.length() >= 32)
    {
        QString str ="";

        //截取字符串
        QString average_flowVelocitySTR = HEXrdStr.mid(9,5);
        QString realTime_flowVelocitySTR = HEXrdStr.mid(15,5);
        QString SNR_flowVelocitySTR = HEXrdStr.mid(21,5);

        //去空格
        average_flowVelocitySTR = average_flowVelocitySTR.mid(0,2) + average_flowVelocitySTR.mid(3,2);
        realTime_flowVelocitySTR = realTime_flowVelocitySTR.mid(0,2) + realTime_flowVelocitySTR.mid(3,2);
        SNR_flowVelocitySTR = SNR_flowVelocitySTR.mid(0,2) + SNR_flowVelocitySTR.mid(3,2);

        //十六进制字符串转十进制
        bool ok;
        int average_flowVelocityINT = average_flowVelocitySTR.toInt(&ok,16);
        int realTime_flowVelocityINT = realTime_flowVelocitySTR.toInt(&ok,16);
        int SNR_flowVelocityINT = SNR_flowVelocitySTR.toInt(&ok,16);

        //十进制数值转为小数点后两位流速值
        double average_flowVelocityDOB = (double)(average_flowVelocityINT/100.0);
        double realTime_flowVelocityDOB = (double)(realTime_flowVelocityINT/100.0);
        double SNR_flowVelocityDOB = (double)(SNR_flowVelocityINT/100.0);

        //记录字符串到字符串列表
        QStringList strL;
        strL << "流速信息 ==> " << "平均数据:" << QString::number(average_flowVelocityDOB,10,2) << " 实时数据:"
             << QString::number(realTime_flowVelocityDOB,10,2) << " 信噪比:" << QString::number(SNR_flowVelocityDOB,10,2)
             << "\n";

        str = strL.join("");

        qDebug()<<"\"查询测量\"模式下 单次 流速信息："<<str;

        flowspeedOfTheAverageValue = QString::number(average_flowVelocityDOB,10,2);
        d_flowspeedOfTheAverageValue = flowspeedOfTheAverageValue.toDouble();

        flowspeedOfTheOriginalValue = QString::number(realTime_flowVelocityDOB,10,2);
        d_flowspeedOfTheOriginalValue = flowspeedOfTheOriginalValue.toDouble();

        SNRValue = QString::number(SNR_flowVelocityDOB,10,2);
        d_SNRValue = SNRValue.toDouble();
    }
    else
    {
//        d_flowspeedOfTheAverageValue = 0.0;
//        d_flowspeedOfTheOriginalValue = 0.0;
//        d_SNRValue = 0.0;
        return;
    }



    generateJSONFile();
}


//生成JSON文件 数据临时保存在 flowVelocity_temp.json 文件
void CreateMeasurementTempJsonFiles::generateJSONFile()
{
    //获取当前经纬度
    getLonData();
    getLatData();
    //获取无人机高度值
    getAltData();

    //创建保存日志的目录
    bool ok;
    QString str = QCoreApplication::applicationDirPath();
    qDebug()<<"str dir:"<<str;
    QDir dir(str + "/jLogs");
    if(dir.exists())
    {
        ok = true;
        qDebug() << "dir exist!";
    }
    else
    {
        ok = dir.mkdir(str + "/jLogs");//只创建一级子目录，即必须保证上级目录存在
        qDebug() << "dir not exists, generate!";
    }

    // 以读写追加方式打开jLogs目录下的flowVelocity_temp.json文件，若该文件不存在则会自动创建
    QFile file("jLogs/flowVelocity_temp.json");
    if(ok)
    {
        if(!file.open(QIODevice::ReadWrite | QIODevice::Append)) {
            qDebug() << "generate File open error";
        } else {
            qDebug() <<"generate File open!";
        }
    }

    //清空文件中的原有内容
//    file.resize(0);

    // 使用QJsonArray添加值，并写入文件
    QJsonArray jsonArray;

    QJsonObject jsonObject;

    jsonObject.insert("01_id", i++);//id
    jsonObject.insert("02_time", QDateTime::currentDateTime().toString("hh:mm:ss"));//时间
    jsonObject.insert("03_date", QDateTime::currentDateTime().toString("yyyy.MM.dd"));//日期
    jsonObject.insert("04_org_flowvely", d_flowspeedOfTheOriginalValue);//流速原始值
    jsonObject.insert("05_calcoe", calibrationCoefficient);//校准系数
    jsonObject.insert("06_cal_flowvely", d_flowspeedOfTheOriginalValue * calibrationCoefficient);//流速校准值
    jsonObject.insert("07_var_lng", var_lng);//经度
    jsonObject.insert("08_var_lat", var_lat);//纬度
    jsonObject.insert("09_ave_flowvely",d_flowspeedOfTheAverageValue);//流速平均值
    jsonObject.insert("10_SNR",d_SNRValue);//信噪比
    jsonObject.insert("11_WLG_Height",double_waterLevelValue);//水位计测得的高度
    jsonObject.insert("12_UAV_Height", var_alt);//飞机返回的相对起飞点(气压)高度
    jsonArray.append(jsonObject);

    QJsonDocument jsonDoc;
    jsonDoc.setArray(jsonArray);

    file.write(jsonDoc.toJson());

    file.close();

    if(HEXrdStr.length() >= 32)
    {
        emit receivetheMessage(true);
    }
    else
    {
       emit receivetheMessage(false);
    }
}

void CreateMeasurementTempJsonFiles::getCalibrationCoefficient_1(double greaterThan,double lessThan,double coefficient)
{
    if(d_flowspeedOfTheOriginalValue > greaterThan &&
            d_flowspeedOfTheOriginalValue <= lessThan
            )
    {
        calibrationCoefficient = coefficient;
        qDebug()<<"当前校准值 1:"<<coefficient;
    }
    else
    {
        calibrationCoefficient = 1;
    }
}

void CreateMeasurementTempJsonFiles::getCalibrationCoefficient_2(double greaterThan_2,double lessThan_2,double coefficient_2)
{
    if(d_flowspeedOfTheOriginalValue > greaterThan_2 &&
            d_flowspeedOfTheOriginalValue <= lessThan_2
            )
    {
        calibrationCoefficient = coefficient_2;
        qDebug()<<"当前校准值 2:"<<coefficient_2;
    }
    else
    {
        calibrationCoefficient = 1;
    }
}

void CreateMeasurementTempJsonFiles::getCalibrationCoefficient_3(double greaterThan_3,double lessThan_3,double coefficient_3)
{
    if(d_flowspeedOfTheOriginalValue > greaterThan_3 &&
            d_flowspeedOfTheOriginalValue <= lessThan_3
            )
    {
        calibrationCoefficient = coefficient_3;
        qDebug()<<"当前校准值 3:"<<coefficient_3;
    }
    else
    {
        calibrationCoefficient = 1;
    }
}

/*====================================水位计======================================*/

void CreateMeasurementTempJsonFiles::WLG_handle_queryResults(QString str)
{
    str_alone_waterLevelGaugeBuf = str;
    if(str_alone_waterLevelGaugeBuf.mid(0,1) == "A" && str_alone_waterLevelGaugeBuf.length() >= 15)
    {
        str_waterLevelValue = str_alone_waterLevelGaugeBuf.mid(4,5);//截取字符串
        qDebug()<<"已截取...";

        qDebug()<<"单次测水位 QString:"<<str_waterLevelValue;
        int int_waterLevelValue = str_waterLevelValue.toInt();
        qDebug()<<"单次测水位 int:"<<int_waterLevelValue;

        double_waterLevelValue = (double)(int_waterLevelValue/1000.0);//十进制数值转为小数点后3位水位值
        qDebug()<<"单次测水位 double:"<<double_waterLevelValue;
    }
    else
    {
//        double_waterLevelValue = 0.000;
        return;
    }

    generateWLGJSONFile();
}


void CreateMeasurementTempJsonFiles::generateWLGJSONFile()
{
    //获取当前经纬高度
    getLonData();
    getLatData();
    getAltData();

    //创建保存日志的目录
    bool ok;
    QString str = QCoreApplication::applicationDirPath();
    qDebug()<<"str dir:"<<str;
    QDir dir(str + "/jLogs");
    if(dir.exists())
    {
        ok = true;
        qDebug() << "dir exist!";
    }
    else
    {
        ok = dir.mkdir(str + "/jLogs");//只创建一级子目录，即必须保证上级目录存在
        qDebug() << "dir not exists, generate!";
    }

    // 以读写追加方式打开jLogs目录下的waterLevelGauge_temp.json文件，若该文件不存在则会自动创建
    QFile file("jLogs/waterLevelGauge_temp.json");
    if(ok)
    {
        if(!file.open(QIODevice::ReadWrite | QIODevice::Append)) {
            qDebug() << "generate File open error";
        } else {
            qDebug() <<"generate File open!";
        }
    }

    QJsonArray jsonArray;
    QJsonObject jsonObject;

    jsonObject.insert("01_id", j++);//id
    jsonObject.insert("02_time", QDateTime::currentDateTime().toString("hh:mm:ss"));//时间
    jsonObject.insert("03_date", QDateTime::currentDateTime().toString("yyyy.MM.dd"));//日期
    jsonObject.insert("04_waterLevel", double_waterLevelValue);//水位原始值
    jsonObject.insert("05_var_lng", var_lng);//经度
    jsonObject.insert("06_var_lat", var_lat);//纬度
    jsonObject.insert("07_var_alt", var_alt);//高度
    jsonArray.append(jsonObject);

    QJsonDocument jsonDoc;
    jsonDoc.setArray(jsonArray);

    file.write(jsonDoc.toJson());

    file.close();

    if(str_alone_waterLevelGaugeBuf.mid(0,1) == "A" && str_alone_waterLevelGaugeBuf.length() >= 15)
    {
        emit WLG_receivetheMessage(true);
    }
    else
    {
        emit WLG_receivetheMessage(false);
    }
}

void CreateMeasurementTempJsonFiles::getAltData()
{
    Vehicle* _vehicle = qgcApp()->toolbox()->multiVehicleManager()->activeVehicle();
    if(_vehicle)
    {
        var_alt = _vehicle->altitudeRelative()->rawValueString();
    }

    qDebug()<<"-------------------------------var_alt:"<<var_alt;
}
