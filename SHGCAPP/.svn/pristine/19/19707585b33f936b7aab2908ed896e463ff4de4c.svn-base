import QtQuick 2.0
import QtQuick.Controls 2.5
import QtQuick.Controls.Styles  1.4
import QtQuick.Layouts 1.12


Rectangle {
    id: _root
    property int widthsize

    opacity: 0.8
    radius: widthsize/2
    color: "#161616"


    Button{
        id: drawwaypoint
        property bool drawingwaypoint:false
        implicitWidth: widthsize
        implicitHeight: widthsize*1.5
        visible:true
        anchors.top: parent.top
        anchors.horizontalCenter: parent.horizontalCenter
        background: Rectangle{
            id:drawwaypointre
            color:"transparent"
            Image {
                id:image01
                anchors.fill: parent
                fillMode: Image.PreserveAspectFit
                source: "qrc:/qml/resources/qml/Clicked.png"
                }
        }
        onClicked:
        {

            if(drawingwaypoint)
            {
                drawingwaypoint = false
                image01.source = "qrc:/qml/resources/qml/Clicked.png"
                image01.fillMode = Image.PreserveAspectFit
                someObject.removePointFun()
            }else
            {
                drawingwaypoint = true
                image01.source = "qrc:/qml/resources/qml/clicked_Blue.png"
                image01.fillMode = Image.Stretch
                someObject.drawRoutePlan()
            }
        }
    }

    Rectangle{
        id:re01
        implicitWidth: widthsize
        implicitHeight: 2
        anchors.top: drawwaypoint.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        color: "#D8D8D8"
    }

    Button{
        id: drawline
        property bool drawingwaypoint:false
        implicitWidth: widthsize
        implicitHeight: widthsize*1.5
        visible:true
        anchors.top: re01.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        background: Rectangle{
            id:drawlineupre
            color:"transparent"
            Image {
                anchors.fill: parent
                fillMode: Image.PreserveAspectFit
                source: "qrc:/qml/resources/qml/DrawLine.png"
                }
        }
        onClicked:
        {

            if(drawingwaypoint)
            {
                drawlineupre.color = "transparent"
                drawingwaypoint = false
            }else
            {
                drawlineupre.color = "#03A8F7"
                drawingwaypoint = true
            }
        }
    }

    Rectangle{
        id:re02
        implicitWidth: widthsize
        implicitHeight: 2
        anchors.top: drawline.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        color: "#D8D8D8"
    }

    Button{
        id: parallelogram
        property bool drawingwaypoint:false
        implicitWidth: widthsize
        implicitHeight: widthsize*1.5
        visible:true
        anchors.top: re02.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        background: Rectangle{
            id:parallelogramre
            color:"transparent"
            Image {
                anchors.fill: parent
                fillMode: Image.PreserveAspectFit
                source: "qrc:/qml/resources/qml/Parallelogram.png"
                }
        }
        onClicked:
        {

            if(drawingwaypoint)
            {
                parallelogramre.color = "transparent"
                drawingwaypoint = false
            }else
            {
                parallelogramre.color = "#03A8F7"
                drawingwaypoint = true
            }
        }
    }

    Rectangle{
        id:re03
        implicitWidth: widthsize
        implicitHeight: 2
        anchors.top: parallelogram.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        color: "#D8D8D8"
    }

    Button{
        id: straightLine
        property bool drawingwaypoint:false
        implicitWidth: widthsize
        implicitHeight: widthsize*1.5
        visible:true
        anchors.top: re03.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        background: Rectangle{
            id:straightLinere
            color:"transparent"
            Image {
                anchors.fill: parent
                fillMode: Image.PreserveAspectFit
                source: "qrc:/qml/resources/qml/StraightLine.png"
                }
        }
        onClicked:
        {

            if(drawingwaypoint)
            {
                straightLinere.color = "transparent"
                drawingwaypoint = false
                straightlineedit.visible = false
            }else
            {
                straightLinere.color = "#03A8F7"
                drawingwaypoint = true
                straightlineedit.visible = true
            }
        }
    }

    Rectangle{
        id:re04
        implicitWidth: widthsize
        implicitHeight: 2
        anchors.top: straightLine.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        color: "#D8D8D8"
    }

    Button{
        id: circular
        property bool drawingwaypoint:false
        implicitWidth: widthsize
        implicitHeight: widthsize*1.5
        visible:true
        anchors.top: re04.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        background: Rectangle{
            id:circularre
            color:"transparent"
            Image {
                id:image02
                anchors.fill: parent
                fillMode: Image.PreserveAspectFit
                source: "qrc:/qml/resources/qml/Circular.png"
                }
        }
        onClicked:
        {

            if(drawingwaypoint)
            {
//                circularre.color = "transparent"
                image02.source = "qrc:/qml/resources/qml/Circular.png"
                image02.fillMode = Image.PreserveAspectFit
                drawingwaypoint = false
            }else
            {
//                circularre.color = "#03A8F7"
                image02.source = "qrc:/qml/resources/qml/circular_Blue.png"
                image02.fillMode = Image.Stretch
                drawingwaypoint = true
            }
        }
    }

}
