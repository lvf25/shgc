import QtQuick 2.0
import QtQuick.Controls 2.5
import QtQuick.Controls.Styles  1.4
import QtQuick.Layouts 1.12


Rectangle {
    id:_root
    property int waypointsize

    opacity: 0.8
    radius: waypointsize/2
    color: "#161616"
    Button{
        id: deletewaypoint
        property bool drawingwaypoint:false
        implicitWidth: waypointsize*0.8
        implicitHeight: waypointsize*0.8
        anchors.left: parent.left
        anchors.leftMargin: waypointsize/2
        anchors.verticalCenter: parent.verticalCenter
        background: Rectangle{
            id:deletewaypointre
            color:"transparent"
            Image {
                id:image01
                anchors.fill: parent
                fillMode: Image.PreserveAspectFit
                source: "qrc:/qml/resources/qml/delete.png"
                }
        }
        onClicked:
        {
            if(drawingwaypoint)
            {
                deletewaypointre.color = "transparent"
                drawingwaypoint = false
            }else
            {
                deletewaypointre.color = "#03A8F7"
                drawingwaypoint = true
            }
        }
    }

    Button{
        id: deleteall
        implicitWidth: waypointsize*0.8
        implicitHeight: waypointsize*0.8
        anchors.left: deletewaypoint.right
        anchors.leftMargin: waypointsize*0.8
        anchors.verticalCenter: parent.verticalCenter
        background: Rectangle{
            id:deleteallre
            color:"transparent"
            Image {
                anchors.fill: parent
                fillMode: Image.PreserveAspectFit
                source: "qrc:/qml/resources/qml/deleteAll.png"
                }
        }

        onPressed:
        {
            deleteallre.color = "#03A8F7"
        }

        onReleased: {
            deleteallre.color = "transparent"
        }

        onClicked:
        {

        }
    }

    Rectangle{
        id:rec
        implicitHeight: waypointsize*0.8
        implicitWidth: 1
        anchors.left: deleteall.right
        anchors.leftMargin: waypointsize*0.4
        anchors.verticalCenter: parent.verticalCenter
        color: "#D8D8D8"
    }

    Button{
        id: rollback
        implicitWidth: waypointsize*0.8
        implicitHeight: waypointsize*0.8
        anchors.left: rec.right
        anchors.leftMargin:  waypointsize*0.4
        anchors.verticalCenter: parent.verticalCenter
        background: Rectangle{
            id:rollbackre
            color:"transparent"
            Image {
                anchors.fill: parent
                fillMode: Image.PreserveAspectFit
                source: "qrc:/qml/resources/qml/WithdrawThePreviousStep.png"
                }
        }

        onPressed:
        {
            rollbackre.color = "#03A8F7"
        }

        onReleased: {
            rollbackre.color = "transparent"
        }

        onClicked:
        {
        }
    }

    Button{
        id: recover
        implicitWidth: waypointsize*0.8
        implicitHeight: waypointsize*0.8
        anchors.left: rollback.right
        anchors.leftMargin: waypointsize*0.8
        anchors.verticalCenter: parent.verticalCenter
        background: Rectangle{
            id:recoverre
            color:"transparent"
            Image {
                anchors.fill: parent
                fillMode: Image.PreserveAspectFit
                source: "qrc:/qml/resources/qml/WithdrawOfTheNextStep.png"
                }
        }

        onPressed:
        {
            recoverre.color = "#03A8F7"
        }

        onReleased: {
            recoverre.color = "transparent"
        }

        onClicked:
        {
        }
    }

}
