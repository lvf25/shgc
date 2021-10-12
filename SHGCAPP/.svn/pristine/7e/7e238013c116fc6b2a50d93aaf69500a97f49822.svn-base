import QtQuick 2.0
import QtQml 2.0
import QtQuick.Controls         1.2
import QtQuick.Controls.Styles  1.4
import QtQuick.Layouts 1.12
import QGroundControl.FactSystem    1.0
import QGroundControl.Vehicle       1.0

import QGroundControl               1.0

Rectangle{
    property var fontSize

    property var  _activeVehicle:       QGroundControl.multiVehicleManager.activeVehicle
    property real airSpeed:             _activeVehicle ? _activeVehicle.airSpeed.rawValue :0
    property real groundSpeed:          _activeVehicle ?_activeVehicle.groundSpeed.rawValue:0
    property real distanceToHome:       _activeVehicle ?_activeVehicle.distanceToHome.rawValue:0
    property real distanceToGCS:        _activeVehicle ?_activeVehicle.distanceToGCS.rawValue:0
    property var currentDate:           _activeVehicle ? _activeVehicle.clock.currentDate.value : "00-00-00"
    property var currentTime:           _activeVehicle ? _activeVehicle.clock.currentTime.value : "00:00:00"
    property var flightTime:            _activeVehicle ? _activeVehicle.flightTime.valueString: "00:00:00"


//    on_ActiveVehicleChanged: {
//        if(dataTimer.running)
//        {
//            dataTimer.restart()
//        }else
//        {
//            dataTimer.start()
//        }
//    }

//    label_3.text = flightTime
//    label_4.text = currentTime
//    label_7.text = airSpeed.toFixed(2) + "m/s"
//    label_8.text = groundSpeed.toFixed(2)+ "m/s"
//    label_11.text = distanceToHome.toFixed(2)+ "m"
//    label_12.text = distanceToGCS.toFixed(2) + "m"

    onCurrentDateChanged: {
        label_2.text = currentDate
    }

    onFlightTimeChanged: {
        label_3.text = flightTime
    }

    onCurrentTimeChanged: {
        label_4.text = currentTime
    }

    onAirSpeedChanged: {
        label_7.text = airSpeed.toFixed(2) + "m/s"
    }

    onGroundSpeedChanged: {
        label_8.text = groundSpeed.toFixed(2)+ "m/s"
    }

    onDistanceToGCSChanged: {
        label_12.text = distanceToGCS.toFixed(2) + "m"
    }

    onDistanceToHomeChanged: {
        label_11.text = distanceToHome.toFixed(2)+ "m"

    }



    RowLayout{
        id:dataShow
//        spacing: fontSize
        anchors.fill: parent
        RowLayout{
            spacing: fontSize/2
            ColumnLayout{
                spacing: button_width/2
                Label{
                    id:label_1
                    horizontalAlignment:    Text.AlignLeft
                    font.pointSize:  fontSize*1.2
                    color: "#FFFFFF"
                    font.bold: true
                    text: qsTr("飞行时间")
                }
                Label{
                    id:label_2
                    horizontalAlignment:    Text.AlignLeft
                    font.pointSize:  fontSize*1.2
                    color: "#FFFFFF"
                    font.bold: true
                    text: currentDate

                }
            }
            ColumnLayout{
                spacing: button_width/2
                Label{
                    id:label_3
                    horizontalAlignment:    Text.AlignLeft
                    font.pointSize: fontSize *1.2
                    font.bold: true
                    color: "#FFFFFF"
                    text: flightTime
                }
                Label{
                    id:label_4
                    horizontalAlignment:    Text.AlignLeft
                    font.pointSize:  fontSize*1.2
                    color: "#FFFFFF"
                    font.bold: true
                    text: currentTime
                }
            }
        }

        RowLayout{
            spacing: fontSize/2
            ColumnLayout{
                spacing: button_width/2
                Label{
                    id:label_5
                    horizontalAlignment:    Text.AlignLeft
                    font.pointSize:  fontSize*1.2
                    color: "#FFFFFF"
                    font.bold: true
                    text: qsTr("空速")
                }
                Label{
                    id:label_6
                    horizontalAlignment:    Text.AlignLeft
                    font.pointSize:  fontSize*1.2
                    color: "#FFFFFF"
                    font.bold: true
                    text: qsTr("地速")
                }
            }

            ColumnLayout{
                spacing: button_width/2
                Label{
                    id:label_7
                    horizontalAlignment:    Text.AlignLeft
                    font.pointSize:  fontSize*1.2
                    color: "#FFFFFF"
                    font.bold: true
                    text: airSpeed.toFixed(2) + "m/s"
                }
                Label{
                    id:label_8
                    horizontalAlignment:    Text.AlignLeft
                    font.pointSize:  fontSize*1.2
                    color: "#FFFFFF"
                    font.bold: true
                    text: groundSpeed.toFixed(2)+ "m/s"
                }
            }
        }


        RowLayout{
            spacing: button_width/2
            ColumnLayout{
                spacing: button_width/2
                Label{
                    id:label_9
                    horizontalAlignment:    Text.AlignLeft
                    font.pointSize:  fontSize*1.2
                    font.bold: true
                    font.family: "DIN-Bold"
                    color: "#FFFFFF"
                    text: qsTr("距返航点")

                }
                Label{
                    id:label_10
                    horizontalAlignment:    Text.AlignLeft
                    font.pointSize:  fontSize*1.2
                    color: "#FFFFFF"
                    font.bold: true
                    text: qsTr("距起飞点")
                }
            }

            ColumnLayout{
                spacing: button_width/2
                Label{
                    id:label_11
                    horizontalAlignment:    Text.AlignLeft
                    font.pointSize:  fontSize*1.2
                    font.bold: true
                    color: "#FFFFFF"
                    text:  distanceToHome.toFixed(2)+ "m"
                }
                Label{

                    id:label_12
                    horizontalAlignment:    Text.AlignLeft
                    font.pointSize:  fontSize*1.2
                    font.bold: true
                    color: "#FFFFFF"
                    text: distanceToGCS.toFixed(2) + "m"
                }
            }
        }
    }

//    Timer {
//        id:         dataTimer
//        interval:   100
//        running:    false
//        repeat:     true
//        onTriggered:{
//            label_2.text = currentDate
//            label_3.text = flightTime
//            label_4.text = currentTime
//            label_7.text = airSpeed.toFixed(2) + "m/s"
//            label_8.text = groundSpeed.toFixed(2)+ "m/s"
//            label_11.text = distanceToHome.toFixed(2)+ "m"
//            label_12.text = distanceToGCS.toFixed(2) + "m"

////            if(!_activeVehicle)
////            {
////                dataTimer.stop()
////            }

//        }
//    }

}
