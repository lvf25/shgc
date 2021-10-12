#include <QFile>
#include <QFlags>
#include <QPixmap>
#include <QDesktopWidget>
#include <QPainter>
#include <QStyleFactory>
#include <QAction>
#include <QStringListModel>
#include <QRegularExpression>
#include "QGCFileDialogController.h"
#ifdef QGC_ENABLE_BLUETOOTH
#include <QBluetoothLocalDevice>
#endif

#include <QDebug>
#include <QDir>

#include "QGC.h"
#include "QGCApplication.h"
#include "AudioOutput.h"
#include "CmdLineOptParser.h"

#include "UDPLink.h"
#include "LinkManager.h"
#include "UASMessageHandler.h"
#include "QGCTemporaryFile.h"
#include "QGCLoggingCategory.h"

#include "QGCToolbox.h"
#include "AutoPilotPlugin.h"
#include "FirmwarePluginManager.h"
#include "MultiVehicleManager.h"
#include "Vehicle.h"
#include "QmlObjectListModel.h"
#include "Settings/SettingsManager.h"
//#include "CoordinateVector.h"
#include "QGCPositionManager.h"
#include "QGCFileDownload.h"
#include "Controls/ScreenToolsController.h"
#include "MissionManager/MissionManager.h"
#include "MissionManager/MissionCommandTree.h"
#include "QGroundControlQmlGlobal.h"
#include "MissionManager/PlanMasterController.h"

#include "ParameterManager.h"

#ifndef NO_SERIAL_LINK
#include "comm/SerialLink.h"
#endif

#include "QGCMessageBox.h"
#include "QGCQFileDialog.h"
#include "QGCMessageBox.h"

#ifndef __mobile__
////#include "FirmwareUpgradeController.h"
////#include "GeoTagController.h"
////#include "MavlinkConsoleController.h"
#include "GPSManager.h"
#endif


#ifdef QGC_RTLAB_ENABLED
#include "OpalLink.h"
#endif

#ifdef Q_OS_LINUX
#ifndef __mobile__
#include <unistd.h>
#include <sys/types.h>
#endif
#endif
#include "LogSqlModel.h"


QGCApplication* QGCApplication::_app = nullptr;

const char* QGCApplication::_deleteAllSettingsKey           = "DeleteAllSettingsNextBoot";
const char* QGCApplication::_settingsVersionKey             = "SettingsVersion";
// Mavlink status structures for entire app
mavlink_status_t m_mavlink_status[MAVLINK_COMM_NUM_BUFFERS];

static QObject* screenToolsControllerSingletonFactory(QQmlEngine*, QJSEngine*)
{
    ScreenToolsController* screenToolsController = new ScreenToolsController;
    return screenToolsController;
}

static QObject* qgroundcontrolQmlGlobalSingletonFactory(QQmlEngine*, QJSEngine*)
{
    // We create this object as a QGCTool even though it isn't in the toolbox
    QGroundControlQmlGlobal* qmlGlobal = new QGroundControlQmlGlobal(qgcApp(), qgcApp()->toolbox());
    qmlGlobal->setToolbox(qgcApp()->toolbox());

    return qmlGlobal;
}

static QObject* getMainWindowQml(QQmlEngine*,QJSEngine*)
{
    MainWindow* _mainwindow = MainWindow::instance();
    return _mainwindow;
}


