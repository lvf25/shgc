#ifndef APMFirmwarePluginFactory_H
#define APMFirmwarePluginFactory_H

#include "FirmwarePlugin/FirmwarePlugin.h"

class ArduCopterFirmwarePlugin;
class ArduPlaneFirmwarePlugin;
class ArduRoverFirmwarePlugin;
class ArduSubFirmwarePlugin;

class APMFirmwarePluginFactory : public FirmwarePluginFactory
{
    Q_OBJECT

public:
    APMFirmwarePluginFactory(void);

    QList<MAV_AUTOPILOT>    supportedFirmwareTypes      (void) const final;
    FirmwarePlugin*         firmwarePluginForAutopilot  (MAV_AUTOPILOT autopilotType, MAV_TYPE vehicleType) final;

private:
    ArduCopterFirmwarePlugin*   _arduCopterPluginInstance;
    ArduPlaneFirmwarePlugin*    _arduPlanePluginInstance;
    ArduRoverFirmwarePlugin*    _arduRoverPluginInstance;
    ArduSubFirmwarePlugin*      _arduSubPluginInstance;
};

#endif
