import QtQuick 2.0
import QtQml 2.0
import QtQuick.Controls         1.2
import QtQuick.Controls.Styles  1.4
import QtQuick.Layouts 1.12



Rectangle {

    property var buttonSize
    property alias position: position
    property bool fv_WLGBottonClicked: false


//    RowLayout{
//        id:row01
//        spacing: buttonSize/3
//        anchors.fill: parent
        Button{
            id: logplayback
            implicitWidth: buttonSize
            implicitHeight: buttonSize
            anchors.right: flowVelocityMeasurement_waterlevelgauge.left
            anchors.rightMargin: buttonSize/3
            anchors.top: parent.top
            style: ButtonStyle{
                background: BorderImage{
                    source:"qrc:/qml/resources/qml/logShow.png"
                    border.left: 0;border.right: 0;border.top: 0;border.bottom: 0
                }
            }
            onClicked:
            {
//                root.logReplay();
                logplayer.visible = true;
            }
        }

        Button{
            id: flowVelocityMeasurement_waterlevelgauge
            implicitWidth: buttonSize
            implicitHeight: buttonSize
            anchors.right: position.left
            anchors.rightMargin: buttonSize*(5/3)
            anchors.top: parent.top
            style: ButtonStyle{
                background: BorderImage{
                    source:"qrc:/qml/resources/qml/measuringFlowVelocity.png"
                    border.left: 0;border.right: 0;border.top: 0;border.bottom: 0
                }
            }
            onClicked:
            {
                if(!fv_WLGBottonClicked)
                {
                    fv_WLGBottonClicked = true;
                    root.showFV_WLG();
                }
                else
                {
                    fv_WLGBottonClicked = false;
                    root.closeFV_WLG();
                }
            }
        }

        Button{
            id:compass01
            implicitWidth: buttonSize
            implicitHeight: buttonSize
            anchors.right: position.left
            anchors.rightMargin: buttonSize/3
            anchors.top: parent.top
            visible: true
            style: ButtonStyle{
                background: BorderImage{
                    source:"qrc:/qml/resources/qml/lock_unlock_north.png"
                    border.left: 0;border.right: 0;border.top: 0;border.bottom: 0
                }
            }
            onClicked:
            {
                compass01.visible = false
                compass02.visible = true
            }
        }

        Button{
            id:compass02
            implicitWidth: buttonSize
            implicitHeight: buttonSize
            anchors.right: position.left
            anchors.rightMargin: buttonSize/3
            anchors.top: parent.top
            visible: false
            style: ButtonStyle{
                background: BorderImage{
                    source:"qrc:/qml/resources/qml/north_unlock.png"
                    border.left: 0;border.right: 0;border.top: 0;border.bottom: 0
                }
            }
            onClicked:
            {
                compass01.visible = true
                compass02.visible = false
            }
        }

        Button{
            id:position
            implicitWidth: buttonSize
            implicitHeight: buttonSize
            anchors.right: displaystatus.left
            anchors.rightMargin: buttonSize/3
            anchors.top: parent.top
            style: ButtonStyle{
                background: BorderImage{
                    source:"qrc:/qml/resources/qml/positioning.png"
                    border.left: 0;border.right: 0;border.top: 0;border.bottom: 0
                }
            }
            onClicked:
            {
                if(positionshow.visible)
                {
                    closeallretange()

                }else
                {
                    closeallretange()
                    positionshow.visible = true
                }

            }
        }

        Button{
            id:  displaystatus
            implicitWidth: buttonSize
            implicitHeight: buttonSize
            anchors.right: mapswitching.left
            anchors.rightMargin: buttonSize/3
            anchors.top: parent.top
            style: ButtonStyle{
                background: BorderImage{
                    source:"qrc:/qml/resources/qml/waypoint.png"
                    border.left: 0;border.right: 0;border.top: 0;border.bottom: 0
                }
            }
            onClicked:
            {
                if(switchrectange.visible)
                {
                    closeallretange()
                }
                else
                {
                    closeallretange()
                    switchrectange.visible = true
                }


            }
        }

        Button{
            id:  mapswitching
            implicitWidth: buttonSize
            implicitHeight: buttonSize
            anchors.right: parent.right
            anchors.top: parent.top
            style: ButtonStyle{
                background: BorderImage{
                    source:"qrc:/qml/resources/qml/mapswitch.png"
                    border.left: 0;border.right: 0;border.top: 0;border.bottom: 0
                }
            }
            onClicked:
            {
                if(mapchoose.visible)
                {
                    closeallretange()

                }else{
                    closeallretange()
                    mapchoose.visible = true
                }
            }
        }
}