QGCApplication::QGCApplication(int &argc, char* argv[])
    : QApplication              (argc, argv)
    , _logOutput                (false)
    , _fakeMobile               (false)
    , _settingsUpgraded         (false)
    , _majorVersion             (0)
    , _minorVersion             (0)
    , _buildVersion             (0)
    , _currentVersionDownload   (nullptr)
    , _gpsRtkFactGroup          (nullptr)
    , _toolbox                  (nullptr)
    , _bluetoothAvailable       (false)
{
    _app = this;  
    QLocale locale = QLocale::system();
    //-- Some forced locales for testing
    //QLocale locale = QLocale(QLocale::German);
    //QLocale locale = QLocale(QLocale::French);
    //QLocale locale = QLocale(QLocale::Chinese);
#if defined (__macos__)
    locale = QLocale(locale.name());
#endif

    //-- Our localization
    if(_QGCTranslator.load(locale, "qgc_", "", ":/localization"))
        _app->installTranslator(&_QGCTranslator);


//#ifndef __android__
//    setAttribute(Qt::AA_DontCreateNativeWidgetSiblings);
//#endif

    // Setup for network proxy support
    QNetworkProxyFactory::setUseSystemConfiguration(true);

    // Parse command line options

    bool fClearSettingsOptions = false; // Clear stored settings
    bool fClearCache = false;           // Clear parameter/airframe caches
    bool logging = false;               // Turn on logging
    QString loggingOptions;



    CmdLineOpt_t rgCmdLineOptions[] = {
        { "--clear-settings",   &fClearSettingsOptions, nullptr },
        { "--clear-cache",      &fClearCache,           nullptr },
        { "--logging",          &logging,               &loggingOptions },
        { "--fake-mobile",      &_fakeMobile,           nullptr },
        { "--log-output",       &_logOutput,            nullptr },
        // Add additional command line option flags here
    };

    ParseCmdLineOptions(argc, argv, rgCmdLineOptions, sizeof(rgCmdLineOptions)/sizeof(rgCmdLineOptions[0]), false);

    // Set up timer for delayed missing fact display
    _missingParamsDelayedDisplayTimer.setSingleShot(true);
    _missingParamsDelayedDisplayTimer.setInterval(_missingParamsDelayedDisplayTimerTimeout);
    connect(&_missingParamsDelayedDisplayTimer, &QTimer::timeout, this, &QGCApplication::_missingParamsDisplay);

    setApplicationName(QGC_APPLICATION_NAME);
    setOrganizationName(QGC_ORG_NAME);
    setOrganizationDomain(QGC_ORG_DOMAIN);

    this->setApplicationVersion(QString(GIT_VERSION));

    // Set settings format
    QSettings::setDefaultFormat(QSettings::IniFormat);
    QSettings settings;
    qDebug() << "Settings location" << settings.fileName() << "Is writable?:" << settings.isWritable();

#ifdef UNITTEST_BUILD
    if (!settings.isWritable()) {
        qWarning() << "Setings location is not writable";
    }
#endif
    // The setting will delete all settings on this boot
    fClearSettingsOptions |= settings.contains(_deleteAllSettingsKey);


    if (fClearSettingsOptions) {
        // User requested settings to be cleared on command line
        settings.clear();

        // Clear parameter cache
        QDir paramDir(ParameterManager::parameterCacheDir());
        paramDir.removeRecursively();
        paramDir.mkpath(paramDir.absolutePath());
    } else {
        // Determine if upgrade message for settings version bump is required. Check and clear must happen before toolbox is started since
        // that will write some settings.
        if (settings.contains(_settingsVersionKey)) {
            if (settings.value(_settingsVersionKey).toInt() != QGC_SETTINGS_VERSION) {
                settings.clear();
                _settingsUpgraded = true;
            }
        } else if (settings.allKeys().count()) {
            // Settings version key is missing and there are settings. This is an upgrade scenario.
            settings.clear();
            _settingsUpgraded = true;
        }
    }
    settings.setValue(_settingsVersionKey, QGC_SETTINGS_VERSION);

    if (fClearCache) {
        QDir dir(ParameterManager::parameterCacheDir());
        dir.removeRecursively();
        QFile airframe(cachedAirframeMetaDataFile());
        airframe.remove();
        QFile parameter(cachedParameterMetaDataFile());
        parameter.remove();
    }

    // Set up our logging filters
    QGCLoggingCategoryRegister::instance()->setFilterRulesFromSettings(loggingOptions);

    // Initialize Bluetooth
#ifdef QGC_ENABLE_BLUETOOTH
    QBluetoothLocalDevice localDevice;
    if (localDevice.isValid())
    {
        _bluetoothAvailable = true;
    }
#endif

    // Gstreamer debug settings
#if defined(__ios__) || defined(__android__)
    // Initialize Video Streaming
//    initializeVideoStreaming(argc, argv, nullptr, nullptr);
#else
    QString savePath, gstDebugLevel;
    if (settings.contains(AppSettings::savePathName)) {
        savePath = settings.value(AppSettings::savePathName).toString();
    }
    if(savePath.isEmpty()) {
        savePath = "/tmp";
    }
    savePath = savePath + "/Logs/gst";
    if (!QDir(savePath).exists()) {
        QDir().mkpath(savePath);
    }
    if (settings.contains(AppSettings::gstDebugLevelName)) {
        gstDebugLevel = "*:" + settings.value(AppSettings::gstDebugLevelName).toString();
    }
//    // Initialize Video Streaming
//    initializeVideoStreaming(argc, argv, savePath.toUtf8().data(), gstDebugLevel.toUtf8().data());
#endif

    _toolbox = new QGCToolbox(this);
    _toolbox->setChildToolboxes();

#ifndef __mobile__
    _gpsRtkFactGroup = new GPSRTKFactGroup(this);
   GPSManager *gpsManager = _toolbox->gpsManager();
   if (gpsManager) {
       connect(gpsManager, &GPSManager::onConnect,          this, &QGCApplication::_onGPSConnect);
       connect(gpsManager, &GPSManager::onDisconnect,       this, &QGCApplication::_onGPSDisconnect);
       connect(gpsManager, &GPSManager::surveyInStatus,     this, &QGCApplication::_gpsSurveyInStatus);
       connect(gpsManager, &GPSManager::satelliteUpdate,    this, &QGCApplication::_gpsNumSatellites);
   }
#endif /* __mobile__ */

//    _checkForNewVersion();

}


