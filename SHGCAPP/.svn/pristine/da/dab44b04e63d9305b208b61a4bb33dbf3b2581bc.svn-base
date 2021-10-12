import QtQuick 2.0
import QtQml 2.0
import QtQuick.Controls         1.2
import QtQuick.Controls.Styles  1.4
import QtQuick.Layouts 1.12

Rectangle{
    id: deletdConfirm
    property var waypointtip
    property var deleteTipWidth
    property var deleteTipHeight
    property var fontsize
    property var success
    property var failure

    signal showSuccessMsg(var success)
    signal showFailureMsg(var failure)


    onShowSuccessMsg: {
        waypointtip.showSuccessMsg(success)
    }
    onShowFailureMsg: {
        waypointtip.showFailureMsg(failure)
    }

    opacity: 0.8
    color: "#161616"
    visible: false
    property bool deleteokpressed: false
    property bool deletecanclepressed: true


    ColumnLayout{
        id:confirmColumn
        anchors.centerIn: parent
        spacing: deleteTipHeight/5


        ColumnLayout{
            spacing: fontsize
            Layout.alignment: Qt.AlignHCenter
            Label{
                id:deleteTitle
                Text{
                    anchors.fill: parent
                    horizontalAlignment:    Text.AlignHCenter
                    font.bold: true
                    font.pixelSize: fontsize*2
                    text: qsTr("<font color=\"#FFFFFF\">删除飞控上的航点？</font>")
                }
            }

            Label{
                id:deleteTip
                Text{
                    anchors.fill: parent
                    horizontalAlignment:    Text.AlignHCenter
                    font.bold: true;
                    font.pixelSize: fontsize*1.7;
                    text: qsTr("<font color=\"#FFFFFF\">删除后无法恢复，请谨慎操作！</font>")
                }
            }
        }

        RowLayout{
            id:_buttons
            spacing: deleteTipWidth*0.2

            Button{
                id:deleteOk
                property bool deleteokpressed: false
                implicitWidth: deleteTipHeight*0.4
                implicitHeight: deleteTipHeight*0.2


                Text{
                    wrapMode: Text.WordWrap
                    font.bold: true
                    font.pixelSize: fontsize*2
                    anchors.centerIn: parent
                    text: deleteOk.pressed ? qsTr("<font color=\"#FFFFFF\">删除</font>") : qsTr("<font color=\"#03A8F7\">删除</font>")
                }


                style: ButtonStyle
                {
                    id:_buttonstyle
                    background: Rectangle{
                        id:_deleteok
                        color: deleteOk.pressed ? "#03A8F7" : "transparent"
                        radius:deleteTipHeight*0.1
                        border.color: "#03A8F7"
                        border.width: 2
                    }
                }


                onClicked: {
                    deletdConfirm.visible = false
                    waypointtip.visible = true
                    showSuccessMsg(success)
                }



            }

            Button{
                id:deleteCancle
//                property bool deletecanclepressed: true
                implicitWidth: deleteTipHeight*0.4
                implicitHeight: deleteTipHeight*0.2
                isDefault: true


                Text{
                    wrapMode: Text.WordWrap
                    font.bold: true
                    font.pixelSize: fontsize*2
                    anchors.centerIn: parent
                    text: deleteCancle.pressed ? qsTr("<font color=\"#03A8F7\">取消</font>") : qsTr("<font color=\"#FFFFFF\">取消</font>")
                }


                style: ButtonStyle
                {
                    id:_buttonstyles
                    background: Rectangle{
                        id:_deletecancle
                        color: deleteCancle.pressed ? "transparent" : "#03A8F7"
                        radius:deleteTipHeight*0.1
                        border.color: "#03A8F7"
                        border.width: 2
                    }
                }

                onClicked: {
                    deletdConfirm.visible = false
                    waypointtip.visible = true
                    showFailureMsg(failure)
                }



            }


        }

    }


}


