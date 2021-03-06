#ifndef QGCTOOLBOX_H
#define QGCTOOLBOX_H

#include <QObject>

class FactSystem;
class FirmwarePluginManager;
class AudioOutput;
class GPSManager;
//class JoystickManager;
//class FollowMe;
class LinkManager;
class MAVLinkProtocol;
class MissionCommandTree;
class MultiVehicleManager;
//class QGCMapEngineManager;
class QGCApplication;
//class QGCImageProvider;
class UASMessageHandler;
class QGCPositionManager;
//class VideoManager;
class MAVLinkLogManager;
class QGCCorePlugin;
class SettingsManager;
//class AirspaceManager;
class QmlObjectTableModel;
class SHGCDataModel;

#if defined(QGC_GST_TAISYNC_ENABLED)
class TaisyncManager;
#endif

class QGCToolbox : public QObject
{
    Q_OBJECT
public:
    QGCToolbox(QGCApplication* app);

    FirmwarePluginManager*      firmwarePluginManager   () { return _firmwarePluginManager; }
    AudioOutput*                audioOutput             () { return _audioOutput; }

    LinkManager*                linkManager             () { return _linkManager; }
    QGCCorePlugin*              corePlugin              () { return _corePlugin; }
    SettingsManager*            settingsManager         () { return _settingsManager; }
    MAVLinkProtocol*            mavlinkProtocol         () { return _mavlinkProtocol; }
    MultiVehicleManager*        multiVehicleManager     () { return _multiVehicleManager; }
    MissionCommandTree*         missionCommandTree      () { return _missionCommandTree; }
    MAVLinkLogManager*          mavlinkLogManager       () { return _mavlinkLogManager; }
    UASMessageHandler*          uasMessageHandler       () { return _uasMessageHandler; }
    QGCPositionManager*         qgcPositionManager      () { return _qgcPositionManager; }

    SHGCDataModel*              Shgcdatamodel           () { return _shgcdatamodel; }
#ifndef __mobile__
    GPSManager*                 gpsManager              () { return _gpsManager; }
#endif
#if defined(QGC_GST_TAISYNC_ENABLED)
//    TaisyncManager*             taisyncManager          () { return _taisyncManager; }
#endif

    friend class QGCApplication;

private:
    void setChildToolboxes(void);
    void _scanAndLoadPlugins(QGCApplication *app);

    FirmwarePluginManager*      _firmwarePluginManager  = nullptr;
    AudioOutput*                _audioOutput            = nullptr;
    MAVLinkLogManager*          _mavlinkLogManager      = nullptr;
    MissionCommandTree*         _missionCommandTree     = nullptr;
    UASMessageHandler*          _uasMessageHandler      = nullptr;

    MultiVehicleManager*        _multiVehicleManager    = nullptr;
    LinkManager*                _linkManager            = nullptr;
    QGCCorePlugin*              _corePlugin             = nullptr;
    SettingsManager*            _settingsManager        = nullptr;
    FactSystem*                 _factSystem             = nullptr;
    MAVLinkProtocol*            _mavlinkProtocol        = nullptr;
    QGCPositionManager*         _qgcPositionManager     = nullptr;
#ifndef __mobile__
    GPSManager*                 _gpsManager             = nullptr;
#endif

    SHGCDataModel*              _shgcdatamodel          = nullptr;

};


/// This is the base class for all tools
class QGCTool : public QObject {
    Q_OBJECT

public:
    // All tools must be parented to the QGCToolbox and go through a two phase creation. In the constructor the toolbox
    // should only be passed to QGCTool constructor for correct parenting. It should not be referenced or set in the
    // protected member. Then in the second phase of setToolbox calls is where you can reference the toolbox.
    QGCTool(QGCApplication* app, QGCToolbox* toolbox);

    // If you override this method, you must call the base class.
    virtual void setToolbox(QGCToolbox* toolbox);

protected:
    QGCApplication* _app;
    QGCToolbox*     _toolbox;
};

#endif // QGCTOOLBOX_H
