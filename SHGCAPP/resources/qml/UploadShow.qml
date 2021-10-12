import QtQuick 2.0
import QtQuick.Dialogs  1.2
import QtQuick.Controls         1.2
import QtQuick.Controls.Styles  1.4
import QtQuick.Layouts 1.12

Rectangle {
    id:uploadshow


    Rectangle{
        id:rec
        implicitHeight:button_width
        implicitWidth: parent.width
        anchors.top: parent.top
        anchors.left: parent.left
        color: "#03A8F7"
        Label{
            anchors.centerIn: parent
            Text{
                anchors.fill: parent
                horizontalAlignment:    Text.AlignHCenter
                verticalAlignment:      Text.AlignVCenter
                font.bold: true
                font.pixelSize: shihuanfontsize*2
                text: qsTr("<font color=\"#FFFFFF\">确定无误后，点击“确定”按钮上传</font>")
            }
        }
    }

    TableViewItem{
        id : tableitem
        implicitWidth: parent.width
        anchors.top: rec.bottom
        anchors.left: parent.left
        missionController:playview._missionController
    }

    Rectangle{
        id:btn
        implicitHeight: button_width
        implicitWidth: button_width*7
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottom : parent.bottom
        Button{
            id:uploadOk
            implicitWidth: button_width*3
            implicitHeight: button_width*0.8
            anchors.verticalCenter: parent.verticalCenter
            anchors.left: parent.left

            Text{
                wrapMode: Text.WordWrap
                font.bold: true
                font.pixelSize: shihuanfontsize*1.5
                anchors.centerIn: parent
                text: uploadOk.pressed ? qsTr("<font color=\"#FFFFFF\">上传</font>") : qsTr("<font color=\"#03A8F7\">上传</font>")
            }


            style: ButtonStyle
            {
                id:_buttonstyle
                background: Rectangle{
                    color: control.pressed ? "#03A8F7" : "transparent"
                    radius:button_width*0.4
                    border.color: "#03A8F7"
                    border.width: 2
                }
            }
        }

        Button{
            id:cancelupload
            property bool deleteokpressed: false
            implicitWidth: button_width*3
            implicitHeight: button_width*0.8
            anchors.verticalCenter: parent.verticalCenter
            anchors.right: parent.right
            Text{
                wrapMode: Text.WordWrap
                font.bold: true
                font.pixelSize: shihuanfontsize*1.5
                anchors.centerIn: parent
                text: cancelupload.pressed ? qsTr("<font color=\"#FFFFFF\">取消</font>") : qsTr("<font color=\"#03A8F7\">取消</font>")
            }

            style: ButtonStyle
            {
                background: Rectangle{
                    color: control.pressed ? "#03A8F7" : "transparent"
                    radius:button_width*0.4
                    border.color: "#03A8F7"
                    border.width: 2
                }
            }

        }
    }








}
