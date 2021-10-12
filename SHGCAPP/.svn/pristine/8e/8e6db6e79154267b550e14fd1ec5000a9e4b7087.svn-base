import QtQuick 2.0
import QtQml 2.0
import QtQuick.Controls         1.2
import QtQuick.Controls.Styles  1.4
import QtQuick.Layouts 1.12


Rectangle {
    property var buttonSize
    property var playController
    property bool measureClicked:false

    Button{
        id: openFile
        implicitWidth: buttonSize
        implicitHeight: buttonSize
        anchors.left: parent.left
        anchors.top: parent.top
        style: ButtonStyle{
            background: BorderImage{
                source:"qrc:/qml/resources/qml/folder.png"
                border.left: 0;border.right: 0;border.top: 0;border.bottom: 0
            }
        }
        onClicked:
        {
            playController.loadFromSelectedFile()
        }
    }

    Button{
        id: saveFile
        implicitWidth: buttonSize
        implicitHeight: buttonSize
        anchors.left: openFile.right
        anchors.leftMargin: buttonSize/3
        anchors.top: parent.top
        style: ButtonStyle{
            background: BorderImage{
                source:"qrc:/qml/resources/qml/storage.png"
                border.left: 0;border.right: 0;border.top: 0;border.bottom: 0
            }
        }
        onClicked:
        {
             playController.saveToSelectedFile()
        }
    }

    Button{
        id: actionmission
        implicitWidth: buttonSize
        implicitHeight: buttonSize
        anchors.left: saveFile.right
        anchors.leftMargin: buttonSize/3
        anchors.top: parent.top
        style: ButtonStyle{
            background: BorderImage{
                source:"qrc:/qml/resources/qml/FlightControl.png"
                border.left: 0;border.right: 0;border.top: 0;border.bottom: 0
            }
        }
        onClicked:
        {
            if(flycontroloperating.visible)
            {
                closeallretange()

            }else{
                closeallretange()
                flycontroloperating.visible = true
            }
        }
    }

    AnctionWayPoint{
        id:actionwaypoint
        implicitHeight: buttonSize
        implicitWidth: buttonSize*6.6
        anchors.left: actionmission.right
        anchors.leftMargin: buttonSize/3
        anchors.top: parent.top
        waypointsize:buttonSize
    }

    Button{
        id: conversion
        implicitWidth: buttonSize
        implicitHeight: buttonSize
        anchors.left: actionwaypoint.right
        anchors.leftMargin: buttonSize/3
        anchors.top: parent.top
        style: ButtonStyle{
            background: BorderImage{
                source:"qrc:/qml/resources/qml/conversion.png"
                border.left: 0;border.right: 0;border.top: 0;border.bottom: 0
            }
        }
        onClicked:
        {
            someObject.reverseOrder()
        }
    }

    Button{
        id: scale
        implicitWidth: buttonSize
        implicitHeight: buttonSize
        anchors.left: conversion.right
        anchors.leftMargin: buttonSize/3
        anchors.top: parent.top
        style: ButtonStyle{
            background: BorderImage{
                id:_scale
                source: measureClicked ? "qrc:/qml/resources/qml/scale_Blue.png" : "qrc:/qml/resources/qml/scale.png"
                border.left: 0;border.right: 0;border.top: 0;border.bottom: 0
            }
        }
        onClicked:
        {
            if(measureClicked){
                measureClicked = false
                someObject.cancleMeasuring()
            }else{
                measureClicked = true
                someObject.rulerMeasuring()
            }

        }
    }


}
