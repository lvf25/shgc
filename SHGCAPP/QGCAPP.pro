#-------------------------------------------------
#
# Project created by QtCreator 2019-07-30T13:37:03
#
#-------------------------------------------------

QMAKE_PROJECT_DEPTH = 0 # undocumented qmake flag to force absolute paths in make files

exists($${OUT_PWD}/QGCAPP.pro) {
    error("You must use shadow build (e.g. mkdir build; cd build; qmake ../QGCAPP.pro).")
}

message(Qt version $$[QT_VERSION])

!equals(QT_MAJOR_VERSION, 5) | !greaterThan(QT_MINOR_VERSION, 8) {
    error("Unsupported Qt version, 5.9+ is required")
}

include(QGCCommon.pri)

TARGET   = QGCAPP
TEMPLATE = app
QGCROOT  = $$PWD

DebugBuild {
    DESTDIR  = $${OUT_PWD}/debug
} else {
    DESTDIR  = $${OUT_PWD}/release
}


#
# OS Specific settings
#

MacBuild {
    QMAKE_INFO_PLIST    = Custom-Info.plist
#    ICON                = $${BASEDIR}/resources/icons/macx.icns
    OTHER_FILES        += Custom-Info.plist
    equals(QT_MAJOR_VERSION, 5) | greaterThan(QT_MINOR_VERSION, 5) {
        LIBS           += -framework ApplicationServices
    }
}

LinuxBuild {
    CONFIG  += qesp_linux_udev
}

WindowsBuild {
    RC_ICONS = resources/icons/QGC.ico
}

#
# Branding
#

QGC_APP_NAME        = "QGroundControl"
QGC_ORG_NAME        = "QGroundControl.org"
QGC_ORG_DOMAIN      = "org.qgroundcontrol"
QGC_APP_DESCRIPTION = "Open source ground control app provided by QGroundControl dev team"
QGC_APP_COPYRIGHT   = "Copyright (C) 2017 QGroundControl Development Team. All rights reserved."

WindowsBuild {
    QGC_INSTALLER_ICON          = "resources/icons/QGC.ico"
    QGC_INSTALLER_HEADER_BITMAP = "installheader.bmp"
}

# Load additional config flags from user_config.pri
exists(user_config.pri):infile(user_config.pri, CONFIG) {
    CONFIG += $$fromfile(user_config.pri, CONFIG)
    message($$sprintf("Using user-supplied additional config: '%1' specified in user_config.pri", $$fromfile(user_config.pri, CONFIG)))
}

#
# Custom Build
#
# QGC will create a "CUSTOMCLASS" object (exposed by your custom build
# and derived from QGCCorePlugin).
# This is the start of allowing custom Plugins, which will eventually use a
# more defined runtime plugin architecture and not require a QGC project
# file you would have to keep in sync with the upstream repo.
#

# This allows you to ignore the custom build even if the custom build
# is present. It's useful to run "regular" builds to make sure you didn't
# break anything.

contains (CONFIG, QGC_DISABLE_CUSTOM_BUILD) {
    message("Disable custom build override")
} else {
    exists($$PWD/custom/custom.pri) {
        message("Found custom build")
        CONFIG  += CustomBuild
        DEFINES += QGC_CUSTOM_BUILD
        # custom.pri must define:
        # CUSTOMCLASS  = YourIQGCCorePluginDerivation
        # CUSTOMHEADER = \"\\\"YourIQGCCorePluginDerivation.h\\\"\"
        include($$PWD/custom/custom.pri)
    }
}

WindowsBuild {
    # Sets up application properties
    QMAKE_TARGET_COMPANY        = "$${QGC_ORG_NAME}"
    QMAKE_TARGET_DESCRIPTION    = "$${QGC_APP_DESCRIPTION}"
    QMAKE_TARGET_COPYRIGHT      = "$${QGC_APP_COPYRIGHT}"
    QMAKE_TARGET_PRODUCT        = "$${QGC_APP_NAME}"
}


#
# Plugin configuration
#
# This allows you to build custom versions of QGC which only includes your
# specific vehicle plugin. To remove support for a firmware type completely,
# disable both the Plugin and PluginFactory entries. To include custom support
# for an existing plugin type disable PluginFactory only. Then provide you own
# implementation of FirmwarePluginFactory and use the FirmwarePlugin and
# AutoPilotPlugin classes as the base clase for your derived plugin
# implementation.

contains (CONFIG, QGC_DISABLE_APM_PLUGIN) {
    message("Disable APM Plugin")
} else {
    CONFIG += APMFirmwarePlugin
}

contains (CONFIG, QGC_DISABLE_APM_PLUGIN_FACTORY) {
    message("Disable APM Plugin Factory")
} else {
    CONFIG += APMFirmwarePluginFactory
}

contains (CONFIG, QGC_DISABLE_PX4_PLUGIN) {
    message("Disable PX4 Plugin")
} else {
    CONFIG += PX4FirmwarePlugin
}

contains (CONFIG, QGC_DISABLE_PX4_PLUGIN_FACTORY) {
    message("Disable PX4 Plugin Factory")
} else {
    CONFIG += PX4FirmwarePluginFactory
}

# Bluetooth
contains (DEFINES, QGC_DISABLE_BLUETOOTH) {
    message("Skipping support for Bluetooth (manual override from command line)")
    DEFINES -= QGC_ENABLE_BLUETOOTH
} else:exists(user_config.pri):infile(user_config.pri, DEFINES, QGC_DISABLE_BLUETOOTH) {
    message("Skipping support for Bluetooth (manual override from user_config.pri)")
    DEFINES -= QGC_ENABLE_BLUETOOTH
} else:exists(user_config.pri):infile(user_config.pri, DEFINES, QGC_ENABLE_BLUETOOTH) {
    message("Including support for Bluetooth (manual override from user_config.pri)")
    DEFINES += QGC_ENABLE_BLUETOOTH
}

# USB Camera and UVC Video Sources
contains (DEFINES, QGC_DISABLE_UVC) {
    message("Skipping support for UVC devices (manual override from command line)")
    DEFINES += QGC_DISABLE_UVC
} else:exists(user_config.pri):infile(user_config.pri, DEFINES, QGC_DISABLE_UVC) {
    message("Skipping support for UVC devices (manual override from user_config.pri)")
    DEFINES += QGC_DISABLE_UVC
} else:LinuxBuild {
    contains(QT_VERSION, 5.5.1) {
        message("Skipping support for UVC devices (conflict with Qt 5.5.1 on Ubuntu)")
        DEFINES += QGC_DISABLE_UVC
    }
}

