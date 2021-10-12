import QtQuick 2.0
import QtQml 2.0
import QtQuick.Controls         1.2
import QtQuick.Controls.Styles  1.4
import QtQuick.Layouts 1.12
import QGroundControl               1.0


Rectangle {

    property int flightpositionheight
    property var  _activeVehicle:        QGroundControl.multiVehicleManager.activeVehicle
    property real _lat:                   _activeVehicle ? _activeVehicle.gps.lat.value :0
    property real _lon:                   _activeVehicle ? _activeVehicle.gps.lon.value :0
    property real heading:                _activeVehicle ? _activeVehicle.heading.rawValue :0
    property bool   _vehicleArmed:          _activeVehicle ? _activeVehicle.armed  : false



    on_LatChanged: {
        someObject.latSignal(_lat)
        lat01.text = _lat.toFixed(6)
    }
    on_LonChanged: {
        someObject.lonSignal(_lon)
        lngshow.text = _lon.toFixed(6)
    }

    onHeadingChanged: {
        someObject.headingRecvDataToHtml(heading)
    }

    on_VehicleArmedChanged: {
        someObject.openLogAnctionToHtml(_vehicleArmed)
        someObject.setAircraftCenter(true)
    }

    id:latlonshow
    color: Qt.rgba(22/255,22/255,22/255,1)//(1)
    opacity: 0.8
    radius: 2

    RowLayout{
        anchors.centerIn: parent
        RowLayout{
            spacing: 0
            Label{
                id: lng
                horizontalAlignment:    Text.AlignLeft
                font.pointSize: flightpositionheight/5
                color: "#FFFFFF"
                font.bold: true
                text: qsTr("经度:")
            }
            Label{
                id: lngshow
                horizontalAlignment:    Text.AlignLeft
                font.pointSize:  flightpositionheight/5
                color: "#FFFFFF"
                font.bold: true
                text: qsTr(" ")
            }
        }


        RowLayout{
            spacing: 0
            Label{
                id: lat
                horizontalAlignment:    Text.AlignLeft
                font.pointSize: flightpositionheight/5
                color: "#FFFFFF"
                font.bold: true
                text: qsTr("纬度:")
            }
            Label{
                id: lat01
                horizontalAlignment:    Text.AlignLeft
                font.pointSize:  flightpositionheight/5
                color: "#FFFFFF"
                font.bold: true
                text: qsTr(" ")
            }
        }
    }



}
