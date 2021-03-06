#ifndef ArduCopterFirmwarePlugin_H
#define ArduCopterFirmwarePlugin_H

#include "APMFirmwarePlugin.h"

class APMCopterMode : public APMCustomMode
{
public:
    enum Mode {
        STABILIZE   = 0,   // hold level position
        ACRO        = 1,   // rate control
        ALT_HOLD    = 2,   // AUTO control
        AUTO        = 3,   // AUTO control
        GUIDED      = 4,   // AUTO control
        LOITER      = 5,   // Hold a single location
        RTL         = 6,   // AUTO control
        CIRCLE      = 7,   // AUTO control
        POSITION    = 8,   // Deprecated
        LAND        = 9,   // AUTO control
        OF_LOITER   = 10,  // Deprecated
        DRIFT       = 11,  // Drift 'Car Like' mode
        RESERVED_12 = 12,  // RESERVED FOR FUTURE USE
        SPORT       = 13,
        FLIP        = 14,
        AUTOTUNE    = 15,
        POS_HOLD    = 16, // HYBRID LOITER.
        BRAKE       = 17,
        THROW       = 18,
        AVOID_ADSB  = 19,
        GUIDED_NOGPS= 20,
        SAFE_RTL   = 21,   //Safe Return to Launch
    };
    static const int modeCount = 22;

    APMCopterMode(uint32_t mode, bool settable);
};

class ArduCopterFirmwarePlugin : public APMFirmwarePlugin
{
    Q_OBJECT

public:
    ArduCopterFirmwarePlugin(void);

    // Overrides from FirmwarePlugin
    void    guidedModeLand                      (Vehicle* vehicle) final;
    const FirmwarePlugin::remapParamNameMajorVersionMap_t& paramNameRemapMajorVersionMap(void) const final { return _remapParamName; }
    int     remapParamNameHigestMinorVersionNumber(int majorVersionNumber) const final;
    bool    multiRotorCoaxialMotors             (Vehicle* vehicle) final;
    bool    multiRotorXConfig                   (Vehicle* vehicle) final;
    QString offlineEditingParamFile             (Vehicle* vehicle) final { Q_UNUSED(vehicle); return QStringLiteral(":/FirmwarePlugin/APM/Copter3.5.OfflineEditing.params"); }
    QString pauseFlightMode                     (void) const override { return QString("Brake"); }
    QString landFlightMode                      (void) const override { return QString("Land"); }
    QString takeControlFlightMode               (void) const override { return QString("Loiter"); }
//    bool    vehicleYawsToNextWaypointInMission  (const Vehicle* vehicle) const final;
    QString autoDisarmParameter                 (Vehicle* vehicle) final { Q_UNUSED(vehicle); return QStringLiteral("DISARM_DELAY"); }

private:
    static bool _remapParamNameIntialized;
    static FirmwarePlugin::remapParamNameMajorVersionMap_t  _remapParamName;
};

#endif
