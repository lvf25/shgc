import QtQuick 2.0
import QtQuick.Controls 1.2
import QtQuick.Dialogs  1.2
import QtLocation       5.3
import QtPositioning    5.3
import QtQuick.Layouts  1.2
import QtQuick.Window   2.2
import QtLocation       5.3
import QtPositioning    5.3


Rectangle {
    id:_upload
    radius: button_width/2
//    color: "#161616"
    color: Qt.rgba(22/255,22/255,22/255,1)//(1)
    opacity: 0.8

    Label{
        anchors.centerIn: parent
        Text{
            anchors.fill: parent
            horizontalAlignment:    Text.AlignHCenter
            verticalAlignment:      Text.AlignVCenter
            font.bold: true
            font.pixelSize: shihuanfontsize*2
            text: qsTr("<font color=\"#FFFFFF\">上传到飞控</font>")
        }
    }

    MouseArea{
        anchors.fill: parent
        onClicked: {
            root.showUpload()
        }
    }
}
