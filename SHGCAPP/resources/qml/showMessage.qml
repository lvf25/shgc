import QtQuick 2.0
import QtQuick.Controls         1.2
import QtQuick.Dialogs          1.2
import QtQuick.Controls.Styles  1.4
import QtQuick.Layouts          1.2


Rectangle {
    id:showmessage
    property var messge: " "
    property var fontsize
    color: "#ffaa00"
    opacity: 0.8
    radius: 5

//    onMessgeChanged: {
//        deleteTip.text = qsTr(messge)
//    }

    Label{
        id:deleteTip
        anchors.fill: parent
        Text{
            anchors.fill: parent
            horizontalAlignment:    Text.AlignHCenter
            verticalAlignment:      Text.AlignVCenter
            font.bold: true;
            font.pixelSize: fontsize*1.5;
            color: "black"
            text: qsTr(messge)
        }
    }

    Button{
        id:closebtn
        implicitHeight: parent.height*0.5
        implicitWidth: parent.height*0.5
        anchors.top: parent.top
        anchors.right: parent.right
        style: ButtonStyle{
            background: BorderImage{
                source:"qrc:/qml/resources/qml/close01.png"
                border.left: 0;border.right: 0;border.top: 0;border.bottom: 0
            }
        }

        onClicked:
        {
            showmessage.visible = 0
        }
    }



}
