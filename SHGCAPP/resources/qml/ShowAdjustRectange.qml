import QtQuick 2.0
import QtQml 2.0
import QtQuick.Controls 2.5
import QtQuick.Controls.Styles  1.4
import QtQuick.Layouts 1.12

import QGroundControl.Controllers 1.0

Rectangle{
    id:adjust
    color: Qt.rgba(22/255,22/255,22/255,0.8)

    Rectangle {
        id:adjustroot
        color: "#F4F8F9"
        anchors.centerIn: parent
        implicitHeight: button_width*10
        implicitWidth: flightData_width*3.5

        RowLayout{
            id:adjustbutton
            anchors.top: parent.top
            anchors.topMargin: button_width*0.5
            anchors.horizontalCenter: parent.horizontalCenter

            Button{
                id:levelHorizonbutton
                implicitHeight: button_width*0.8
                implicitWidth: button_width*1.3
                checked: true
                Text
                {
                    id:levelHorizonbuttontext
                    width: implicitWidth;
                    height: implicitHeight;
                    wrapMode: Text.WordWrap;
                    font.bold: false;
                    font.pixelSize: button_width*0.2
                    text: levelHorizonbutton.activeFocus ? qsTr("<font color=\"#FFFFFF\">水平校准</font>") : qsTr("<font color=\"#03A8F7\">水平校准</font>")
                    anchors.centerIn: parent
                }


                background: Rectangle{
                    id:levelHorizonbuttonbuttonstyles
                    color:levelHorizonbutton.activeFocus ? "#03A8F7":"#FFFFFF"
                    radius: 2
                }

                onClicked: {
                    levelHorizon_content.visible = true
                    calibrateCompass_content.visible = false
                }
            }

            Button{
                id:calibrateCompass
                implicitHeight: button_width*0.8
                implicitWidth: button_width*1.3

                Text
                {
                    width: implicitWidth;
                    height: implicitHeight;
                    wrapMode: Text.WordWrap;
                    font.bold: false;
                    font.pixelSize: button_width*0.2
                    text: calibrateCompass.activeFocus ? qsTr("<font color=\"#FFFFFF\">罗盘校准</font>") : qsTr("<font color=\"#03A8F7\">罗盘校准</font>")
                    anchors.centerIn: parent
                }

                background: Rectangle{
                    color:calibrateCompass.activeFocus ? "#03A8F7":"#FFFFFF"
                    radius: 2
                }

                onClicked: {
                    levelHorizon_content.visible = false
                       calibrateCompass_content.visible = true
                }

            }

//            Button{
//                id:calibrateAccel

//                implicitHeight: button_width*0.8
//                implicitWidth: button_width*1.3

//                Text
//                {
//                    width: implicitWidth;
//                    height: implicitHeight;
//                    wrapMode: Text.WordWrap;
//                    font.bold: false;
//                    font.pixelSize: button_width*0.2
//                    text: calibrateAccel.activeFocus ? qsTr("<font color=\"#FFFFFF\">加速计校准</font>") : qsTr("<font color=\"#03A8F7\">加速计校准</font>")
//                    anchors.centerIn: parent
//                }


//                background: Rectangle{
//                    color:calibrateAccel.activeFocus ? "#03A8F7":"#FFFFFF"
//                    radius: 2
//                }

//                onClicked: {

//                }
//            }
        }

        Rectangle{
            id:levelHorizon_content
            color: Qt.rgba(22/255,22/255,22/255,0)
            anchors.centerIn: parent
            visible: true
            implicitHeight: adjustroot.height*0.4
            implicitWidth: adjustroot.width*0.8


            RowLayout{
                id:levelHorizon01
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.bottom: levelHorizonTextc.top
                anchors.bottomMargin: button_width*0.5
                Label{
                    id: levelHorizonText
                    font.pixelSize: button_width*0.3
                    text: qsTr("水平校准:")
                }

                ProgressBar {
                    id:             levelHorizonprogressBar
                    implicitHeight: button_width*0.5
                    implicitWidth: levelHorizon_content.width*0.5
                }
            }

            Label{
                id: levelHorizonTextc
                visible: false
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.bottom: startlevelHorizon.top
                anchors.bottomMargin: button_width*0.5
                font.pixelSize: button_width*0.3
                text: qsTr("正在校准...:")
                color: "#03A8F7"
            }

            Button{
                id:startlevelHorizon
                anchors.bottom: parent.bottom
                anchors.bottomMargin: button_width*0.5
                anchors.horizontalCenter: parent.horizontalCenter
                implicitHeight: button_width*0.8
                implicitWidth: button_width*1.5

                Text
                {
                    width: implicitWidth;
                    height: implicitHeight;
                    wrapMode: Text.WordWrap;
                    font.bold: false;
                    font.pixelSize: button_width*0.2
//                    text: qsTr("开始校准")
//                    color: "#FFFFFF"
                    text: startlevelHorizon.enabled ? qsTr("<font color=\"#FFFFFF\">开始校准</font>") : qsTr("<font color=\"#03A8F7\">开始校准</font>")
                    anchors.centerIn: parent
                }

                background: Rectangle{
                    color: "#03A8F7"
                    radius: button_width*0.4
                }

                onClicked: {
                     levelHorizonTextc.visible = true
                     controller.levelHorizon()
                }
            }
        }

        Rectangle{
            id:calibrateCompass_content
            color: Qt.rgba(22/255,22/255,22/255,0)
            anchors.centerIn: parent
            visible: false
            implicitHeight: adjustroot.height*0.4
            implicitWidth: adjustroot.width*0.8


            RowLayout{
                id:calibrateCompass01
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.bottom: calibrateCompass02.top
                anchors.bottomMargin: button_width*0.5
                Label{
                    id: calibrateCompassText01
                    font.pixelSize: button_width*0.3
                    text: qsTr("罗盘1:")
                }

                ProgressBar {
                    id:             calibrateCompassprogressBar1
                    implicitHeight: button_width*0.5
                    implicitWidth: levelHorizon_content.width*0.5
                }
            }

            RowLayout{
                id:calibrateCompass02
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.bottom: calibrateCompass03.top
                anchors.bottomMargin: button_width*0.5
                Label{
                    id: calibrateCompassText02
                    font.pixelSize: button_width*0.3
                    text: qsTr("罗盘2:")
                }

                ProgressBar {
                    id:             calibrateCompassprogressBar2
                    implicitHeight: button_width*0.5
                    implicitWidth: levelHorizon_content.width*0.5
                }
            }

            //
            RowLayout{
                id:calibrateCompass03
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.bottom: calibrateCompassTextc.top
                anchors.bottomMargin: button_width*0.5
                Label{
                    id: calibrateCompassText03
                    font.pixelSize: button_width*0.3
                    text: qsTr("罗盘3:")
                }

                ProgressBar {
                    id:             calibrateCompassprogressBar3
                    implicitHeight: button_width*0.5
                    implicitWidth: levelHorizon_content.width*0.5
                }
            }

            Label{
                id: calibrateCompassTextc
                visible: false
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.bottom: startcalibrateCompass.top
                anchors.bottomMargin: button_width*0.5
                font.pixelSize: button_width*0.3
                text: qsTr("正在校准...:")
                color: "#03A8F7"
            }

            Button{
                id:startcalibrateCompass
                anchors.bottom: parent.bottom
                anchors.bottomMargin: button_width*0.5
                anchors.horizontalCenter: parent.horizontalCenter
                implicitHeight: button_width*0.8
                implicitWidth: button_width*1.5

                Text
                {
                    width: implicitWidth;
                    height: implicitHeight;
                    wrapMode: Text.WordWrap;
                    font.bold: false;
                    font.pixelSize: button_width*0.2
//                    text: qsTr("开始校准")
//                    color: "#FFFFFF"
                    text: startcalibrateCompass.enabled ? qsTr("<font color=\"#FFFFFF\">开始校准</font>") : qsTr("<font color=\"#03A8F7\">开始校准</font>")
                    anchors.centerIn: parent
                }

                background: Rectangle{
                    color: "#03A8F7"
                    radius: button_width*0.4
                }

                onClicked: {
                    calibrateCompassTextc.visible = true

                   controller.calibrateCompass()
                }
            }
        }
    }

    APMSensorsComponentController{
        id: controller
        progressBar01:calibrateCompassprogressBar1
        progressBar02:calibrateCompassprogressBar2
        progressBar03:calibrateCompassprogressBar3
     }
}