LinuxBuild {
    CONFIG += link_pkgconfig
}

# Qt configuration

CONFIG += qt \
    thread \
    c++11

DebugBuild {
    CONFIG -= qtquickcompiler
} else {
    CONFIG += qtquickcompiler
}

contains(DEFINES, ENABLE_VERBOSE_OUTPUT) {
    message("Enable verbose compiler output (manual override from command line)")
} else:exists(user_config.pri):infile(user_config.pri, DEFINES, ENABLE_VERBOSE_OUTPUT) {
    message("Enable verbose compiler output (manual override from user_config.pri)")
} else {
    CONFIG += silent
}

QT += \
    concurrent \
    gui \
    location \
    network \
    opengl \
    positioning \
    qml \
    quick \
    quickwidgets \
    sql \
    svg \
    widgets \
    xml \
    texttospeech \

# Multimedia only used if QVC is enabled
!contains (DEFINES, QGC_DISABLE_UVC) {
    QT += \
        multimedia
}

AndroidBuild || iOSBuild {
    # Android and iOS don't unclude these
} else {
    QT += \
        printsupport \
        serialport \
        charts \
}

contains(DEFINES, QGC_ENABLE_BLUETOOTH) {
QT += \
    bluetooth \
}

#  testlib is needed even in release flavor for QSignalSpy support
QT += testlib
ReleaseBuild {
    # We don't need the testlib console in release mode
    QT.testlib.CONFIG -= console
}

#
# Build-specific settings
#

DebugBuild {
!iOSBuild {
    CONFIG += console
}
}


#
# Our QtLocation "plugin"
#

include(QtLocationPlugin/QGCLocationPlugin.pri)

#
# External library configuration
#

include(QGCExternalLibs.pri)

#
# Resources (custom code can replace them)
#

CustomBuild {
    exists($$PWD/custom/qgc.qrc) {
        message("Using custom qgc.qrc")
        RESOURCES += $$PWD/custom/qgc.qrc
    } else {
        RESOURCES += $$PWD/qgc.qrc
    }
    exists($$PWD/custom/qgcresources.qrc) {
        message("Using custom qgcresources.qrc")
        RESOURCES += $$PWD/custom/qgcresources.qrc
    } else {
        RESOURCES += $$PWD/qgcresources.qrc
    }
} else {
    DEFINES += QGC_APPLICATION_NAME=\"\\\"QGroundControl\\\"\"
    DEFINES += QGC_ORG_NAME=\"\\\"QGroundControl.org\\\"\"
    DEFINES += QGC_ORG_DOMAIN=\"\\\"org.qgroundcontrol\\\"\"
    RESOURCES += \
        $$PWD/qgc.qrc \
#        $$PWD/qgcresources.qrc
}

# On Qt 5.9 android versions there is the following bug: https://bugreports.qt.io/browse/QTBUG-61424
# This prevents FileDialog from being used. So we have a temp hack workaround for it which just no-ops
# the FileDialog fallback mechanism on android 5.9 builds.
#equals(QT_MAJOR_VERSION, 5):equals(QT_MINOR_VERSION, 9):AndroidBuild {
#    RESOURCES += $$PWD/HackAndroidFileDialog.qrc
#} else {
#    RESOURCES += $$PWD/HackFileDialog.qrc
#}

#
# Main QGroundControl portion of project file
#

#DebugBuild {
#    # Unit Test resources
#    RESOURCES += UnitTest.qrc
#}

DEPENDPATH += \
    . \
    plugins

INCLUDEPATH += .

INCLUDEPATH += \
    include/ui \
    api \
    AutoPilotPlugins \
    GPS \
    postionManager \
    QtLocationPlugin \
    Settings \
    VehicleSetup \
    Audio \
    comm \
    uas \
    ui \
    Controls\
    MissionManager \
    ui/SHGCSetUp \
    ui/publicClass \
    ui/SHGCDataModel \
    ui/flowVelocityMeasurement \
    comm/flowVelocityMeasurement \

FORMS += \
    ui/MainWindow.ui \
    ui/QGCDashBoard.ui \
    ui/leftMainWindow.ui \
    ui/SHGCUpLoadWidget.ui \
    ui/SHGCSetUp/SHGCMainSetup.ui \
    ui/flowVelocityMeasurement/SHGCFvmApplicationTipsWidget.ui \
    ui/flowVelocityMeasurement/SHGCVelocimeterInteractiveWidget.ui \
    ui/flowVelocityMeasurement/SHGCVelocimeterSetupWidget.ui \
    ui/point_edit_widget.ui

!MobileBuild {
FORMS += \
    ui/QGCMAVLinkLogPlayer.ui \

}

#
# Plugin API
#

HEADERS += \
    api/QGCCorePlugin.h \
    api/QGCOptions.h \
    api/QGCSettings.h \
    api/QmlComponentInfo.h \
    comm/MavlinkMessagesTimer.h \
    ui/QGCDashBoard.h \
    ui/flightdashboard.h \
    ui/powerdisplay.h \
    ui/leftMainWindow.h \
    ui/publicClass/autoSaveSlider.h \
    ui/publicClass/clickedWidget.h \
    ui/publicClass/elecPointer.h \
    ui/publicClass/sliderPointer.h \
    ui/publicClass/switchbutton.h \
    ui/SHGCUpLoadWidget.h \
    ui/SHGCDataModel/qmlobjecttablemodel.h \
    ui/SHGCDataModel/SHGCDataModel.h \
    ui/SHGCSetUp/SHGCMainSetup.h \
    QGCToolbox.h \
    ui/MainWindow.h \
    ui/flowVelocityMeasurement/SHGCFvmApplicationTipsWidget.h \
    ui/flowVelocityMeasurement/SHGCVelocimeterInteractiveWidget.h \
    ui/flowVelocityMeasurement/SHGCVelocimeterSetupWidget.h \
    ui/publicClass/AutoResize.h \
    comm/flowVelocityMeasurement/CreateMeasurementTempJsonFiles.h \
    comm/flowVelocityMeasurement/FlowVelocimeterLink.h \
    comm/flowVelocityMeasurement/SHGCMountManager.h \
    comm/flowVelocityMeasurement/WaterLevelGaugeLink.h \
    comm/SerialLink.h \
    comm/TCPLink.h \
    comm/UDPLink.h \
    comm/LinkManager.h \
    comm/LinkInterface.h \
    sqlinit.h \
    ui/SHGCDataModel/LogSqlModel.h \
    ui/point_edit_widget.h \
    ui/pushbuttonitemdelegate.h \
    ui/qpersistentstyleditemdelegate.h \
