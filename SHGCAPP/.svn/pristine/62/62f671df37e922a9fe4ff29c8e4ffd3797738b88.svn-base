#ifndef SETTINGSMANAGER_H
#define SETTINGSMANAGER_H


#include "QGCLoggingCategory.h"
#include "MultiVehicleManager.h"
#include "QGCToolbox.h"
#include "AppSettings.h"
#include "UnitsSettings.h"
#include "AutoConnectSettings.h"
#include "RTKSettings.h"
#include "FlyViewSettings.h"
#include "PlanViewSettings.h"
#include "FlightMapSettings.h"


#if defined(QGC_AIRMAP_ENABLED)
#include "AirMapSettings.h"
#endif
#include <QVariantList>


class SettingsManager : public QGCTool
{
    Q_OBJECT

public:
    SettingsManager(QGCApplication* app, QGCToolbox* toolbox);

    Q_PROPERTY(QObject* appSettings         READ appSettings            CONSTANT)
    Q_PROPERTY(QObject* autoConnectSettings READ autoConnectSettings    CONSTANT)
    Q_PROPERTY(QObject* unitsSettings       READ unitsSettings          CONSTANT)
    Q_PROPERTY(QObject* rtkSettings         READ rtkSettings            CONSTANT)
    Q_PROPERTY(QObject* flyViewSettings     READ flyViewSettings        CONSTANT)
    Q_PROPERTY(QObject* planViewSettings    READ planViewSettings       CONSTANT)
    Q_PROPERTY(QObject* flightMapSettings   READ flightMapSettings      CONSTANT)


    AppSettings*            appSettings         (void) { return _appSettings; }
    UnitsSettings*          unitsSettings       (void) { return _unitsSettings; }
    AutoConnectSettings*    autoConnectSettings (void) { return _autoConnectSettings; }
    RTKSettings*            rtkSettings         (void) { return _rtkSettings; }
    FlyViewSettings*        flyViewSettings     (void) { return _flyViewSettings; }
    PlanViewSettings*       planViewSettings    (void) { return _planViewSettings; }
    FlightMapSettings*      flightMapSettings   (void) { return _flightMapSettings; }


    // Override from QGCTool
    virtual void setToolbox(QGCToolbox *toolbox);

private:
    UnitsSettings*          _unitsSettings;
    AutoConnectSettings*    _autoConnectSettings;
    AppSettings*            _appSettings;
    RTKSettings*            _rtkSettings;
    FlyViewSettings*        _flyViewSettings;
    PlanViewSettings*       _planViewSettings;
    FlightMapSettings*      _flightMapSettings;

};

#endif // SETTINGSMANAGER_H
