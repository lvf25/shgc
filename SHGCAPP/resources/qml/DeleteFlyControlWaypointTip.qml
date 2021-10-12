import QtQuick 2.0
import QtQuick.Controls         1.2
import QtQuick.Controls.Styles  1.4
import QtQuick.Layouts 1.12
import QtQml 2.12

Rectangle{
    id:deleteflycontrolwaypointtip
    property var tipWidth
    property var tipHeight
    property var deletefontsize
    property var success
    property var failure
    color: "#161616"
    opacity: 0.8
    visible: false

    Rectangle{
        id:deletesuccess
        visible: false
        color: "transparent"
        anchors.centerIn: parent
        Label{
            id:_deletesuccess
            anchors.centerIn: parent
            Text{
                anchors.fill: parent
                horizontalAlignment:    Text.AlignHCenter
                verticalAlignment:      Text.AlignVCenter
                font.bold: true
                font.pixelSize: deletefontsize*1.7
                text: qsTr("<font color=\"#FFFFFF\">删除成功</font>")
            }
        }

    }

    Rectangle{
        id:deletefailure
        visible: false
        color: "transparent"
        anchors.centerIn: parent
        Label{
            id:_deletefailure
            anchors.centerIn: parent
            Text{
                anchors.fill: parent
                horizontalAlignment:    Text.AlignHCenter
                verticalAlignment:      Text.AlignVCenter
                font.bold: true
                font.pixelSize: deletefontsize*1.7
                text: qsTr("<font color=\"#FFFFFF\">删除失败</font>")
            }
        }
    }

    Timer{
            id:_timer
            interval: 2000
            repeat: false
            running: true
            triggeredOnStart: false
            onTriggered: {
                closeTipDlg()

            }
        }

    function showSuccessMsg(success){
        deletesuccess.visible = true
        deletefailure.visible = false
        _timer.start()
    }

    function showFailureMsg(failure){
        deletesuccess.visible = false
        deletefailure.visible = true
        _timer.start()
    }

    function closeTipDlg()
    {
        deleteflycontrolwaypointtip.visible = false
        deletesuccess.visible = false
        deletefailure.visible = false

    }



}


