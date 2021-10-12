#pragma once


#include "SettingsGroup.h"
#include <QObject>




class AutoConnectSettings : public SettingsGroup
{
    Q_OBJECT

public:
    AutoConnectSettings(QObject* parent = nullptr);

    DEFINE_SETTING_NAME_GROUP();

    DEFINE_SETTINGFACT(autoConnectUDP)
    DEFINE_SETTINGFACT(autoConnectPixhawk)
    DEFINE_SETTINGFACT(autoConnectSiKRadio)
    DEFINE_SETTINGFACT(autoConnectPX4Flow)
    DEFINE_SETTINGFACT(autoConnectRTKGPS)
    DEFINE_SETTINGFACT(autoConnectLibrePilot)
    DEFINE_SETTINGFACT(autoConnectNmeaPort)
    DEFINE_SETTINGFACT(autoConnectNmeaBaud)
    DEFINE_SETTINGFACT(udpListenPort)
    DEFINE_SETTINGFACT(udpTargetHostIP)
    DEFINE_SETTINGFACT(udpTargetHostPort)
    DEFINE_SETTINGFACT(nmeaUdpPort)
};