QGCApplication::~QGCApplication()
{
    // Place shutdown code in _shutdown
    _app = nullptr;
}

void QGCApplication::_initCommon(void)
{
    static const char* kRefOnly         = "Reference only";
    static const char* kQGCControllers  = "QGroundControl.Controllers";
    static const char* kQGCVehicle      = "QGroundControl.Vehicle";

    qmlRegisterUncreatableType<Vehicle>             (kQGCVehicle,                           1, 0, "Vehicle",                    kRefOnly);
    qmlRegisterUncreatableType<MissionItem>         (kQGCVehicle,                           1, 0, "MissionItem",                kRefOnly);
    qmlRegisterUncreatableType<MissionManager>      (kQGCVehicle,                           1, 0, "MissionManager",             kRefOnly);
    qmlRegisterUncreatableType<ParameterManager>    (kQGCVehicle,                           1, 0, "ParameterManager",           kRefOnly);
    qmlRegisterUncreatableType<LinkInterface>       (kQGCVehicle,                           1, 0, "LinkInterface",              kRefOnly);
    qmlRegisterUncreatableType<MissionController>   (kQGCControllers,                       1, 0, "MissionController",          kRefOnly);
    qmlRegisterUncreatableType<QmlObjectListModel>  ("QGroundControl",                      1, 0, "QmlObjectListModel",         kRefOnly);
    qmlRegisterUncreatableType<QGCPositionManager>  ("QGroundControl.QGCPositionManager",   1, 0, "QGCPositionManager",         kRefOnly);

    qmlRegisterUncreatableType<MissionCommandTree>  ("QGroundControl",                      1, 0, "MissionCommandTree",         kRefOnly);

//    qmlRegisterType<ParameterEditorController>      (kQGCControllers,                       1, 0, "ParameterEditorController");
    qmlRegisterType<ScreenToolsController>          (kQGCControllers,                       1, 0, "ScreenToolsController");

    qmlRegisterType<PlanMasterController>           (kQGCControllers,                       1, 0, "PlanMasterController");
    qmlRegisterType<QGCFileDialogController>        (kQGCControllers,                       1, 0, "QGCFileDialogController");


    qmlRegisterType<QGCMAVLinkLogPlayer>            (kQGCControllers,                           1, 0, "QGCMAVLinkLogPlayer");
    qmlRegisterType<LogSqlModel>                    (kQGCControllers,                           1, 0, "LogSqlModel");

    // Register Qml Singletons
    qmlRegisterSingletonType<QGroundControlQmlGlobal>   ("QGroundControl",                          1, 0, "QGroundControl",         qgroundcontrolQmlGlobalSingletonFactory);
    qmlRegisterSingletonType<ScreenToolsController>     ("QGroundControl.ScreenToolsController",    1, 0, "ScreenToolsController",  screenToolsControllerSingletonFactory);
//    qmlRegisterSingletonType<ShapeFileHelper>           ("QGroundControl.ShapeFileHelper",          1, 0, "ShapeFileHelper",        shapeFileHelperSingletonFactory);
//    qmlRegisterSingletonType<MainWindow>     ("MainWindow",    1, 0, "MainWindow",  getMainWindowQml);

}