#    AutoPilotPlugins/APM/APMSensorsComponentController.h \
    ui/leftMainWindow.h \
#    ui/qmlobjecttablemodel.h \



SOURCES += \
    api/QGCCorePlugin.cpp \
    api/QGCOptions.cc \
    api/QGCSettings.cc \
    api/QmlComponentInfo.cc \
    comm/MavlinkMessagesTimer.cpp \
    ui/QGCDashBoard.cpp \
    ui/flightdashboard.cpp \
    ui/powerdisplay.cpp \
    ui/leftMainWindow.cpp \
    ui/publicClass/autoSaveSlider.cpp \
    ui/publicClass/clickedWidget.cpp \
    ui/publicClass/elecPointer.cpp \
    ui/publicClass/sliderPointer.cpp \
    ui/publicClass/switchbutton.cpp \
    ui/SHGCUpLoadWidget.cpp \
    ui/SHGCDataModel/qmlobjecttablemodel.cpp \
    ui/SHGCDataModel/SHGCDataModel.cpp \
    QGCToolbox.cpp \
    QGCApplication.cpp \
    ui/SHGCSetUp/SHGCMainSetup.cpp \
    ui/MainWindow.cpp \
    ui/flowVelocityMeasurement/SHGCFvmApplicationTipsWidget.cpp \
    ui/flowVelocityMeasurement/SHGCVelocimeterInteractiveWidget.cpp \
    ui/flowVelocityMeasurement/SHGCVelocimeterSetupWidget.cpp \
    ui/publicClass/AutoResize.cpp \
    comm/flowVelocityMeasurement/CreateMeasurementTempJsonFiles.cpp \
    comm/flowVelocityMeasurement/FlowVelocimeterLink.cc \
    comm/flowVelocityMeasurement/SHGCMountManager.cpp \
    comm/flowVelocityMeasurement/WaterLevelGaugeLink.cc \
    comm/LinkManager.cpp \
    ui/SHGCDataModel/LogSqlModel.cpp \
    ui/point_edit_widget.cpp \
    ui/pushbuttonitemdelegate.cpp \
    ui/qpersistentstyleditemdelegate.cpp \
#    AutoPilotPlugins/APM/APMSensorsComponentController.cc \
    ui/leftMainWindow.cpp \
#    ui/qmlobjecttablemodel.cpp \

#
# Unit Test specific configuration goes here (requires full debug build with all plugins)
#

#DebugBuild { PX4FirmwarePlugin { PX4FirmwarePluginFactory  { APMFirmwarePlugin { APMFirmwarePluginFactory { !MobileBuild {
#    DEFINES += UNITTEST_BUILD

#    INCLUDEPATH += \
#        qgcunittest

#    HEADERS += \
#        AnalyzeView/LogDownloadTest.h \
#        Audio/AudioOutputTest.h \
#        FactSystem/FactSystemTestBase.h \
#        FactSystem/FactSystemTestGeneric.h \
#        FactSystem/FactSystemTestPX4.h \
#        FactSystem/ParameterManagerTest.h \
#        MissionManager/CameraCalcTest.h \
#        MissionManager/CameraSectionTest.h \
#        MissionManager/CorridorScanComplexItemTest.h \
#        MissionManager/FWLandingPatternTest.h \
#        MissionManager/MissionCommandTreeTest.h \
#        MissionManager/MissionControllerManagerTest.h \
#        MissionManager/MissionControllerTest.h \
#        MissionManager/MissionItemTest.h \
#        MissionManager/MissionManagerTest.h \
#        MissionManager/MissionSettingsTest.h \
#        MissionManager/PlanMasterControllerTest.h \
#        MissionManager/QGCMapPolygonTest.h \
#        MissionManager/QGCMapPolylineTest.h \
#        MissionManager/SectionTest.h \
#        MissionManager/SimpleMissionItemTest.h \
#        MissionManager/SpeedSectionTest.h \
#        MissionManager/StructureScanComplexItemTest.h \
#        MissionManager/SurveyComplexItemTest.h \
#        MissionManager/TransectStyleComplexItemTest.h \
#        MissionManager/VisualMissionItemTest.h \
#        qgcunittest/FileDialogTest.h \
#        qgcunittest/FileManagerTest.h \
#        qgcunittest/FlightGearTest.h \
#        qgcunittest/GeoTest.h \
#        qgcunittest/LinkManagerTest.h \
#        qgcunittest/MainWindowTest.h \
#        qgcunittest/MavlinkLogTest.h \
#        qgcunittest/MessageBoxTest.h \
#        qgcunittest/MultiSignalSpy.h \
#        qgcunittest/RadioConfigTest.h \
#        qgcunittest/TCPLinkTest.h \
#        qgcunittest/TCPLoopBackServer.h \
#        qgcunittest/UnitTest.h \
#        Vehicle/SendMavCommandTest.h \

#    SOURCES += \
#        AnalyzeView/LogDownloadTest.cc \
#        Audio/AudioOutputTest.cc \
#        FactSystem/FactSystemTestBase.cc \
#        FactSystem/FactSystemTestGeneric.cc \
#        FactSystem/FactSystemTestPX4.cc \
#        FactSystem/ParameterManagerTest.cc \
#        MissionManager/CameraCalcTest.cc \
#        MissionManager/CameraSectionTest.cc \
#        MissionManager/CorridorScanComplexItemTest.cc \
#        MissionManager/FWLandingPatternTest.cc \
#        MissionManager/MissionCommandTreeTest.cc \
#        MissionManager/MissionControllerManagerTest.cc \
#        MissionManager/MissionControllerTest.cc \
#        MissionManager/MissionItemTest.cc \
#        MissionManager/MissionManagerTest.cc \
#        MissionManager/MissionSettingsTest.cc \
#        MissionManager/PlanMasterControllerTest.cc \
#        MissionManager/QGCMapPolygonTest.cc \
#        MissionManager/QGCMapPolylineTest.cc \
#        MissionManager/SectionTest.cc \
#        MissionManager/SimpleMissionItemTest.cc \
#        MissionManager/SpeedSectionTest.cc \
#        MissionManager/StructureScanComplexItemTest.cc \
#        MissionManager/SurveyComplexItemTest.cc \
#        MissionManager/TransectStyleComplexItemTest.cc \
#        MissionManager/VisualMissionItemTest.cc \
#        qgcunittest/FileDialogTest.cc \
#        qgcunittest/FileManagerTest.cc \
#        qgcunittest/FlightGearTest.cc \
#        qgcunittest/GeoTest.cc \
#        qgcunittest/LinkManagerTest.cc \
#        qgcunittest/MainWindowTest.cc \
#        qgcunittest/MavlinkLogTest.cc \
#        qgcunittest/MessageBoxTest.cc \
#        qgcunittest/MultiSignalSpy.cc \
#        qgcunittest/RadioConfigTest.cc \
#        qgcunittest/TCPLinkTest.cc \
#        qgcunittest/TCPLoopBackServer.cc \
#        qgcunittest/UnitTest.cc \
#        qgcunittest/UnitTestList.cc \
#        Vehicle/SendMavCommandTest.cc \
#} } } } } }

