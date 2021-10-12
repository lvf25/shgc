#ifndef FIRMWAREPLUGINMANAGER_H
#define FIRMWAREPLUGINMANAGER_H

#include "FirmwarePlugin.h"
#include "comm/QGCMAVLink.h"
#include "QGCToolbox.h"


class QGCApplication;

class FirmwarePluginManager : public QGCTool
{
    Q_OBJECT
public:
    FirmwarePluginManager(QGCApplication* app, QGCToolbox* toolbox);
    ~FirmwarePluginManager();

    /// Returns list of firmwares which are supported by the system
    QList<MAV_AUTOPILOT> supportedFirmwareTypes(void);

    /// Returns the list of supported vehicle types for the specified firmware
    QList<MAV_TYPE> supportedVehicleTypes(MAV_AUTOPILOT firmwareType);

    /// Returns appropriate plugin for autopilot type.
    ///     @param firmwareType Type of firmwware to return plugin for.
    ///     @param vehicleType Vehicle type to return plugin for.
    /// @return Singleton FirmwarePlugin instance for the specified MAV_AUTOPILOT.
    FirmwarePlugin* firmwarePluginForAutopilot(MAV_AUTOPILOT firmwareType, MAV_TYPE vehicleType);



private:
    FirmwarePluginFactory* _findPluginFactory(MAV_AUTOPILOT firmwareType);

    FirmwarePlugin*         _genericFirmwarePlugin;
    QList<MAV_AUTOPILOT>    _supportedFirmwareTypes;

};

#endif // FIRMWAREPLUGINMANAGER_H
