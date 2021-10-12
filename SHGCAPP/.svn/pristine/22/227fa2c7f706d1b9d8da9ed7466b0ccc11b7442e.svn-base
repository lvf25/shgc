import QtQuick 2.0
import QtQuick.Controls 1.2
import QtQuick.Dialogs  1.2
import QtLocation       5.3
import QtPositioning    5.3
import QtQuick.Layouts  1.2
import QtQuick.Window   2.2
import QtLocation       5.3
import QtPositioning    5.3

import QGroundControl.Controllers       1.0
import QGroundControl                   1.0

Item {
    id:         _mapControl

    property var    _planMasterController:              masterController
    property var    _missionController:                 _planMasterController.missionController
    property var    _visualItems:                       _missionController.visualItems
    property var    _appSettings:                       QGroundControl.settingsManager.appSettings

    property int    getvisualItemscount:                _visualItems.count
    property var    _waypointPath:                      _missionController.waypointPath
    property var    takeoffCoordinate:                  _missionController.takeoffCoordinate
    property var    plannedHomePosition:                _missionController.plannedHomePosition

    property bool   showUploadRectange:                 !masterController.offline && !masterController.syncInProgress && _visualItems.count > 1
    property alias   fileDialog:                          fileDialog
    readonly property var   _activeVehicle:             QGroundControl.multiVehicleManager.activeVehicle

    property bool  loactionHomeItem:                     _missionController.locationHomeItem
    readonly property int   _decimalPlaces:             8


    on_WaypointPathChanged: {
        if(_waypointPath)
        {
            for(var i in _waypointPath)
            {
                if(_waypointPath[i].longitude == 0 && _waypointPath[i].latitude == 0)
                {
                    return;
                }

                someObject.drawLine(i,_waypointPath[i].longitude,_waypointPath[i].latitude);
            }
        }  
    }


    onShowUploadRectangeChanged: {
        console.log(showUploadRectange)
        if(showUploadRectange)
        {
            upload.visible = true
        }else{
            upload.visible = false
        }
    }

    function addSimpleMissionItem(lat,lng,row)
    {
        var coordinate = QtPositioning.coordinate(lat+0.0001, lng+0.0001, 0)
        coordinate.latitude = coordinate.latitude.toFixed(_decimalPlaces)
        coordinate.longitude = coordinate.longitude.toFixed(_decimalPlaces)
        coordinate.altitude = coordinate.altitude.toFixed(_decimalPlaces)
        var sequenceNumber = _missionController.insertSimpleMissionItem(coordinate, row)
//        _missionController.setCurrentPlanViewIndex(sequenceNumber, true)
    }

    function deleteSimpleMissionItem(row)
    {
        var sequenceNumber = _missionController.removeMissionItem(row)
    }

    function insertSimpleMissionItem(coordinate, index) {
//        console.log(coordinate.latitude)
        var sequenceNumber = _missionController.insertSimpleMissionItem(coordinate, _visualItems.count)
        _missionController.setCurrentPlanViewIndex(sequenceNumber, true)
    }

    function _insertSimpleMissionItem(coordinate)
    {
        insertSimpleMissionItem(coordinate,_missionController.visualItems.count)
    }

    function upSimpleMissionItem(row)
    {
        var sequenceNumber = _missionController.upMissionItem(row)
    }

    function downSimpleMissionItem(row)
    {
        var sequenceNumber = _missionController.downMissionItem(row)
    }




    PlanMasterController{
        id: masterController

        Component.onCompleted: {
            start(false /* flyView */)
            _missionController.setCurrentPlanViewIndex(0, true)
        }

        function saveToSelectedFile() {
            if (!readyForSaveSend()) {
//                waitingOnDataMessage()
                console.log("无法打开文件")
                return
            }
            _missionController.savefile(masterController.saveNameFilters)
        }

        //打开航点文件
        function loadFromSelectedFile() {
            _missionController.loadfile(masterController.loadNameFilters)
        }


        function upload() {
            if (!readyForSaveSend()) {
//                waitingOnDataMessage()
                return
            }
            if (_activeVehicle && _activeVehicle.armed && _activeVehicle.flightMode === _activeVehicle.missionFlightMode) {
//                root.showDialog(activeMissionUploadDialogComponent, qsTr("Plan Upload"), _qgcView.showDialogDefaultWidth, StandardButton.Cancel)
            } else {
                sendToVehicle()

            }
        }
    }

    Connections {
        target: _missionController

        onNewItemsFromVehicle: {
            if (_visualItems && _visualItems.count != 1) {
//                mapFitFunctions.fitMapViewportToMissionItems()
            }
            _missionController.setCurrentPlanViewIndex(0, true)
        }

        onLocationHomeItemChange:
        {
            someObject.setStartPoiCenter()
        }
    }

    SHGCFileDialog {
        id:             fileDialog
        qgcView:        _mapControl
        folder:         _appSettings.missionSavePath

        property bool planFiles: true    ///< true: working with plan files, false: working with kml file

        onAcceptedForSave: {
            if (planFiles) {
//                masterController.saveToFile(file)
//            } else {
//                masterController.saveToKml(file)
            }
//            masterController.saveToFile(file)
//            close()
        }

        onAcceptedForLoad: {
            if (planFiles) {
                masterController.loadFromFile(file)
////                masterController.fitViewportToItems()
                _missionController.setCurrentPlanViewIndex(0, true)
            }
        }

        onRejected: {
            fileDialog.visible = false
        }

    }




}