# Main QGC Headers and Source files



HEADERS += \
    Audio/AudioOutput.h \
    CmdLineOptParser.h \
    FirmwarePlugin/PX4/px4_custom_mode.h \
    JsonHelper.h \
    MG.h \
    postionManager/QGCPositionManager.h \
    postionManager/SimulatedPosition.h \
    QGC.h \
    QGCApplication.h \
    QGCConfig.h \
    QGCFileDownload.h \
#    QGCGeo.h \
    QGCLoggingCategory.h \
    QGCQGeoCoordinate.h \
    QGCTemporaryFile.h \
    ui/SHGCDataModel/QmlObjectListModel.h \
#    QtLocationPlugin/QMLControl/QGCMapEngineManager.h \
    Settings/AppSettings.h \
    Settings/AutoConnectSettings.h \
    Settings/RTKSettings.h \
    Settings/SettingsGroup.h \
    Settings/SettingsManager.h \
    Settings/UnitsSettings.h \
    Settings/FlightMapSettings.h \
    Settings/FlyViewSettings.h \
    Settings/PlanViewSettings.h \
    Vehicle/MAVLinkLogManager.h \
    comm/LinkConfiguration.h \
    comm/MAVLinkProtocol.h \
    comm/ProtocolInterface.h \
    comm/QGCMAVLink.h \
    comm/UdpIODevice.h \
    uas/UAS.h \
    uas/UASInterface.h \
    uas/UASMessageHandler.h \
#    UTM.h \
    QGCMessageBox.h \
    Controls/ScreenToolsController.h \
    Controls/QGroundControlQmlGlobal.h \
    MissionManager/MissionCommandTree.h \
    MissionManager/MissionCommandUIInfo.h \
    MissionManager/MissionCommandList.h \
    MissionManager/MissionManager.h \
    MissionManager/MissionItem.h \
    MissionManager/PlanManager.h \
    MissionManager/VisualMissionItem.h \
    MissionManager/MissionController.h \
    MissionManager/PlanMasterController.h \
    MissionManager/PlanElementController.h \
    MissionManager/SimpleMissionItem.h \
    MissionManager/MissionSettingsItem.h \
    MissionManager/SpeedSection.h \
    MissionManager/CameraSection.h \
    Controls/CoordinateVector.h \
    MissionManager/Section.h \
    MissionManager/ComplexMissionItem.h \
    Controls/QGCFileDialogController.h \
    MissionManager/FixedWingLandingComplexItem.h \
#    MissionManager/SurveyComplexItem.h \
#    MissionManager/TransectStyleComplexItem.h \
#    MissionManager/StructureScanComplexItem.h \
#    MissionManager/CorridorScanComplexItem.h \



AndroidBuild {
HEADERS += \
#    Joystick/JoystickAndroid.h \
}

DebugBuild {
HEADERS += \
#    comm/MockLink.h \
#    comm/MockLinkFileServer.h \
#    comm/MockLinkMissionItemHandler.h \
}

WindowsBuild {
    PRECOMPILED_HEADER += stable_headers.h
    HEADERS += stable_headers.h
    CONFIG -= silent
    OTHER_FILES += .appveyor.yml
}

contains(DEFINES, QGC_ENABLE_BLUETOOTH) {
    HEADERS += \
#    comm/BluetoothLink.h \
}

!NoSerialBuild {
HEADERS += \
    comm/QGCSerialPortInfo.h \
}

!MobileBuild {
HEADERS += \
#    AnalyzeView/GeoTagController.h \
#    AnalyzeView/MavlinkConsoleController.h \
    GPS/Drivers/src/gps_helper.h \
    GPS/Drivers/src/rtcm.h \
    GPS/Drivers/src/ashtech.h \
    GPS/Drivers/src/ubx.h \
    GPS/Drivers/src/sbf.h \
    GPS/GPSManager.h \
    GPS/GPSPositionMessage.h \
    GPS/GPSProvider.h \
    GPS/RTCM/RTCMMavlink.h \
    GPS/definitions.h \
    GPS/satellite_info.h \
    GPS/vehicle_gps_position.h \
#    Joystick/JoystickSDL.h \
    QGCQFileDialog.h \
#    QGCMessageBox.h \
#    RunGuard.h \
#    ViewWidgets/CustomCommandWidget.h \
#    ViewWidgets/CustomCommandWidgetController.h \
#    ViewWidgets/ViewWidgetController.h \
    comm/LogReplayLink.h \
#    comm/QGCFlightGearLink.h \
#    comm/QGCHilLink.h \
#    comm/QGCJSBSimLink.h \
#    comm/QGCXPlaneLink.h \
    uas/FileManager.h \
#    ui/HILDockWidget.h \
#    ui/MAVLinkDecoder.h \
#    MainWindow.h \
#    ui/MultiVehicleDockWidget.h \
#    ui/QGCHilConfiguration.h \
#    ui/QGCHilFlightGearConfiguration.h \
#    ui/QGCHilJSBSimConfiguration.h \
#    ui/QGCHilXPlaneConfiguration.h \
#    ui/QGCMAVLinkInspector.h \
    ui/QGCMAVLinkLogPlayer.h \
#    ui/QGCMapRCToParamDialog.h \
#    ui/QGCUASFileView.h \
#    ui/QGCUASFileViewMulti.h \
#    ui/linechart/ChartPlot.h \
#    ui/linechart/IncrementalPlot.h \
#    ui/linechart/LinechartPlot.h \
#    ui/linechart/LinechartWidget.h \
#    ui/linechart/Linecharts.h \
#    ui/linechart/ScrollZoomer.h \
#    ui/linechart/Scrollbar.h \
#    ui/uas/QGCUnconnectedInfoWidget.h \
}

