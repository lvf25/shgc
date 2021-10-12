import QtQuick 2.0
import QtQuick.Controls         1.2
import QtQuick.Controls.Styles  1.4
import QtQuick.Dialogs          1.2
import QtLocation               5.3
import QtPositioning            5.3
import QtQuick.Layouts          1.2

import QGroundControl                           1.0
import QGroundControl.QGCCorePlugin 1.0
import QGroundControl.QGCOptions 1.0
import QGroundControl.Vehicle   1.0


Item {
    id: _root
    property var confirmDialog
    property var missionController
    property var _actionData

    readonly property string emergencyStopTitle:            qsTr("EMERGENCY STOP")
    readonly property string armTitle:                      qsTr("Arm")
    readonly property string disarmTitle:                   qsTr("Disarm")
    readonly property string rtlTitle:                      qsTr("RTL")
    readonly property string takeoffTitle:                  qsTr("Takeoff")
    readonly property string landTitle:                     qsTr("Land")
    readonly property string startMissionTitle:             qsTr("Start Mission")
    readonly property string mvStartMissionTitle:           qsTr("Start Mission (MV)")
    readonly property string continueMissionTitle:          qsTr("Continue Mission")
    readonly property string resumeMissionUploadFailTitle:  qsTr("Resume FAILED")
    readonly property string pauseTitle:                    qsTr("Pause")
    readonly property string mvPauseTitle:                  qsTr("Pause (MV)")
    readonly property string changeAltTitle:                qsTr("Change Altitude")
    readonly property string orbitTitle:                    qsTr("Orbit")
    readonly property string landAbortTitle:                qsTr("Land Abort")
    readonly property string setWaypointTitle:              qsTr("Set Waypoint")
    readonly property string gotoTitle:                     qsTr("Goto Location")
    readonly property string vtolTransitionTitle:           qsTr("VTOL Transition")

    readonly property string armMessage:                        qsTr("Arm the vehicle.")
    readonly property string disarmMessage:                     qsTr("Disarm the vehicle")
    readonly property string emergencyStopMessage:              qsTr("WARNING: THIS WILL STOP ALL MOTORS. IF VEHICLE IS CURRENTLY IN THE AIR IT WILL CRASH.")
    readonly property string takeoffMessage:                    qsTr("Takeoff from ground and hold position.")
    readonly property string startMissionMessage:               qsTr("Takeoff from ground and start the current mission.")
    readonly property string continueMissionMessage:            qsTr("Continue the mission from the current waypoint.")
    readonly property string resumeMissionUploadFailMessage:    qsTr("Upload of resume mission failed. Confirm to retry upload")
    readonly property string landMessage:                       qsTr("Land the vehicle at the current position.")
    readonly property string rtlMessage:                        qsTr("Return to the home position of the vehicle.")
    readonly property string changeAltMessage:                  qsTr("Change the altitude of the vehicle up or down.")
    readonly property string gotoMessage:                       qsTr("Move the vehicle to the specified location.")
             property string setWaypointMessage:                qsTr("Adjust current waypoint to %1.").arg(_actionData)
    readonly property string orbitMessage:                      qsTr("Orbit the vehicle around the specified location.")
    readonly property string landAbortMessage:                  qsTr("Abort the landing sequence.")
    readonly property string pauseMessage:                      qsTr("Pause the vehicle at it's current position, adjusting altitude up or down as needed.")
    readonly property string mvPauseMessage:                    qsTr("Pause all vehicles at their current position.")
    readonly property string vtolTransitionFwdMessage:          qsTr("Transition VTOL to fixed wing flight.")
    readonly property string vtolTransitionMRMessage:           qsTr("Transition VTOL to multi-rotor flight.")

    readonly property int actionRTL:                        1
    readonly property int actionLand:                       2
    readonly property int actionTakeoff:                    3
    readonly property int actionArm:                        4
    readonly property int actionDisarm:                     5
    readonly property int actionEmergencyStop:              6
    readonly property int actionChangeAlt:                  7
    readonly property int actionGoto:                       8
    readonly property int actionSetWaypoint:                9
    readonly property int actionOrbit:                      10
    readonly property int actionLandAbort:                  11
    readonly property int actionStartMission:               12
    readonly property int actionContinueMission:            13
    readonly property int actionResumeMission:              14
    readonly property int _actionUnused:                    15
    readonly property int actionResumeMissionUploadFail:    16
    readonly property int actionPause:                      17
    readonly property int actionMVPause:                    18
    readonly property int actionMVStartMission:             19
    readonly property int actionVtolTransitionToFwdFlight:  20
    readonly property int actionVtolTransitionToMRFlight:   21

    property bool showEmergenyStop:     _guidedActionsEnabled && !_hideEmergenyStop && _vehicleArmed && _vehicleFlying
    property bool showArm:              _guidedActionsEnabled && !_vehicleArmed
    property bool showDisarm:           _guidedActionsEnabled && _vehicleArmed && !_vehicleFlying
    property bool showRTL:              _guidedActionsEnabled && _vehicleArmed && _activeVehicle.guidedModeSupported && _vehicleFlying && !_vehicleInRTLMode
    property bool showTakeoff:          _guidedActionsEnabled && _activeVehicle.takeoffVehicleSupported && !_vehicleFlying
    property bool showLand:             _guidedActionsEnabled && _activeVehicle.guidedModeSupported && _vehicleArmed && !_activeVehicle.fixedWing && !_vehicleInLandMode
    property bool showStartMission:     _guidedActionsEnabled && _missionAvailable && !_missionActive && !_vehicleFlying
    property bool showContinueMission:  _guidedActionsEnabled && _missionAvailable && !_missionActive && _vehicleArmed && _vehicleFlying && (_currentMissionIndex < _missionItemCount - 1)
    property bool showPause:            _guidedActionsEnabled && _vehicleArmed && _activeVehicle.pauseVehicleSupported && _vehicleFlying && !_vehiclePaused
    property bool showChangeAlt:        _guidedActionsEnabled && _vehicleFlying && _activeVehicle.guidedModeSupported && _vehicleArmed && !_missionActive
    property bool showOrbit:            _guidedActionsEnabled && _vehicleFlying && _activeVehicle.orbitModeSupported && !_missionActive
    property bool showLandAbort:        _guidedActionsEnabled && _vehicleFlying && _activeVehicle.fixedWing && _vehicleLanding
    property bool showGotoLocation:     _guidedActionsEnabled && _vehicleFlying

    property bool   showResumeMission:    _activeVehicle && !_vehicleArmed && _vehicleWasFlying && _missionAvailable && _resumeMissionIndex > 0 && (_resumeMissionIndex < _missionItemCount - 2)

    property var    _corePlugin:            QGroundControl.corePlugin
    property bool   _guidedActionsEnabled:  _activeVehicle
    property var    _activeVehicle:         QGroundControl.multiVehicleManager.activeVehicle
    property string _flightMode:            _activeVehicle ? _activeVehicle.flightMode : ""
    property bool   _missionAvailable:      missionController.containsItems
    property bool   _missionActive:         _activeVehicle ? _vehicleArmed && (_vehicleInLandMode || _vehicleInRTLMode || _vehicleInMissionMode) : false
    property bool   _vehicleArmed:          _activeVehicle ? _activeVehicle.armed  : false
    property bool   _vehicleFlying:         _activeVehicle ? _activeVehicle.flying  : false
    property bool   _vehicleLanding:        _activeVehicle ? _activeVehicle.landing  : false

    property bool   _vehiclePaused:         false
    property bool   _vehicleInMissionMode:  false
    property bool   _vehicleInRTLMode:      false
    property bool   _vehicleInLandMode:     false

    property int    _missionItemCount:      missionController.missionItemCount
    property int    _currentMissionIndex:   missionController.currentMissionIndex
    property int    _resumeMissionIndex:    missionController.resumeMissionIndex
    property bool   _hideEmergenyStop:      !QGroundControl.corePlugin.options.guidedBarShowEmergencyStop
    property bool   _hideOrbit:             !QGroundControl.corePlugin.options.guidedBarShowOrbit
    property bool   _vehicleWasFlying:      false
    property bool   _rcRSSIAvailable:       _activeVehicle ? _activeVehicle.rcRSSI > 0 && _activeVehicle.rcRSSI <= 100 : false


    // You can turn on log output for GuidedActionsController by turning on GuidedActionsControllerLog category
    property bool __guidedModeSupported:    _activeVehicle ? _activeVehicle.guidedModeSupported : false
    property bool __pauseVehicleSupported:  _activeVehicle ? _activeVehicle.pauseVehicleSupported : false
    property bool __flightMode:             _flightMode

    on_FlightModeChanged: {
        _vehiclePaused =        _activeVehicle ? _flightMode === _activeVehicle.pauseFlightMode : false
        _vehicleInRTLMode =     _activeVehicle ? _flightMode === _activeVehicle.rtlFlightMode : false
        _vehicleInLandMode =    _activeVehicle ? _flightMode === _activeVehicle.landFlightMode : false
        _vehicleInMissionMode = _activeVehicle ? _flightMode === _activeVehicle.missionFlightMode : false // Must be last to get correct signalling for showStartMission popups
    }

    // Called when an action is about to be executed in order to confirm
    function confirmAction(actionCode, actionData, mapIndicator) {
        var showImmediate = true
        closeAll()
        confirmDialog.action = actionCode
        confirmDialog.actionData = actionData
        confirmDialog.hideTrigger = true
        confirmDialog.mapIndicator = mapIndicator
        _actionData = actionData
        switch (actionCode) {
        case actionArm:
            if (_vehicleFlying || !_guidedActionsEnabled) {
                return
            }
            confirmDialog.title = armTitle
            confirmDialog.message = armMessage
            confirmDialog.hideTrigger = Qt.binding(function() { return !showArm })
            break;
        case actionDisarm:
            if (_vehicleFlying) {
                return
            }
            confirmDialog.title = disarmTitle
            confirmDialog.message = disarmMessage
            confirmDialog.hideTrigger = Qt.binding(function() { return !showDisarm })
            break;
        case actionEmergencyStop:
            confirmDialog.title = emergencyStopTitle
            confirmDialog.message = emergencyStopMessage
            confirmDialog.hideTrigger = Qt.binding(function() { return !showEmergenyStop })
            break;
        case actionTakeoff:
            confirmDialog.title = takeoffTitle
            confirmDialog.message = takeoffMessage
            confirmDialog.hideTrigger = Qt.binding(function() { return !showTakeoff })
//            altitudeSlider.setToMinimumTakeoff()
//            altitudeSlider.visible = true
//            confirmDialog.visible = true
            break;
        case actionStartMission:
            showImmediate = false
            confirmDialog.title = startMissionTitle
            confirmDialog.message = startMissionMessage
            confirmDialog.hideTrigger = Qt.binding(function() { return !showStartMission })
            break;
        case actionMVStartMission:
            confirmDialog.title = mvStartMissionTitle
            confirmDialog.message = startMissionMessage
            confirmDialog.hideTrigger = true
            break;
        case actionContinueMission:
            showImmediate = false
            confirmDialog.title = continueMissionTitle
            confirmDialog.message = continueMissionMessage
            confirmDialog.hideTrigger = Qt.binding(function() { return !showContinueMission })
            break;
        case actionResumeMission:
            // Resume Mission is handled in mission end dialog
            return
        case actionResumeMissionUploadFail:
            confirmDialog.title = resumeMissionUploadFailTitle
            confirmDialog.message = resumeMissionUploadFailMessage
            confirmDialog.hideTrigger = Qt.binding(function() { return !showResumeMission })
            break;
        case actionLand:
            confirmDialog.title = landTitle
            confirmDialog.message = landMessage
            confirmDialog.hideTrigger = Qt.binding(function() { return !showLand })
            break;
        case actionRTL:
            confirmDialog.title = rtlTitle
            confirmDialog.message = rtlMessage
            confirmDialog.hideTrigger = Qt.binding(function() { return !showRTL })
            break;
        case actionChangeAlt:
            confirmDialog.title = changeAltTitle
            confirmDialog.message = changeAltMessage
            confirmDialog.hideTrigger = Qt.binding(function() { return !showChangeAlt })
            altitudeSlider.reset()
            altitudeSlider.visible = true
            break;
        case actionGoto:
            confirmDialog.title = gotoTitle
            confirmDialog.message = gotoMessage
            confirmDialog.hideTrigger = Qt.binding(function() { return !showGotoLocation })
            break;
        case actionSetWaypoint:
            confirmDialog.title = setWaypointTitle
            confirmDialog.message = setWaypointMessage
            break;
        case actionOrbit:
            confirmDialog.title = orbitTitle
            confirmDialog.message = orbitMessage
            confirmDialog.hideTrigger = Qt.binding(function() { return !showOrbit })
            altitudeSlider.reset()
            altitudeSlider.visible = true
            break;
        case actionLandAbort:
            confirmDialog.title = landAbortTitle
            confirmDialog.message = landAbortMessage
            confirmDialog.hideTrigger = Qt.binding(function() { return !showLandAbort })
            break;
        case actionPause:
            confirmDialog.title = pauseTitle
            confirmDialog.message = pauseMessage
            confirmDialog.hideTrigger = Qt.binding(function() { return !showPause })
//            altitudeSlider.reset()
//            altitudeSlider.visible = true
            break;
        case actionMVPause:
            confirmDialog.title = mvPauseTitle
            confirmDialog.message = mvPauseMessage
            confirmDialog.hideTrigger = true
            break;
        case actionVtolTransitionToFwdFlight:
            confirmDialog.title = vtolTransitionTitle
            confirmDialog.message = vtolTransitionFwdMessage
            confirmDialog.hideTrigger = true
            break
        case actionVtolTransitionToMRFlight:
            confirmDialog.title = vtolTransitionTitle
            confirmDialog.message = vtolTransitionMRMessage
            confirmDialog.hideTrigger = true
            break
        default:
            console.warn("Unknown actionCode", actionCode)
            return
        }
        confirmDialog.show(showImmediate)
    }



    function executeAction(actionCode, actionData, actionAltitudeChange) {
        var i;
        var rgVehicle;
        switch (actionCode) {
        case actionRTL:
            _activeVehicle.guidedModeRTL()
            break
        case actionLand:
            _activeVehicle.guidedModeLand()
            break
        case actionTakeoff:
            _activeVehicle.guidedModeTakeoff(actionAltitudeChange)
            break
        case actionResumeMission:
        case actionResumeMissionUploadFail:
            missionController.resumeMission(missionController.resumeMissionIndex)
            break
        case actionStartMission:
        case actionContinueMission:
            _activeVehicle.startMission()
            break
        case actionMVStartMission:
            rgVehicle = QGroundControl.multiVehicleManager.vehicles
            for (i = 0; i < rgVehicle.count; i++) {
                rgVehicle.get(i).startMission()
            }
            break
        case actionArm:
            _activeVehicle.armed = true
            break
        case actionDisarm:
            _activeVehicle.armed = false
            break
        case actionEmergencyStop:
            _activeVehicle.emergencyStop()
            break
        case actionChangeAlt:
            _activeVehicle.guidedModeChangeAltitude(actionAltitudeChange)
            break
        case actionGoto:
            _activeVehicle.guidedModeGotoLocation(actionData)
            break
        case actionSetWaypoint:
            _activeVehicle.setCurrentMissionSequence(actionData)
            break
        case actionOrbit:
            _activeVehicle.guidedModeOrbit(orbitMapCircle.center, orbitMapCircle.radius() * (orbitMapCircle.clockwiseRotation ? 1 : -1), _activeVehicle.altitudeAMSL.rawValue + actionAltitudeChange)
            break
        case actionLandAbort:
            _activeVehicle.abortLanding(50)     // hardcoded value for climbOutAltitude that is currently ignored
            break
        case actionPause:
            _activeVehicle.pauseVehicle()
            _activeVehicle.guidedModeChangeAltitude(actionAltitudeChange)
            break
        case actionMVPause:
            rgVehicle = QGroundControl.multiVehicleManager.vehicles
            for (i = 0; i < rgVehicle.count; i++) {
                rgVehicle.get(i).pauseVehicle()
            }
            break
        case actionVtolTransitionToFwdFlight:
            _activeVehicle.vtolInFwdFlight = true
            break
        case actionVtolTransitionToMRFlight:
            _activeVehicle.vtolInFwdFlight = false
            break
        default:
            console.warn(qsTr("Internal error: unknown actionCode"), actionCode)
            break
        }
    }







}