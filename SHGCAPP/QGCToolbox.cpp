
#include "FactSystem.h"
#include "FirmwarePluginManager.h"
#include "AudioOutput.h"
#ifndef __mobile__
#include "GPSManager.h"
#endif

#include "LinkManager.h"
#include "MAVLinkProtocol.h"
#include "MultiVehicleManager.h"
#include "MissionManager/MissionCommandTree.h"
#include "MAVLinkLogManager.h"
#include "QGCPositionManager.h"
#include "QGCCorePlugin.h"
#include "SettingsManager.h"
#include "QGCApplication.h"
#include "uas/UASMessageHandler.h"
#include "SHGCDataModel.h"
#include "qmlobjecttablemodel.h"

//#if defined(QGC_AIRMAP_ENABLED)
//#include "AirMapManager.h"
//#else
//#include "AirspaceManager.h"
//#endif
#if defined(QGC_GST_TAISYNC_ENABLED)
//#include "TaisyncManager.h"
#endif

#if defined(QGC_CUSTOM_BUILD)
#include CUSTOMHEADER
#endif


QGCToolbox::QGCToolbox(QGCApplication* app)
{

    _settingsManager        = new SettingsManager           (app, this);

    //-- Scan and load plugins
//    _scanAndLoadPlugins(app);
    _corePlugin = new QGCCorePlugin(app, this);
    _audioOutput            = new AudioOutput               (app, this);


    _factSystem             = new FactSystem                (app, this);
    _firmwarePluginManager  = new FirmwarePluginManager     (app, this);
#ifndef __mobile__
    _gpsManager             = new GPSManager                (app, this);
#endif
    _linkManager            = new LinkManager               (app, this);

    _mavlinkProtocol        = new MAVLinkProtocol           (app, this);
    _missionCommandTree     = new MissionCommandTree        (app, this);

    _multiVehicleManager    = new MultiVehicleManager       (app, this);
    _uasMessageHandler      = new UASMessageHandler         (app, this);

    _qgcPositionManager     = new QGCPositionManager        (app, this);

    _mavlinkLogManager      = new MAVLinkLogManager         (app, this);

    _shgcdatamodel          = new SHGCDataModel             (app, this);
}


void QGCToolbox::setChildToolboxes(void)
{
    _settingsManager->setToolbox(this);

    _corePlugin->setToolbox(this);

    _factSystem->setToolbox(this);

    _firmwarePluginManager->setToolbox(this);

#ifndef __mobile__
    _gpsManager->setToolbox(this);
#endif

    _linkManager->setToolbox(this);

    _mavlinkProtocol->setToolbox(this);
    _missionCommandTree->setToolbox(this);

    _multiVehicleManager->setToolbox(this);
    _uasMessageHandler->setToolbox(this);

    _qgcPositionManager->setToolbox(this);

    _mavlinkLogManager->setToolbox(this);

    _shgcdatamodel->setToolbox(this);
}



void QGCToolbox::_scanAndLoadPlugins(QGCApplication* app)
{
    _corePlugin = new QGCCorePlugin(app, app->toolbox());
}


QGCTool::QGCTool(QGCApplication* app, QGCToolbox* toolbox)
    : QObject(toolbox)
    , _app(app)
    , _toolbox(nullptr)
{
}

void QGCTool::setToolbox(QGCToolbox* toolbox)
{
    _toolbox = toolbox;

}
