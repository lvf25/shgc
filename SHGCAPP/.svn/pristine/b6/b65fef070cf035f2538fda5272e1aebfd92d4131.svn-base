import QtQuick 2.0
import QtQml 2.0
import QtQuick.Controls 2.5
import QtQuick.Controls.Styles  1.4
import QtQuick.Layouts 1.12
import QGroundControl               1.0


Rectangle {
    id:chooselog
    color: Qt.rgba(22/255,22/255,22/255,0.8)
    property var fontSize
    property var labersize
    property var model:      logtableview.model
    property var mavlinkplayer

    ColumnLayout
    {
        layoutDirection:Qt.Horizontal
//        layoutDirection: Qt.LeftToRight
        spacing: 3
        RowLayout{
            spacing: labersize*0.5
            Rectangle{
                id:title
                implicitHeight: root.height/15
                implicitWidth: chooselog.width*0.6
                color: Qt.rgba(22/255,22/255,22/255,0)//(1)
                Label
                {
                    anchors.verticalCenter: parent.verticalCenter
                    horizontalAlignment:    Text.AlignLeft
                    font.pointSize:  fontSize*1.5
                    color: "#FFFFFF"
                    font.bold: true
                    text: qsTr("历史播放记录")
                }
            }

            Button{
                id: openfile
                text:""
                z:chooselog.z+1
                implicitWidth: labersize*0.4
                implicitHeight: labersize*0.4
                visible: true

                background: Rectangle{
                    color:"transparent"
                    Image {
                        anchors.fill: parent
                        fillMode: Image.PreserveAspectFit
                        source: "qrc:/qml/resources/qml/openlogfile.png"
                        }
                }


                onClicked:
                {
                    logplayer.mavlinkplayer.selectLogFileForPlayback()
                }
            }

            Button{
                id: closefile
                text:""
                implicitWidth: labersize*0.4
                implicitHeight: labersize*0.4
                z:chooselog.z+1
                visible: true

                background: Rectangle{
                    color:"transparent"
                    Image {
                        anchors.fill: parent
                        fillMode: Image.PreserveAspectFit
                        source: "qrc:/qml/resources/qml/closelog.png"
                        }
                }


                onClicked:
                {
                    chooselog.visible = false
                }
            }
        }


        TextField
        {
            id:selectFile
            implicitHeight: root.height/15
            implicitWidth: chooselog.width*0.8
//            anchors.horizontalCenter: parent.horizontalCenter
            placeholderText: qsTr("请输入日期或文件名称")
            placeholderTextColor:"#C8C8C8"
            color: "#C8C8C8"
            font.pointSize:  fontSize
            text:qsTr("")
            onPressed:
            {
                if(selectFile.text)
                {
                    model.setFilters(selectFile.text);
                    model.select();
                }
            }

            background: Rectangle {
                radius: 2
                border.color: "#C8C8C8"
                border.width: 2
                color: Qt.rgba(22/255,22/255,22/255,0)
            }
        }

        SHGCLogTableView{
            id:logtableview
            implicitHeight: chooselog.height-root.height/15*2.5
            implicitWidth: chooselog.width
            tablewidth: root.height/15
        }

    }

}