iOSBuild {
    OBJECTIVE_SOURCES += \
        MobileScreenMgr.mm \
}

AndroidBuild {
    SOURCES += MobileScreenMgr.cc \
#    Joystick/JoystickAndroid.cc \
}

SOURCES += \
#    AnalyzeView/ExifParser.cc \
#    AnalyzeView/LogDownloadController.cc \
#    AnalyzeView/PX4LogParser.cc \
#    AnalyzeView/ULogParser.cc \
    Audio/AudioOutput.cc \
#    Camera/QGCCameraControl.cc \
#    Camera/QGCCameraIO.cc \
#    Camera/QGCCameraManager.cc \
    CmdLineOptParser.cc \
#    FlightDisplay/VideoManager.cc \
#    FlightMap/Widgets/ValuesWidgetController.cc \
#    FollowMe/FollowMe.cc \
#    Joystick/Joystick.cc \
#    Joystick/JoystickManager.cc \
    JsonHelper.cc \
#    KMLFileHelper.cc \
#    LogCompressor.cc \
#    MissionManager/CameraCalc.cc \
    MissionManager/CameraSection.cc \
#    MissionManager/CameraSpec.cc \
    MissionManager/ComplexMissionItem.cc \
#    MissionManager/CorridorScanComplexItem.cc \
    MissionManager/FixedWingLandingComplexItem.cc \
#    MissionManager/GeoFenceController.cc \
#    MissionManager/GeoFenceManager.cc \
#    MissionManager/KML.cc \
    MissionManager/MissionCommandList.cc \
    MissionManager/MissionCommandTree.cc \
    MissionManager/MissionCommandUIInfo.cc \
    MissionManager/MissionController.cc \
    MissionManager/MissionItem.cc \
    MissionManager/MissionManager.cc \
    MissionManager/MissionSettingsItem.cc \
    MissionManager/PlanElementController.cc \
    MissionManager/PlanManager.cc \
    MissionManager/PlanMasterController.cc \
#    MissionManager/QGCFenceCircle.cc \
#    MissionManager/QGCFencePolygon.cc \
#    MissionManager/QGCMapCircle.cc \
#    MissionManager/QGCMapPolygon.cc \
#    MissionManager/QGCMapPolyline.cc \
#    MissionManager/RallyPoint.cc \
#    MissionManager/RallyPointController.cc \
#    MissionManager/RallyPointManager.cc \
    MissionManager/SimpleMissionItem.cc \
    MissionManager/SpeedSection.cc \
#    MissionManager/StructureScanComplexItem.cc \
#    MissionManager/SurveyComplexItem.cc \
#    MissionManager/TransectStyleComplexItem.cc \
    MissionManager/VisualMissionItem.cc \
    postionManager/QGCPositionManager.cpp \
    postionManager/SimulatedPosition.cpp \
    QGC.cc \
#    QGCComboBox.cc \
#    QGCDockWidget.cc \
    QGCFileDownload.cc \
#    QGCGeo.cc \
    QGCLoggingCategory.cc \
#    QGCMapPalette.cc \
#    QGCPalette.cc \
    QGCQGeoCoordinate.cc \
#    QGCQmlWidgetHolder.cpp \
#    QGCQuickWidget.cc \
    QGCTemporaryFile.cc \
#    QmlControls/AppMessages.cc \
    Controls/CoordinateVector.cc \
#    QmlControls/EditPositionDialogController.cc \
#    QmlControls/ParameterEditorController.cc \
    Controls/QGCFileDialogController.cc \
#    QmlControls/QGCImageProvider.cc \
    Controls/QGroundControlQmlGlobal.cc \
    ui/SHGCDataModel/QmlObjectListModel.cc \
#    QmlControls/QGCGeoBoundingCube.cc \
#    QmlControls/RCChannelMonitorController.cc \
    Controls/ScreenToolsController.cc \
#    QtLocationPlugin/QMLControl/QGCMapEngineManager.cc \
    Settings/AppSettings.cpp \
    Settings/AutoConnectSettings.cc \
#    Settings/BrandImageSettings.cc \
    Settings/FlightMapSettings.cc \
    Settings/FlyViewSettings.cc \
    Settings/PlanViewSettings.cc \
    Settings/RTKSettings.cc \
    Settings/SettingsGroup.cpp \
    Settings/SettingsManager.cpp \
    Settings/UnitsSettings.cpp \
#    Settings/VideoSettings.cc \
#    ShapeFileHelper.cpp \
#    SHPFileHelper.cc \
#    Terrain/TerrainQuery.cc \
#    TerrainTile.cc\
    Vehicle/MAVLinkLogManager.cc \
#    VehicleSetup/JoystickConfigController.cc \
    comm/LinkConfiguration.cpp \
    comm/LinkInterface.cpp \
    comm/MAVLinkProtocol.cpp \
    comm/QGCMAVLink.cpp \
    comm/TCPLink.cc \
    comm/UDPLink.cpp \
    comm/UdpIODevice.cc \
    main.cpp \
    uas/UAS.cc \
    uas/UASMessageHandler.cc \
#    UTM.cpp \


DebugBuild {
SOURCES += \
#    comm/MockLink.cc \
#    comm/MockLinkFileServer.cc \
#    comm/MockLinkMissionItemHandler.cc \
}

!NoSerialBuild {
SOURCES += \
    comm/QGCSerialPortInfo.cpp \
    comm/SerialLink.cpp \
}

contains(DEFINES, QGC_ENABLE_BLUETOOTH) {
    SOURCES += \
#    comm/BluetoothLink.cc \
}