bool QGCApplication::_initForNormalAppBoot(void)
{
    QSettings settings;

    // Exit main application when last window is closed
    connect(this, &QGCApplication::lastWindowClosed, this, QGCApplication::quit);

    MainWindow* mainWindow = MainWindow::_create();


//    mainWindow->show();
    Q_CHECK_PTR(mainWindow);
//    qmlRegisterUncreatableType<MainWindow>             ("MainWindow",                           1, 0, "MainWindow",                    "Reference only");

    // Now that main window is up check for lost log files
    connect(this, &QGCApplication::checkForLostLogFiles, toolbox()->mavlinkProtocol(), &MAVLinkProtocol::checkForLostLogFiles);
    emit checkForLostLogFiles();

    // Load known link configurations
    toolbox()->linkManager()->loadLinkConfigurationList();

    if (_settingsUpgraded) {
        showMessage(QString(tr("The format for %1 saved settings has been modified. "
                    "Your saved settings have been reset to defaults.")).arg(applicationName()));
    }


    // Connect links with flag AutoconnectLink
//    toolbox()->linkManager()->startAutoConnectedLinks();

    settings.sync();

    qmlRegisterSingletonType<MainWindow>        ("MainWindow",                       1, 0, "MainWindow",getMainWindowQml);


    return true;
}

/// Called when the delay timer fires to show the missing parameters warning
void QGCApplication::_missingParamsDisplay(void)
{
    if (_missingParams.count()) {
        QString params;
        foreach (const QString &name, _missingParams) {
            if (params.isEmpty()) {
                params += name;
            } else {
                params += QString(", %1").arg(name);
            }
        }
        _missingParams.clear();

        showMessage(tr("Parameters are missing from firmware. You may be running a version of firmware QGC does not work correctly with or your firmware has a bug in it. Missing params: %1").arg(params));
    }
}


void QGCApplication::_shutdown(void)
{
    // This code is specifically not in the destructor since the application object may not be available in the destructor.
    // This cause problems for deleting object like settings which are in the toolbox which may have qml references. By
    // moving them here and having main.cc call this prior to deleting the app object we make sure app object is still
    // around while these things are shutting down.

    MainWindow* mainWindow = MainWindow::instance();
    if (mainWindow) {
        delete mainWindow;
    }

    delete _toolbox;
}

void QGCApplication::showMessage(const QString& message)
{
    // PreArm messages are handled by Vehicle and shown in Map
    if (message.startsWith(QStringLiteral("PreArm")) || message.startsWith(QStringLiteral("preflight"), Qt::CaseInsensitive)) {
        return;
    }
    qDebug() << message << endl;

//    emit showmessage(message);
}

QObject* QGCApplication::_rootQmlObject()
{
    MainWindow * _MainWindow = MainWindow::instance();
    if (_MainWindow) {
        return _MainWindow->rootQmlObject();
    }else {
        qWarning() << "Why is MainWindow missing?";
        return nullptr;
    }
}


void QGCApplication::checkTelemetrySavePathOnMainThread(void)
{
    // This is called with an active vehicle so don't pop message boxes which holds ui thread
    _checkTelemetrySavePath(false /* useMessageBox */);
}


bool QGCApplication::_checkTelemetrySavePath(bool useMessageBox)
{
    QString errorTitle = tr("Telemetry save error");

    QString saveDirPath = _toolbox->settingsManager()->appSettings()->telemetrySavePath();
    if (saveDirPath.isEmpty()) {
        QString error = tr("Unable to save telemetry log. Application save directory is not set.");
#ifndef __mobile__
        if (useMessageBox) {
            QGCMessageBox::warning(errorTitle, error);
        } else {
#endif
            Q_UNUSED(useMessageBox);
            showMessage(error);
#ifndef __mobile__
        }
#endif
        return false;
    }

    QDir saveDir(saveDirPath);
    if (!saveDir.exists()) {
        QString error = tr("Unable to save telemetry log. Telemetry save directory \"%1\" does not exist.").arg(saveDirPath);
#ifndef __mobile__
        if (useMessageBox) {
            QGCMessageBox::warning(errorTitle, error);
        } else {
#endif
            showMessage(error);
#ifndef __mobile__
        }
#endif
        return false;
    }

    return true;
}

