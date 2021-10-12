import QtQuick 2.0
//import QtQuick.Controls         1.2
import QtQuick.Controls 2.5
import QtQuick.Controls.Styles  1.4
import QtQuick.Layouts 1.12


Rectangle{
    id: res
    property int size
    property alias qgcposition02: qgcposition02
    property alias qgcposition01: qgcposition01
    property alias flightposition01: flightposition01
    property alias flightposition02: flightposition02
    visible: false
    opacity: 0.8
    Image {
        width: size*3.6
        height: size*1.1
        fillMode: Image.Stretch
        source: "qrc:/qml/resources/qml/level_BottomFrame.png"
    }

    Button{
        id: takeoffposition01
        implicitWidth: size*0.9
        implicitHeight: size*0.8
        visible:true
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        background: Rectangle{
            id:takeoffposition01re
            color:"transparent"
            Image {
                anchors.fill: parent
                fillMode: Image.PreserveAspectFit
                source: "qrc:/qml/resources/qml/white_starting_Location.png"
                }
        }

        onPressed:
        {
            takeoffposition01re.color = "#03A8F7"
        }

        onReleased: {
            takeoffposition01re.color = "transparent"
        }

        onClicked:
        {
            someObject.setStartPoiCenter()
        }
    }


    Button{
        id: qgcposition01
        implicitWidth: size*0.9
        implicitHeight: size*0.8
        visible:true
        anchors.right: takeoffposition01.left
        anchors.bottom: parent.bottom

        background: Rectangle{
            id: qgcposition01re
            color: "transparent"
            Image {
            anchors.fill: parent
            fillMode: Image.PreserveAspectFit
            source: "qrc:/qml/resources/qml/white_my_Location.png"
            }
        }

        onPressed:
        {
            qgcposition01re.color = "#03A8F7"
            visibleTimer.restart()
        }

        onReleased: {
            qgcposition01re.color = "transparent"
            visibleTimer.stop()
        }

        onClicked:
        {
            someObject.setQGCCenter(false)
        }

    }

    Button{
        id: qgcposition02
        implicitWidth: size*0.9
        implicitHeight: size*0.8
        visible:false
        anchors.right: takeoffposition01.left
        anchors.bottom: parent.bottom

        background: BorderImage{
            source:"qrc:/qml/resources/qml/white_my_Location_Blue1.png"
            border.left: 0;border.right: 0;border.top: 0;border.bottom: 0
        }

        onPressed:
        {

        }

        onClicked:
        {
            qgcposition01re.color = "transparent"
            qgcposition02.visible = false
            qgcposition01.visible = true

            someObject.setQGCCenter(false)

        }

    }


    Button{
        id: homeposition
        autoRepeat: true
        implicitWidth: size*0.9
        implicitHeight: size*0.8
        visible:true
        anchors.right: takeoffposition01.left
        anchors.rightMargin: size*0.9
        anchors.bottom: parent.bottom

        background: Rectangle{
            id:homepositionre
            color:"transparent"
            Image {
                anchors.fill: parent
                fillMode: Image.PreserveAspectFit
                source: "qrc:/qml/resources/qml/white_turnback_Location.png"
                }
        }

        onPressed:
        {
            homepositionre.color = "#03A8F7"
        }

        onReleased: {
            homepositionre.color = "transparent"
        }

        onClicked:
        {
            someObject.setHomeCenter()
        }
    }


    Button{
        id: flightposition01
//        autoRepeat: true
        implicitWidth: size*0.9
        implicitHeight: size*0.8
        visible:true
        anchors.right: homeposition.left
        anchors.bottom: parent.bottom

        background: Rectangle{
            id:flightpositionre
            color:"transparent"
            Image {
                anchors.fill: parent
                fillMode: Image.PreserveAspectFit
                source: "qrc:/qml/resources/qml/thePlane_Location.png"
                }
        }

        onPressed:
        {
            flightpositionre.color = "#03A8F7"
            flightTimer.restart()
        }

        onReleased: {
            flightpositionre.color = "transparent"
            flightTimer.stop()
        }

        onClicked:
        {
            someObject.setAircraftCenter(false)
        }
    }

    Button{
        id: flightposition02
        implicitWidth: size*0.9
        implicitHeight: size*0.8
        visible:false
        anchors.right: homeposition.left
        anchors.bottom: parent.bottom

        background: BorderImage{
            source:"qrc:/qml/resources/qml/thePlane_Location_Blue.png"
            border.left: 0;border.right: 0;border.top: 0;border.bottom: 0
        }

        onClicked:
        {
            flightpositionre.color = "transparent"
            flightposition01.visible = true
            flightposition02.visible = false

            someObject.setAircraftCenter(false)
        }

    }


    Timer {
        id:             visibleTimer
        interval:       1000
        triggeredOnStart :false
        repeat:         false
        onTriggered:   {

            someObject.setQGCCenter(true)
            if(qgcposition01.visible)
            {
                qgcposition02.visible = true
                qgcposition01.visible = false
            }

        }
    }

    Timer{
        id:         flightTimer
        interval:   1000
        triggeredOnStart :false
        repeat:     false
        onTriggered: {
            someObject.setAircraftCenter(true)

            if(flightposition01.visible)
            {
                flightposition01.visible = false
                flightposition02.visible = true
            }

        }

    }
}