!MobileBuild {
SOURCES += \
#    AnalyzeView/GeoTagController.cc \
#    AnalyzeView/MavlinkConsoleController.cc \
    GPS/Drivers/src/gps_helper.cpp \
    GPS/Drivers/src/rtcm.cpp \
    GPS/Drivers/src/ashtech.cpp \
    GPS/Drivers/src/ubx.cpp \
    GPS/Drivers/src/sbf.cpp \
    GPS/GPSManager.cc \
    GPS/GPSProvider.cc \
    GPS/RTCM/RTCMMavlink.cc \
#    Joystick/JoystickSDL.cc \
    QGCQFileDialog.cc \
#    RunGuard.cc \
#    ViewWidgets/CustomCommandWidget.cc \
#    ViewWidgets/CustomCommandWidgetController.cc \
#    ViewWidgets/ViewWidgetController.cc \
    comm/LogReplayLink.cc \
#    comm/QGCFlightGearLink.cc \
#    comm/QGCJSBSimLink.cc \
#    comm/QGCXPlaneLink.cc \
    uas/FileManager.cc \
#    ui/HILDockWidget.cc \
#    ui/MAVLinkDecoder.cc \
#    MainWindow.cpp \
#    ui/MultiVehicleDockWidget.cc \
#    ui/QGCHilConfiguration.cc \
#    ui/QGCHilFlightGearConfiguration.cc \
#    ui/QGCHilJSBSimConfiguration.cc \
#    ui/QGCHilXPlaneConfiguration.cc \
#    ui/QGCMAVLinkInspector.cc \
    ui/QGCMAVLinkLogPlayer.cc \
#    ui/QGCMapRCToParamDialog.cpp \
#    ui/QGCUASFileView.cc \
#    ui/QGCUASFileViewMulti.cc \
#    ui/linechart/ChartPlot.cc \
#    ui/linechart/IncrementalPlot.cc \
#    ui/linechart/LinechartPlot.cc \
#    ui/linechart/LinechartWidget.cc \
#    ui/linechart/Linecharts.cc \
#    ui/linechart/ScrollZoomer.cc \
#    ui/linechart/Scrollbar.cc \
#    ui/uas/QGCUnconnectedInfoWidget.cc \
}

# Palette test widget in debug builds
DebugBuild {
#    HEADERS += QmlControls/QmlTestWidget.h
#    SOURCES += QmlControls/QmlTestWidget.cc
}

#
# Firmware Plugin Support
#

INCLUDEPATH += \
#    AutoPilotPlugins/Common \
    FirmwarePlugin \
    Vehicle \
    VehicleSetup \

HEADERS+= \
    AutoPilotPlugins/AutoPilotPlugin.h \
#    AutoPilotPlugins/Common/ESP8266Component.h \
#    AutoPilotPlugins/Common/ESP8266ComponentController.h \
#    AutoPilotPlugins/Common/MotorComponent.h \
#    AutoPilotPlugins/Common/RadioComponentController.h \
#    AutoPilotPlugins/Common/SyslinkComponent.h \
#    AutoPilotPlugins/Common/SyslinkComponentController.h \
    AutoPilotPlugins/Generic/GenericAutoPilotPlugin.h \
#    FirmwarePlugin/CameraMetaData.h \
    FirmwarePlugin/FirmwarePlugin.h \
    FirmwarePlugin/FirmwarePluginManager.h \
    Vehicle/ADSBVehicle.h \
    Vehicle/MultiVehicleManager.h \
    Vehicle/GPSRTKFactGroup.h \
    Vehicle/Vehicle.h \
    VehicleSetup/VehicleComponent.h \

!MobileBuild { !NoSerialBuild {
    HEADERS += \
#        VehicleSetup/Bootloader.h \
#        VehicleSetup/FirmwareImage.h \
#        VehicleSetup/FirmwareUpgradeController.h \
#        VehicleSetup/PX4FirmwareUpgradeThread.h \
}}

SOURCES += \
    AutoPilotPlugins/AutoPilotPlugin.cc \
#    AutoPilotPlugins/Common/ESP8266Component.cc \
#    AutoPilotPlugins/Common/ESP8266ComponentController.cc \
#    AutoPilotPlugins/Common/MotorComponent.cc \
#    AutoPilotPlugins/Common/RadioComponentController.cc \
#    AutoPilotPlugins/Common/SyslinkComponent.cc \
#    AutoPilotPlugins/Common/SyslinkComponentController.cc \
    AutoPilotPlugins/Generic/GenericAutoPilotPlugin.cc \
#    FirmwarePlugin/CameraMetaData.cc \
    FirmwarePlugin/FirmwarePlugin.cpp \
    FirmwarePlugin/FirmwarePluginManager.cpp \
    Vehicle/ADSBVehicle.cc \
    Vehicle/MultiVehicleManager.cpp \
    Vehicle/GPSRTKFactGroup.cc \
    Vehicle/Vehicle.cpp \
    VehicleSetup/VehicleComponent.cc \

!MobileBuild { !NoSerialBuild {
    SOURCES += \
#        VehicleSetup/Bootloader.cc \
#        VehicleSetup/FirmwareImage.cc \
#        VehicleSetup/FirmwareUpgradeController.cc \
#        VehicleSetup/PX4FirmwareUpgradeThread.cc \
}}

# ArduPilot FirmwarePlugin