QString QGCApplication::cachedAirframeMetaDataFile(void)
{
    QSettings settings;
    QDir airframeDir = QFileInfo(settings.fileName()).dir();
    return airframeDir.filePath(QStringLiteral("PX4AirframeFactMetaData.xml"));
}

QString QGCApplication::cachedParameterMetaDataFile(void)
{
    QSettings settings;
    QDir parameterDir = QFileInfo(settings.fileName()).dir();
    return parameterDir.filePath(QStringLiteral("ParameterFactMetaData.xml"));
}


void QGCApplication::saveTelemetryLogOnMainThread(QString tempLogfile)
{
    // The vehicle is gone now and we are shutting down so we need to use a message box for errors to hold shutdown and show the error
    if (_checkTelemetrySavePath(true /* useMessageBox */)) {

        QString saveDirPath = _toolbox->settingsManager()->appSettings()->telemetrySavePath();
        QDir saveDir(saveDirPath);

        QString nameFormat("%1%2.%3");
        QString dtFormat("yyyy-MM-dd hh-mm-ss");

        int tryIndex = 1;
        QString saveFileName = nameFormat.arg(
            QDateTime::currentDateTime().toString(dtFormat)).arg("").arg(toolbox()->settingsManager()->appSettings()->telemetryFileExtension);
        while (saveDir.exists(saveFileName)) {
            saveFileName = nameFormat.arg(
                QDateTime::currentDateTime().toString(dtFormat)).arg(QStringLiteral(".%1").arg(tryIndex++)).arg(toolbox()->settingsManager()->appSettings()->telemetryFileExtension);
        }
        QString saveFilePath = saveDir.absoluteFilePath(saveFileName);

        QFile tempFile(tempLogfile);
        if (!tempFile.copy(saveFilePath)) {
            QString error = tr("Unable to save telemetry log. Error copying telemetry to '%1': '%2'.").arg(saveFilePath).arg(tempFile.errorString());
#ifndef __mobile__
            QGCMessageBox::warning(tr("Telemetry Save Error"), error);
#else
            showMessage(error);
#endif
        }
    }
    QFile::remove(tempLogfile);
}

void QGCApplication::criticalMessageBoxOnMainThread(const QString& title, const QString& msg)
{
#ifdef __mobile__
    Q_UNUSED(title)
    showMessage(msg);
#else
    QGCMessageBox::critical(title, msg);
#endif
}


void QGCApplication::_onGPSConnect()
{
    _gpsRtkFactGroup->connected()->setRawValue(true);
}

void QGCApplication::_onGPSDisconnect()
{
    _gpsRtkFactGroup->connected()->setRawValue(false);
}

void QGCApplication::_gpsSurveyInStatus(float duration, float accuracyMM,  double latitude, double longitude, float altitude, bool valid, bool active)
{
    _gpsRtkFactGroup->currentDuration()->setRawValue(duration);
    _gpsRtkFactGroup->currentAccuracy()->setRawValue(accuracyMM/1000.0);
    _gpsRtkFactGroup->currentLatitude()->setRawValue(latitude);
    _gpsRtkFactGroup->currentLongitude()->setRawValue(longitude);
    _gpsRtkFactGroup->currentAltitude()->setRawValue(altitude);
    _gpsRtkFactGroup->valid()->setRawValue(valid);
    _gpsRtkFactGroup->active()->setRawValue(active);
}

void QGCApplication::_gpsNumSatellites(int numSatellites)
{
    _gpsRtkFactGroup->numSatellites()->setRawValue(numSatellites);
}


void QGCApplication::deleteAllSettingsNextBoot(void)
{
    QSettings settings;
    settings.setValue(_deleteAllSettingsKey, true);
}


void QGCApplication::clearDeleteAllSettingsNextBoot(void)
{
    QSettings settings;
    settings.remove(_deleteAllSettingsKey);
}


void QGCApplication::reportMissingParameter(int componentId, const QString& name)
{
    _missingParams += QString("%1:%2").arg(componentId).arg(name);
    _missingParamsDelayedDisplayTimer.start();
}

/// @brief Returns the QGCApplication object singleton.
QGCApplication* qgcApp(void)
{
    return QGCApplication::_app;
}
