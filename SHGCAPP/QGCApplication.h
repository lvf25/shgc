#ifndef QGCAPPLICATION_H
#define QGCAPPLICATION_H
#include <QApplication>
#include <QTimer>
#include <QQmlApplicationEngine>
#include <QNetworkProxyFactory>


#include "LinkConfiguration.h"
#include "LinkManager.h"
#include "MAVLinkProtocol.h"
#include "FirmwarePluginManager.h"
#include "MultiVehicleManager.h"
#include "FactSystem.h"
#include "AudioOutput.h"
#include "GPSRTKFactGroup.h"
#include "UASMessageHandler.h"
//#include "mainwindow.h"
#ifdef QGC_RTLAB_ENABLED
#include "OpalLink.h"
#endif

#include <QTranslator>

class MainWindow;
class QGCToolbox;
class MainWindow;
class QGCFileDownload;

class QGCApplication :public QApplication    // QtWidget based application
{
    Q_OBJECT
public:
    QGCApplication(int &argc, char* argv[]);
    ~QGCApplication();

    /// @brief Sets the persistent flag to delete all settings the next time QGroundControl is started.
    void deleteAllSettingsNextBoot(void);

    /// @brief Clears the persistent flag to delete all settings the next time QGroundControl is started.
    void clearDeleteAllSettingsNextBoot(void);


    /// @brief Returns true if Qt debug output should be logged to a file
    bool logOutput(void) { return _logOutput; }

public:
    // Still working on getting rid of this and using dependency injection instead for everything
    QGCToolbox* toolbox(void) { return _toolbox; }

    /// Show a non-modal message to the user
    void showMessage(const QString& message);

    /// Used to report a missing Parameter. Warning will be displayed to user. Method may be called
    /// multiple times.
    void reportMissingParameter(int componentId, const QString& name);

public:
    void _initCommon(void);

    bool _initForNormalAppBoot(void);

    static QGCApplication*  _app;

public:
    void _shutdown(void);

    bool _checkTelemetrySavePath(bool useMessageBox);

    /// @return true: Fake ui into showing mobile interface
    bool fakeMobile(void) { return _fakeMobile; }

private:
    static const int    _missingParamsDelayedDisplayTimerTimeout = 1000;    ///< Timeout to wait for next missing fact to come in before display
    QTimer              _missingParamsDelayedDisplayTimer;                  ///< Timer use to delay missing fact display
    QStringList         _missingParams;                                     ///< List of missing facts to be displayed
    bool				_fakeMobile;                                        ///< true: Fake ui into displaying mobile interface
    bool                _settingsUpgraded;                                  ///< true: Settings format has been upgrade to new version
    int                 _majorVersion;
    int                 _minorVersion;
    int                 _buildVersion;
    QGCFileDownload*    _currentVersionDownload;
    GPSRTKFactGroup*    _gpsRtkFactGroup;


    bool _logOutput;        ///< true: Log Qt debug output to file

    QGCToolbox* _toolbox;

    QObject* _rootQmlObject(void);

    bool _bluetoothAvailable;

    QTranslator _QGCTranslator;

    static const char* _settingsVersionKey;             ///< Settings key which hold settings version
    static const char* _deleteAllSettingsKey;           ///< If this settings key is set on boot, all settings will be deleted

    static QString cachedParameterMetaDataFile(void);
    static QString cachedAirframeMetaDataFile(void);


signals:
    /// This is connected to MAVLinkProtocol::checkForLostLogFiles. We signal this to ourselves to call the slot
    /// on the MAVLinkProtocol thread;
    void checkForLostLogFiles(void);

    void showmessage(QString msg);

public slots:

    void _missingParamsDisplay(void);
    /// Save the specified telemetry Log
    void saveTelemetryLogOnMainThread(QString tempLogfile);

    /// Check that the telemetry save path is set correctly
    void checkTelemetrySavePathOnMainThread(void);

    /// You can connect to this slot to show a critical message box from a different thread.
    void criticalMessageBoxOnMainThread(const QString& title, const QString& msg);

    void _onGPSConnect();
    void _onGPSDisconnect();
    void _gpsSurveyInStatus(float duration, float accuracyMM,  double latitude, double longitude, float altitude, bool valid, bool active);
    void _gpsNumSatellites(int numSatellites);

};


/// @brief Returns the QGCApplication object singleton.
QGCApplication* qgcApp(void);

#endif // QGCAPPLICATION_H
