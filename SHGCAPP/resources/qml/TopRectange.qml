import QtQuick 2.0
import QtQml 2.0
import QtQuick.Controls         2.5
import QtQuick.Controls.Styles  1.4
import QtQuick.Layouts 1.12


import QGroundControl           1.0


///@topmainwindow
Rectangle{
    id:topmainwindow
    visible: true
    color: "#161616"
    opacity: 0.8

    property var topwidth
    property var topheight
    property var topfontsize
    property var powershow

    property var  _activeVehicle:       QGroundControl.multiVehicleManager.activeVehicle
    property bool _communicationLost:   _activeVehicle ? _activeVehicle.connectionLost : false
    property bool _connect:             _activeVehicle ? true:false

    property real altitudeRelative:     _activeVehicle ? _activeVehicle.altitudeRelative.rawValue :0
    property real climbRate:            _activeVehicle ?_activeVehicle.climbRate.rawValue:0
    property real distanceToGCS:        _activeVehicle ?_activeVehicle.distanceToGCS.value:0
    property var voltage:               _activeVehicle ?_activeVehicle.battery.voltage.value: 0
    property var satellites_Num:        _activeVehicle ?_activeVehicle.gps.count.value:0

    property var flightMode:            _activeVehicle ?_activeVehicle.flightMode:qsTr("设备未连接")
    property real dropRate:             _activeVehicle ?_activeVehicle.droprate.rawValue :0




    onFlightModeChanged: {
        mode.text = flightMode
    }

    onAltitudeRelativeChanged: {
        showflightaltitude.text = altitudeRelative.toFixed(2) +" m"
    }

    onClimbRateChanged: {
        showclimbspeed.text = climbRate.toFixed(2) +" m/s"
    }

    onSatellites_NumChanged: {
        showsatellitesnumber.text = satellites_Num
    }

    onVoltageChanged: {
        showbatteryvoltage.text = voltage.toFixed(2) + " v"
    }

    onDistanceToGCSChanged: {
        showflightdistance.text = distanceToGCS.toFixed(2) + " m"
    }

    on_CommunicationLostChanged:
    {
        if(_communicationLost)
        {
            showisconect.text = qsTr("连接已中断")
        }else
        {
            showisconect.text = qsTr("")

        }
    }

    onDropRateChanged: {
        showradiosignal.text = dropRate+ "%"
    }


    Button{
        id:mainmenu
        implicitWidth: topheight*0.7
        implicitHeight: topheight*0.7
        anchors.left: parent.left
        anchors.leftMargin: topfontsize
        anchors.top: parent.top
        anchors.topMargin: topheight/8

        background: Rectangle{
            id:_enterdataconn
            color:"transparent"
            Image {
                id:image00
                anchors.fill: parent
                fillMode: Image.PreserveAspectFit
                source: "qrc:/qml/resources/qml/shihuanlogowhite.png"
            }
        }
        onClicked:
        {
            root.showLeftMainWindow()
        }
    }


    RowLayout{
        id:layout01
        spacing: topfontsize
        anchors.left: mainmenu.right
        anchors.leftMargin: topheight
        anchors.top: parent.top
        anchors.topMargin: topheight*0.1
        Label{
            id:mode
            Layout.fillWidth: true
            Layout.minimumWidth: topheight*2.2
            Layout.minimumHeight: topheight*0.8
            horizontalAlignment:    Text.AlignHCenter
            verticalAlignment:      Text.AlignVCenter
            font.bold: true
            font.pixelSize: topfontsize*1.7
            color: "#FFFFFF"
            text: flightMode
        }
        Button{
            id:modechoose
            implicitWidth: topheight*0.3
            implicitHeight: topheight*0.3
            Layout.fillWidth: false
            Layout.minimumWidth: topheight*0.3
            Layout.minimumHeight: topheight*0.3
            background: Rectangle{
                id:_modechoose
                color:"transparent"
                Image {
                    id:image01
                    anchors.fill: parent
                    fillMode: Image.PreserveAspectFit
                    source: "qrc:/qml/resources/qml/modeSelection.png"
                }
            }
            onClicked:
            {

            }
        }
    }

    RowLayout{
        id: layout02
        spacing: topfontsize
        anchors.left: layout01.right
        anchors.leftMargin: topheight
        anchors.top: parent.top
        anchors.topMargin: 0
        Rectangle{
            id: alticon
            Layout.fillWidth: true
            width: topheight*0.6
            height: topheight*0.6
            Layout.minimumWidth: topheight*0.6
            Layout.minimumHeight: topheight*0.6
            color: "transparent"
            Image {
                id: image02
                anchors.fill: parent
                fillMode: Image.PreserveAspectFit
                source: "qrc:/qml/resources/qml/flightAltitude.png"
            }
        }

        Label{
            id: showflightaltitude
            Layout.fillWidth: false
            Layout.minimumWidth: topheight
            Layout.minimumHeight: topheight
            horizontalAlignment:    Text.AlignHCenter
            verticalAlignment:      Text.AlignVCenter
            font.bold: true
            font.pixelSize: topfontsize*1.5
            color: "#FFFFFF"
            text: altitudeRelative.toFixed(2) +" m"
        }


    }

    RowLayout{
        id: layout03
        spacing: topfontsize*1.5
        anchors.left: layout02.right
        anchors.leftMargin: topheight
        anchors.top: parent.top
        anchors.topMargin: 0
        Rectangle{
            id: speedicon
            Layout.fillWidth: true
            width: topheight*0.6
            height: topheight*0.6
            Layout.minimumWidth: topheight*0.6
            Layout.minimumHeight: topheight*0.6
            color: "transparent"
            Image {
                id: image03
                anchors.fill: parent
                fillMode: Image.PreserveAspectFit
                source: "qrc:/qml/resources/qml/climbSpeed.png"
            }
        }

        //爬升速度
        Label {
            id: showclimbspeed
            Layout.fillWidth: false
            Layout.minimumWidth: topheight
            Layout.minimumHeight: topheight
            horizontalAlignment:    Text.AlignHCenter
            verticalAlignment:      Text.AlignVCenter
            font.bold: true
            font.pixelSize: topfontsize*1.5
            color: "#FFFFFF"
            text: climbRate.toFixed(2) +" m/s"
        }


    }

    RowLayout{
        id: layout04
        spacing: topfontsize
        anchors.left: layout03.right
        anchors.leftMargin: topheight
        anchors.top: parent.top
        anchors.topMargin: 0
        Rectangle{
            id: distanceicon
            Layout.fillWidth: true
            width: topheight*0.6
            height: topheight*0.6
            Layout.minimumWidth: topheight*0.6
            Layout.minimumHeight: topheight*0.6
            color: "transparent"
            Image {
                id: image04
                anchors.fill: parent
                fillMode: Image.PreserveAspectFit
                source: "qrc:/qml/resources/qml/flightDistance.png"
            }
        }

        Label {
            id: showflightdistance
            Layout.fillWidth: false
            Layout.minimumWidth: topheight
            Layout.minimumHeight: topheight
            horizontalAlignment:    Text.AlignHCenter
            verticalAlignment:      Text.AlignVCenter
            font.bold: true
            font.pixelSize: topfontsize*1.5
            color: "#FFFFFF"
            text: distanceToGCS.toFixed(2) + " m"
        }


    }

    RowLayout{
        id: layout05
        spacing: topfontsize
        anchors.left: layout04.right
        anchors.leftMargin: topheight
        anchors.top: parent.top
        anchors.topMargin: 0

        Rectangle{
            id: voltageicon
            width: topheight*0.6
            height: topheight*0.6
            Layout.fillWidth: true
            Layout.minimumWidth: topheight*0.6
            Layout.minimumHeight: topheight*0.6
            color: "transparent"
            Image {
                id: image05
                anchors.fill: parent
                fillMode: Image.PreserveAspectFit
                source: "qrc:/qml/resources/qml/batteryVoltage.png"
            }
        }

        //电压
        Label {
            id: showbatteryvoltage
            Layout.fillWidth: false
            Layout.minimumWidth: topheight
            Layout.minimumHeight: topheight
            horizontalAlignment:    Text.AlignHCenter
            verticalAlignment:      Text.AlignVCenter
            font.bold: true
            font.pixelSize: topfontsize*1.5
            color: "#FFFFFF"
            text: voltage.toFixed(2) + " v"
        }


    }

    RowLayout{
        id: layout07
        spacing:topfontsize*0.5
        anchors.left: layout05.right
        anchors.leftMargin: topheight
        anchors.top: parent.top
        anchors.topMargin: 0
        Rectangle{
            id: satellitesicon
            width: topheight*0.6
            height: topheight*0.6
            Layout.fillWidth: true
            Layout.minimumWidth: topheight*0.6
            Layout.minimumHeight: topheight*0.6
            color: "transparent"
            Image {
                id: image08
                anchors.fill: parent
                fillMode: Image.PreserveAspectFit
                source: "qrc:/qml/resources/qml/satellites.png"
            }
        }
      //卫星数
        Label {
            id: showsatellitesnumber
            Layout.fillWidth: false
            Layout.minimumWidth: topheight
            Layout.minimumHeight: topheight
            horizontalAlignment:    Text.AlignHCenter
            verticalAlignment:      Text.AlignVCenter
            font.bold: true
            font.pixelSize: topfontsize*1.5
            color: "#FFFFFF"
            text: satellites_Num
        }
    }

    RowLayout{
        id: layout08
        spacing:topfontsize
        anchors.left: layout07.right
        anchors.leftMargin: topheight
        anchors.top: parent.top
        anchors.topMargin: 0

        Rectangle{
            id: radiosignalicon
            width: topheight*0.6
            height: topheight*0.6
            Layout.fillWidth: true
            Layout.minimumWidth: topheight*0.6
            Layout.minimumHeight: topheight*0.6
            color: "transparent"
            Image {
                id: image09
                anchors.fill: parent
                fillMode: Image.PreserveAspectFit
                source: "qrc:/qml/resources/qml/radioSignal.png"
            }
        }

        //掉包率
        Label {
            id: showradiosignal
            Layout.fillWidth: false
            Layout.minimumWidth: topheight
            Layout.minimumHeight: topheight
            horizontalAlignment:    Text.AlignHCenter
            verticalAlignment:      Text.AlignVCenter
            font.bold: true
            font.pixelSize: topfontsize*1.5
            color: "#FFFFFF"
            text: dropRate + "%"
        }
    }


    RowLayout
    {
        id: layout06
        spacing:topfontsize
        anchors.right: parent.right
        anchors.rightMargin: topheight
        anchors.left: layout08.right
        anchors.leftMargin: topheight*2
        anchors.top: parent.top
        anchors.topMargin: 0

        Label {
            id: showisconect
            Layout.fillWidth: false
            Layout.minimumWidth: topheight
            Layout.minimumHeight: topheight
            horizontalAlignment:    Text.AlignHCenter
            verticalAlignment:      Text.AlignVCenter
            font.bold: true
            font.pixelSize: topfontsize*1.5
            color: "#FF0000"
            text: qsTr("")
        }

        //连接无人机
        Button{
            id:diconnect
            implicitWidth: topheight*0.6
            implicitHeight: topheight*0.6
            visible: !_connect
            background: Rectangle{
                id:diconnect_img
                color:"transparent"
                Image {
                    id:image1_disconnect
                    anchors.fill: parent
                    fillMode: Image.PreserveAspectFit
                    source: "qrc:/qml/resources/qml/disconnect.png"
                }
            }
            onClicked:
            {
                root.connectUAVSerialLink(true)
            }
        }

        Button{
            id:connect
            implicitWidth: topheight*0.6
            implicitHeight: topheight*0.6
            visible: _connect
            background: Rectangle{
                id:connect_img
                color:"transparent"
                Image {
                    id:image2_connect
                    anchors.fill: parent
                    fillMode: Image.PreserveAspectFit
                    source: "qrc:/qml/resources/qml/serialport.png"
                }
            }
            onClicked:
            {
                root.connectUAVSerialLink(false)
                someObject.clearLoglines();

            }
        }
    }

//    RowLayout{
//        id: layout06
//        spacing:topfontsize*0.8
//        anchors.right: serialport.right
//        anchors.rightMargin: topheight*8.5
//        anchors.top: parent.top
//        anchors.topMargin: topheight*0.2
//        width: topheight*0.6

//        Label {
//            id: _control
//            Layout.fillWidth: true
//            Layout.minimumWidth: topheight*0.6
//            Layout.minimumHeight: topheight*0.6
//            horizontalAlignment:    Text.AlignHCenter
//            verticalAlignment:      Text.AlignVCenter
//            font.bold: true
//            font.pixelSize: topfontsize*1.5
//            color: "#FFFFFF"
//            text: qsTr("控制权：")
//        }
//        Rectangle{
//            id: controlicon01
//            Layout.fillWidth: false
//            Layout.minimumWidth: topheight*0.6
//            Layout.minimumHeight: topheight*0.6
//            width: topheight*0.6
//            height: topheight*0.6
//            color: "transparent"
//            Image {
//                id: image06
//                anchors.fill: parent
//                fillMode: Image.PreserveAspectFit
//                source: "qrc:/qml/resources/qml/remotecontrol.png"
//            }
//        }
//        Rectangle{
//            id: controlicon02
//            Layout.fillWidth: false
//            Layout.minimumWidth: topheight*0.6
//            Layout.minimumHeight: topheight*0.6
//            width: topheight*0.6
//            height: topheight*0.6
//            color: "transparent"
//            Image {
//                id: image07
//                anchors.fill: parent
//                fillMode: Image.PreserveAspectFit
//                source: "qrc:/qml/resources/qml/pc.png"
//            }
//        }
//    }


}


