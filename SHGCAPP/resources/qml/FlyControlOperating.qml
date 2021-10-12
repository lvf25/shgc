import QtQuick 2.0
import QtQml 2.0
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4
import QtQuick.Layouts 1.12


import QGroundControl.Controllers       1.0
import QGroundControl                   1.0

Rectangle {
    id:root_FC
    property var fcsize
    property var masterController

    color: "#10000000"

    Image {
        id:flycontrolimage
        width: fcsize*2.4
        height: fcsize*3.3
        fillMode: Image.Stretch
        source: "qrc:/qml/resources/qml/bg_flycontrol.png"
    }

    ColumnLayout{
        anchors.centerIn: flycontrolimage
        spacing: -fcsize*0.03

    //下载飞控任务
        Button{
            id:downloadMission
            implicitHeight: fcsize*1.5
            implicitWidth: fcsize*2.4
            enabled: !masterController.offline && !masterController.syncInProgress

            Text{
                width: implicitWidth;
                height: implicitHeight;
                wrapMode: Text.WordWrap;
                font.bold: true;
                font.pixelSize: fcsize*0.3;
                text: downloadMission.hovered || downloadMission.activeFocus ? qsTr("<font color=\"#03A8F7\">下载飞控任务</font>") : qsTr("<font color=\"#FFFFFF\">下载飞控任务</font>")
                anchors.centerIn: parent;
            }

            onClicked: {
                masterController.loadFromVehicle()
                closeallretange()
            }

            style: ButtonStyle
            {
                id:_buttonstyle
                background: Rectangle{
                    id:_downloadmission
                    color:"transparent"
                }
            }
        }

        Rectangle{
            id:hline
            implicitWidth: fcsize*2.4
            implicitHeight: 2
//            anchors.top: downloadMission.bottom
//            anchors.horizontalCenter: parent.horizontalCenter
            color: "#D8D8D8"
        }

        Button{
            id:deleteMission
            implicitHeight: fcsize*1.5
            implicitWidth: fcsize*2.4
            Text{
                width: implicitWidth;
                height: implicitHeight;
                wrapMode: Text.WordWrap;
                font.bold: true;
                font.pixelSize: fcsize*0.3;
                text: deleteMission.hovered || deleteMission.activeFocus ? qsTr("<font color=\"#03A8F7\">删除飞控任务</font>") : qsTr("<font color=\"#FFFFFF\">删除飞控任务</font>")
                anchors.centerIn: parent;
                color: "#161616";
            }

            onClicked: {
                if(flycontroloperating.visible)
                {
                    closeallretange()

                }else{
                    closeallretange()
                    flycontroloperating.visible = true
                }
                deletdConfirm.visible = true
                deletdConfirm.activeFocus

            }

            style: ButtonStyle
            {
                id:_buttonstyles
                background: Rectangle{
                    id:_deletemission
                    color:"transparent"
                }
            }
        }


    }



}
