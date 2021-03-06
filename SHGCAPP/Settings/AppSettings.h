#ifndef APPSETTINGS_H
#define APPSETTINGS_H
#include "SettingsGroup.h"
#include "QGCMAVLink.h"

class AppSettings : public SettingsGroup
{
    Q_OBJECT

public:
    AppSettings(QObject* parent = nullptr);

    DEFINE_SETTING_NAME_GROUP()

    DEFINE_SETTINGFACT(offlineEditingFirmwareType)
    DEFINE_SETTINGFACT(offlineEditingVehicleType)
    DEFINE_SETTINGFACT(offlineEditingCruiseSpeed)
    DEFINE_SETTINGFACT(offlineEditingHoverSpeed)
    DEFINE_SETTINGFACT(offlineEditingAscentSpeed)
    DEFINE_SETTINGFACT(offlineEditingDescentSpeed)
    DEFINE_SETTINGFACT(batteryPercentRemainingAnnounce)
    DEFINE_SETTINGFACT(defaultMissionItemAltitude)
    DEFINE_SETTINGFACT(telemetrySave)
    DEFINE_SETTINGFACT(telemetrySaveNotArmed)
    DEFINE_SETTINGFACT(audioMuted)
    DEFINE_SETTINGFACT(virtualJoystick)
    DEFINE_SETTINGFACT(appFontPointSize)
    DEFINE_SETTINGFACT(indoorPalette)
    DEFINE_SETTINGFACT(showLargeCompass)
    DEFINE_SETTINGFACT(savePath)
    DEFINE_SETTINGFACT(autoLoadMissions)
    DEFINE_SETTINGFACT(useChecklist)
    DEFINE_SETTINGFACT(mapboxToken)
    DEFINE_SETTINGFACT(esriToken)
    DEFINE_SETTINGFACT(defaultFirmwareType)
    DEFINE_SETTINGFACT(gstDebugLevel)
    DEFINE_SETTINGFACT(followTarget)
    DEFINE_SETTINGFACT(enableTaisync)
    DEFINE_SETTINGFACT(enableTaisyncVideo)

    // Although this is a global setting it only affects ArduPilot vehicle since PX4 automatically starts the stream from the vehicle side
    DEFINE_SETTINGFACT(apmStartMavlinkStreams)

    Q_PROPERTY(QString missionSavePath      READ missionSavePath    NOTIFY savePathsChanged)
    Q_PROPERTY(QString parameterSavePath    READ parameterSavePath  NOTIFY savePathsChanged)
    Q_PROPERTY(QString telemetrySavePath    READ telemetrySavePath  NOTIFY savePathsChanged)
    Q_PROPERTY(QString logSavePath          READ logSavePath        NOTIFY savePathsChanged)
    Q_PROPERTY(QString videoSavePath        READ videoSavePath      NOTIFY savePathsChanged)
    Q_PROPERTY(QString crashSavePath        READ crashSavePath      NOTIFY savePathsChanged)

    Q_PROPERTY(QString planFileExtension        MEMBER planFileExtension        CONSTANT)
    Q_PROPERTY(QString missionFileExtension     MEMBER missionFileExtension     CONSTANT)
    Q_PROPERTY(QString waypointsFileExtension   MEMBER waypointsFileExtension   CONSTANT)
    Q_PROPERTY(QString parameterFileExtension   MEMBER parameterFileExtension   CONSTANT)
    Q_PROPERTY(QString telemetryFileExtension   MEMBER telemetryFileExtension   CONSTANT)
    Q_PROPERTY(QString kmlFileExtension         MEMBER kmlFileExtension         CONSTANT)
    Q_PROPERTY(QString shpFileExtension         MEMBER shpFileExtension         CONSTANT)
    Q_PROPERTY(QString logFileExtension         MEMBER logFileExtension         CONSTANT)

    QString missionSavePath     ();
    QString parameterSavePath   ();
    QString telemetrySavePath   ();
    QString logSavePath         ();
    QString videoSavePath       ();
    QString crashSavePath       ();

    static MAV_AUTOPILOT    offlineEditingFirmwareTypeFromFirmwareType  (MAV_AUTOPILOT firmwareType);
    static MAV_TYPE         offlineEditingVehicleTypeFromVehicleType    (MAV_TYPE vehicleType);

    // Application wide file extensions
    static const char* parameterFileExtension;
    static const char* planFileExtension;
    static const char* missionFileExtension;
    static const char* waypointsFileExtension;
    static const char* fenceFileExtension;
    static const char* rallyPointFileExtension;
    static const char* telemetryFileExtension;
    static const char* kmlFileExtension;
    static const char* shpFileExtension;
    static const char* logFileExtension;

    // Child directories of savePath for specific file types
    static const char* parameterDirectory;
    static const char* telemetryDirectory;
    static const char* missionDirectory;
    static const char* logDirectory;
    static const char* videoDirectory;
    static const char* crashDirectory;

signals:
    void savePathsChanged();

private slots:
    void _indoorPaletteChanged();
    void _checkSavePathDirectories();

};

#endif // APPSETTINGS_H