APMFirmwarePlugin {
#    RESOURCES *= FirmwarePlugin/APM/APMResources.qrc

    INCLUDEPATH += \
        AutoPilotPlugins/APM \
        FirmwarePlugin/APM \

    HEADERS += \
        AutoPilotPlugins/APM/APMAirframeComponent.h \
        AutoPilotPlugins/APM/APMAirframeComponentAirframes.h \
        AutoPilotPlugins/APM/APMAirframeComponentController.h \
        AutoPilotPlugins/APM/APMAirframeLoader.h \
        AutoPilotPlugins/APM/APMAutoPilotPlugin.h \
#        AutoPilotPlugins/APM/APMCameraComponent.h \
        AutoPilotPlugins/APM/APMCompassCal.h \
#        AutoPilotPlugins/APM/APMFlightModesComponent.h \
#        AutoPilotPlugins/APM/APMFlightModesComponentController.h \
#        AutoPilotPlugins/APM/APMHeliComponent.h \
#        AutoPilotPlugins/APM/APMLightsComponent.h \
#        AutoPilotPlugins/APM/APMSubFrameComponent.h \
#        AutoPilotPlugins/APM/APMMotorComponent.h \
#        AutoPilotPlugins/APM/APMPowerComponent.h \
#        AutoPilotPlugins/APM/APMRadioComponent.h \
#        AutoPilotPlugins/APM/APMSafetyComponent.h \
        AutoPilotPlugins/APM/APMSensorsComponent.h \
        AutoPilotPlugins/APM/APMSensorsComponentController.h \
#        AutoPilotPlugins/APM/APMTuningComponent.h \
        FirmwarePlugin/APM/APMFirmwarePlugin.h \
        FirmwarePlugin/APM/APMParameterMetaData.h \
        FirmwarePlugin/APM/ArduCopterFirmwarePlugin.h \
        FirmwarePlugin/APM/ArduPlaneFirmwarePlugin.h \
        FirmwarePlugin/APM/ArduRoverFirmwarePlugin.h \
        FirmwarePlugin/APM/ArduSubFirmwarePlugin.h \

    SOURCES += \
        AutoPilotPlugins/APM/APMAirframeComponent.cc \
        AutoPilotPlugins/APM/APMAirframeComponentAirframes.cc \
        AutoPilotPlugins/APM/APMAirframeComponentController.cc \
        AutoPilotPlugins/APM/APMAirframeLoader.cc \
        AutoPilotPlugins/APM/APMAutoPilotPlugin.cc \
#        AutoPilotPlugins/APM/APMCameraComponent.cc \
        AutoPilotPlugins/APM/APMCompassCal.cc \
#        AutoPilotPlugins/APM/APMFlightModesComponent.cc \
#        AutoPilotPlugins/APM/APMFlightModesComponentController.cc \
#        AutoPilotPlugins/APM/APMHeliComponent.cc \
#        AutoPilotPlugins/APM/APMLightsComponent.cc \
#        AutoPilotPlugins/APM/APMSubFrameComponent.cc \
#        AutoPilotPlugins/APM/APMMotorComponent.cc \
#        AutoPilotPlugins/APM/APMPowerComponent.cc \
#        AutoPilotPlugins/APM/APMRadioComponent.cc \
#        AutoPilotPlugins/APM/APMSafetyComponent.cc \
        AutoPilotPlugins/APM/APMSensorsComponent.cc \
        AutoPilotPlugins/APM/APMSensorsComponentController.cc \
#        AutoPilotPlugins/APM/APMTuningComponent.cc \
        FirmwarePlugin/APM/APMFirmwarePlugin.cpp \
        FirmwarePlugin/APM/APMParameterMetaData.cpp \
        FirmwarePlugin/APM/ArduCopterFirmwarePlugin.cpp \
        FirmwarePlugin/APM/ArduPlaneFirmwarePlugin.cc \
        FirmwarePlugin/APM/ArduRoverFirmwarePlugin.cc \
        FirmwarePlugin/APM/ArduSubFirmwarePlugin.cc \
}

APMFirmwarePluginFactory {
    HEADERS   += FirmwarePlugin/APM/APMFirmwarePluginFactory.h
    SOURCES   += FirmwarePlugin/APM/APMFirmwarePluginFactory.cpp
}

# PX4 FirmwarePlugin

PX4FirmwarePlugin {
#    RESOURCES *= FirmwarePlugin/PX4/PX4Resources.qrc

    INCLUDEPATH += \
#        AutoPilotPlugins/PX4 \
        FirmwarePlugin/PX4 \

    HEADERS+= \
#        AutoPilotPlugins/PX4/AirframeComponent.h \
#        AutoPilotPlugins/PX4/AirframeComponentAirframes.h \
#        AutoPilotPlugins/PX4/AirframeComponentController.h \
#        AutoPilotPlugins/PX4/CameraComponent.h \
#        AutoPilotPlugins/PX4/FlightModesComponent.h \
#        AutoPilotPlugins/PX4/PX4AdvancedFlightModesController.h \
#        AutoPilotPlugins/PX4/PX4AirframeLoader.h \
#        AutoPilotPlugins/PX4/PX4AutoPilotPlugin.h \
#        AutoPilotPlugins/PX4/PX4RadioComponent.h \
#        AutoPilotPlugins/PX4/PX4SimpleFlightModesController.h \
#        AutoPilotPlugins/PX4/PX4TuningComponent.h \
#        AutoPilotPlugins/PX4/PowerComponent.h \
#        AutoPilotPlugins/PX4/PowerComponentController.h \
#        AutoPilotPlugins/PX4/SafetyComponent.h \
#        AutoPilotPlugins/PX4/SensorsComponent.h \
#        AutoPilotPlugins/PX4/SensorsComponentController.h \
        FirmwarePlugin/PX4/PX4FirmwarePlugin.h \
        FirmwarePlugin/PX4/PX4ParameterMetaData.h \

    SOURCES += \
#        AutoPilotPlugins/PX4/AirframeComponent.cc \
#        AutoPilotPlugins/PX4/AirframeComponentAirframes.cc \
#        AutoPilotPlugins/PX4/AirframeComponentController.cc \
#        AutoPilotPlugins/PX4/CameraComponent.cc \
#        AutoPilotPlugins/PX4/FlightModesComponent.cc \
#        AutoPilotPlugins/PX4/PX4AdvancedFlightModesController.cc \
#        AutoPilotPlugins/PX4/PX4AirframeLoader.cc \
#        AutoPilotPlugins/PX4/PX4AutoPilotPlugin.cc \
#        AutoPilotPlugins/PX4/PX4RadioComponent.cc \
#        AutoPilotPlugins/PX4/PX4SimpleFlightModesController.cc \
#        AutoPilotPlugins/PX4/PX4TuningComponent.cc \
#        AutoPilotPlugins/PX4/PowerComponent.cc \
#        AutoPilotPlugins/PX4/PowerComponentController.cc \
#        AutoPilotPlugins/PX4/SafetyComponent.cc \
#        AutoPilotPlugins/PX4/SensorsComponent.cc \
#        AutoPilotPlugins/PX4/SensorsComponentController.cc \
        FirmwarePlugin/PX4/PX4FirmwarePlugin.cpp \
        FirmwarePlugin/PX4/PX4ParameterMetaData.cpp \
}

PX4FirmwarePluginFactory {
    HEADERS   += FirmwarePlugin/PX4/PX4FirmwarePluginFactory.h
    SOURCES   += FirmwarePlugin/PX4/PX4FirmwarePluginFactory.cpp
}

# Fact System code

INCLUDEPATH += \
    FactSystem \
    FactSystem/FactControls \

HEADERS += \
    FactSystem/Fact.h \
    FactSystem/FactControls/FactPanelController.h \
    FactSystem/FactGroup.h \
    FactSystem/FactMetaData.h \
    FactSystem/FactSystem.h \
    FactSystem/FactValueSliderListModel.h \
    FactSystem/ParameterManager.h \
    FactSystem/SettingsFact.h \

