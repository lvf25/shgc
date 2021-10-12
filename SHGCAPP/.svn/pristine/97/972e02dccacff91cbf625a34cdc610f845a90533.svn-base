import QtQuick 2.0
import QtQml 2.0
import QtQuick.Controls         1.3
import QtQuick.Controls.Styles  1.4
import QtQuick.Layouts 1.12
import QtGraphicalEffects 1.12

import QGroundControl           1.0


///@Power strip
Rectangle{
    id:powerdisplay
    color: "#282828"
    opacity: 0.8
    visible: _activeVehicle

    property var powerwidth
    property var powerheight
    property var powerfont
    property var  _activeVehicle:       QGroundControl.multiVehicleManager.activeVehicle

    property var _batPercentRemaining:  _activeVehicle ? _activeVehicle.battery.percentRemaining.value *0.01 : 0


    property real percentRemaining:      Math.abs(_batPercentRemaining)


    onPercentRemainingChanged: {
        powerstrip.value = percentRemaining
    }


    ProgressBar{
        id:powerstrip
        anchors.fill: parent
        implicitHeight: parent.height
        implicitWidth: parent.width
        value: percentRemaining
        style: ProgressBarStyle{
            id:powerstripstyle
            background: Rectangle{
                border.width: 1
                border.color: "#282828"
                color: "#10000000"

            }

            progress: Rectangle{
                color: percentRemaining<0.2?"#FF0000":(percentRemaining<0.6?"#FE5723":"#019C07")
                radius: powerheight*0.35
                clip: true
                Text {
                    anchors.right: parent.right;
                    anchors.rightMargin: 10
                    anchors.verticalCenter: parent.verticalCenter
                    text: Math.round(percentRemaining*100) + "%"
                    font.pixelSize: powerfont
                    font.bold: true
                    color: "#FFFFFF"
                }


            }

            panel: Item{
                implicitHeight: powerstrip.height-1
                implicitWidth: powerstrip.width-1

                Loader{
                    anchors.fill: parent
                    sourceComponent: background
                }

                Loader{
                    anchors.top: parent.top
                    anchors.left: parent.left
                    anchors.bottom: parent.bottom
                    anchors.margins: 1.5
                    width: (parent.width-2)*percentRemaining
                    sourceComponent: powerstripstyle.progress
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
//            if(_activeVehicle){
//                powerdisplay.visible = true
//            }else{
//                powerdisplay.visible = false
//            }



//        }

//    }
}

