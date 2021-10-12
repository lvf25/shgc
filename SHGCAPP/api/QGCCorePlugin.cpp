#include "QGCCorePlugin.h"
#include "QmlComponentInfo.h"
#include "QGCApplication.h"
#include "QGCOptions.h"
#include "FactSystem/FactMetaData.h"
#include "QGCLoggingCategory.h"

#include "qmlobjecttablemodel.h"

class QGCCorePlugin_p
{
public:
    QGCCorePlugin_p()
    {
    }

    ~QGCCorePlugin_p()
    {
        if(pGeneral)
            delete pGeneral;
        if(pCommLinks)
            delete pCommLinks;
        if(pOfflineMaps)
            delete pOfflineMaps;
#if defined(QGC_GST_TAISYNC_ENABLED)
        if(pTaisync)
            delete pTaisync;
#endif
#if defined(QGC_AIRMAP_ENABLED)
        if(pAirmap)
            delete pAirmap;
#endif
        if(pMAVLink)
            delete pMAVLink;
        if(pConsole)
            delete pConsole;
#if defined(QT_DEBUG)
        if(pMockLink)
            delete pMockLink;
        if(pDebug)
            delete pDebug;
#endif
        if(defaultOptions)
            delete defaultOptions;
    }

    QmlComponentInfo* pGeneral                  = nullptr;
    QmlComponentInfo* pCommLinks                = nullptr;
    QmlComponentInfo* pOfflineMaps              = nullptr;
#if defined(QGC_GST_TAISYNC_ENABLED)
    QmlComponentInfo* pTaisync                  = nullptr;
#endif
#if defined(QGC_AIRMAP_ENABLED)
    QmlComponentInfo* pAirmap                   = nullptr;
#endif
    QmlComponentInfo* pMAVLink                  = nullptr;
    QmlComponentInfo* pConsole                  = nullptr;
    QmlComponentInfo* pHelp                     = nullptr;
#if defined(QT_DEBUG)
    QmlComponentInfo* pMockLink                 = nullptr;
    QmlComponentInfo* pDebug                    = nullptr;
#endif

    QmlComponentInfo*   valuesPageWidgetInfo    = nullptr;
    QmlComponentInfo*   cameraPageWidgetInfo    = nullptr;
    QmlComponentInfo*   videoPageWidgetInfo     = nullptr;
    QmlComponentInfo*   healthPageWidgetInfo    = nullptr;
    QmlComponentInfo*   vibrationPageWidgetInfo = nullptr;

    QGCOptions*         defaultOptions          = nullptr;
    QVariantList        settingsList;
    QVariantList        instrumentPageWidgetList;

    QmlObjectListModel _emptyCustomMapItems;
};

QGCCorePlugin::~QGCCorePlugin()
{
    if(_p) {
        delete _p;
    }
}

QGCCorePlugin::QGCCorePlugin(QGCApplication *app, QGCToolbox* toolbox)
    : QGCTool(app, toolbox)
    , _showTouchAreas(false)
    , _showAdvancedUI(true)
{
    _p = new QGCCorePlugin_p;

}

void QGCCorePlugin::setToolbox(QGCToolbox *toolbox)
{
    QGCTool::setToolbox(toolbox);
    QQmlEngine::setObjectOwnership(this, QQmlEngine::CppOwnership);
    qmlRegisterUncreatableType<QGCCorePlugin>("QGroundControl.QGCCorePlugin", 1, 0, "QGCCorePlugin", "Reference only");
    qmlRegisterUncreatableType<QGCOptions>("QGroundControl.QGCOptions",       1, 0, "QGCOptions",    "Reference only");

//    _shgcTableModel = new QmlObjectTableModel();

}

bool QGCCorePlugin::overrideSettingsGroupVisibility(QString name)
{
    Q_UNUSED(name);

    // Always show all
    return true;
}

bool QGCCorePlugin::mavlinkMessage(Vehicle* vehicle, LinkInterface* link, mavlink_message_t message)
{
    Q_UNUSED(vehicle);
    Q_UNUSED(link);
    Q_UNUSED(message);

    return true;
}

bool QGCCorePlugin::adjustSettingMetaData(const QString& settingsGroup, FactMetaData& metaData)
{
//    if (settingsGroup != AppSettings::settingsGroup) {
//        // All changes refer to AppSettings
//        return true;
//    }

//    //-- Default Palette
//    if (metaData.name() == AppSettings::indoorPaletteName) {
//        QVariant outdoorPalette;
//#if defined (__mobile__)
//        outdoorPalette = 0;
//#else
//        outdoorPalette = 1;
//#endif
//        metaData.setRawDefaultValue(outdoorPalette);
//        return true;
//    //-- Auto Save Telemetry Logs
//    } else if (metaData.name() == AppSettings::telemetrySaveName) {
//#if defined (__mobile__)
//        metaData.setRawDefaultValue(false);
//        return true;
//#else
//        metaData.setRawDefaultValue(true);
//        return true;
//#endif
//#if defined(__ios__)
//    } else if (metaData.name() == AppSettings::savePathName) {
//        QString appName = qgcApp()->applicationName();
//        QDir rootDir = QDir(QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation));
//        metaData.setRawDefaultValue(rootDir.filePath(appName));
//        return false;
//#endif
//    }
    return true; // Show setting in ui
}


QGCOptions* QGCCorePlugin::options()
{
    if(!_p->defaultOptions) {
        _p->defaultOptions = new QGCOptions();
    }
    return _p->defaultOptions;
}