SOURCES += \
    FactSystem/Fact.cpp \
    FactSystem/FactControls/FactPanelController.cc \
    FactSystem/FactGroup.cpp \
    FactSystem/FactMetaData.cpp \
    FactSystem/FactSystem.cpp \
    FactSystem/FactValueSliderListModel.cpp \
    FactSystem/ParameterManager.cpp \
    FactSystem/SettingsFact.cpp \

#-------------------------------------------------------------------------------------
# Taisync
#contains (DEFINES, QGC_GST_TAISYNC_ENABLED) {
#    INCLUDEPATH += \
#        Taisync

#    HEADERS += \
#        Taisync/TaisyncManager.h \
#        Taisync/TaisyncHandler.h \
#        Taisync/TaisyncSettings.h \

#    SOURCES += \
#        Taisync/TaisyncManager.cc \
#        Taisync/TaisyncHandler.cc \
#        Taisync/TaisyncSettings.cc \

#    iOSBuild | AndroidBuild {
#        HEADERS += \
#            Taisync/TaisyncTelemetry.h \
#            Taisync/TaisyncVideoReceiver.h \

#        SOURCES += \
#            Taisync/TaisyncTelemetry.cc \
#            Taisync/TaisyncVideoReceiver.cc \
#    }
#}

#-------------------------------------------------------------------------------------
# AirMap

#contains (DEFINES, QGC_AIRMAP_ENABLED) {

#    #-- These should be always enabled but not yet
#    INCLUDEPATH += \
#        AirspaceManagement

#    HEADERS += \
#        AirspaceManagement/AirspaceAdvisoryProvider.h \
#        AirspaceManagement/AirspaceFlightPlanProvider.h \
#        AirspaceManagement/AirspaceManager.h \
#        AirspaceManagement/AirspaceRestriction.h \
#        AirspaceManagement/AirspaceRestrictionProvider.h \
#        AirspaceManagement/AirspaceRulesetsProvider.h \
#        AirspaceManagement/AirspaceVehicleManager.h \
#        AirspaceManagement/AirspaceWeatherInfoProvider.h \

#    SOURCES += \
#        AirspaceManagement/AirspaceAdvisoryProvider.cc \
#        AirspaceManagement/AirspaceFlightPlanProvider.cc \
#        AirspaceManagement/AirspaceManager.cc \
#        AirspaceManagement/AirspaceRestriction.cc \
#        AirspaceManagement/AirspaceRestrictionProvider.cc \
#        AirspaceManagement/AirspaceRulesetsProvider.cc \
#        AirspaceManagement/AirspaceVehicleManager.cc \
#        AirspaceManagement/AirspaceWeatherInfoProvider.cc \

#    #-- This is the AirMap implementation of the above
#    RESOURCES += \
#        Airmap/airmap.qrc

#    INCLUDEPATH += \
#        Airmap

#    HEADERS += \
#        Airmap/AirMapAdvisoryManager.h \
#        Airmap/AirMapFlightManager.h \
#        Airmap/AirMapFlightPlanManager.h \
#        Airmap/AirMapManager.h \
#        Airmap/AirMapRestrictionManager.h \
#        Airmap/AirMapRulesetsManager.h \
#        Airmap/AirMapSettings.h \
#        Airmap/AirMapSharedState.h \
#        Airmap/AirMapTelemetry.h \
#        Airmap/AirMapTrafficMonitor.h \
#        Airmap/AirMapVehicleManager.h \
#        Airmap/AirMapWeatherInfoManager.h \
#        Airmap/LifetimeChecker.h \

#    SOURCES += \
#        Airmap/AirMapAdvisoryManager.cc \
#        Airmap/AirMapFlightManager.cc \
#        Airmap/AirMapFlightPlanManager.cc \
#        Airmap/AirMapManager.cc \
#        Airmap/AirMapRestrictionManager.cc \
#        Airmap/AirMapRulesetsManager.cc \
#        Airmap/AirMapSettings.cc \
#        Airmap/AirMapSharedState.cc \
#        Airmap/AirMapTelemetry.cc \
#        Airmap/AirMapTrafficMonitor.cc \
#        Airmap/AirMapVehicleManager.cc \
#        Airmap/AirMapWeatherInfoManager.cc \

#    #-- Do we have an API key?
#    exists(Airmap/Airmap_api_key.h) {
#        HEADERS += \
#            Airmap/Airmap_api_key.h
#        DEFINES += QGC_AIRMAP_KEY_AVAILABLE
#    }

#    include(Airmap/QJsonWebToken/qjsonwebtoken.pri)

#} else {
#    #-- Dummies
#    INCLUDEPATH += \
#        Airmap/dummy
#    RESOURCES += \
#        Airmap/dummy/airmap_dummy.qrc
#    HEADERS += \
#        Airmap/dummy/AirspaceManager.h
#    SOURCES += \
#        Airmap/dummy/AirspaceManager.cc
#}

#-------------------------------------------------------------------------------------
# Video Streaming

#INCLUDEPATH += \
#    VideoStreaming

#HEADERS += \
#    VideoStreaming/VideoItem.h \
#    VideoStreaming/VideoReceiver.h \
#    VideoStreaming/VideoStreaming.h \
#    VideoStreaming/VideoSurface.h \
#    VideoStreaming/VideoSurface_p.h \

#SOURCES += \
#    VideoStreaming/VideoItem.cc \
#    VideoStreaming/VideoReceiver.cc \
#    VideoStreaming/VideoStreaming.cc \
#    VideoStreaming/VideoSurface.cc \

#contains (CONFIG, DISABLE_VIDEOSTREAMING) {
#    message("Skipping support for video streaming (manual override from command line)")
## Otherwise the user can still disable this feature in the user_config.pri file.
#} else:exists(user_config.pri):infile(user_config.pri, DEFINES, DISABLE_VIDEOSTREAMING) {
#    message("Skipping support for video streaming (manual override from user_config.pri)")
#} else {
#    include(VideoStreaming/VideoStreaming.pri)
#}

#-------------------------------------------------------------------------------------
# Android

AndroidBuild {
    contains (CONFIG, DISABLE_BUILTIN_ANDROID) {
        message("Skipping builtin support for Android")
    } else {
        include(android.pri)
    }
}

#-------------------------------------------------------------------------------------
#
# Post link configuration


contains (CONFIG, QGC_DISABLE_BUILD_SETUP) {
    message("Disable standard build setup")
} else {
    include(QGCSetup.pri)
}

#
# Installer targets
#

include(QGCInstaller.pri)
