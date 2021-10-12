import QtQuick 2.0
import QtQml 2.0
import QtQuick.Controls         1.2
import QtQuick.Controls.Styles  1.4
import QtQuick.Layouts 1.12


Rectangle {
    id:_root
    property int switchsize
    visible: false
    opacity: 0.8
    Image {
        id:switchimage
        width: switchsize*4.5
        height: switchsize*1.1
        fillMode: Image.Stretch
        source: "qrc:/qml/resources/qml/level_BottomFrame.png"
    }

    RowLayout{
        spacing: switchsize *0.5
        anchors.centerIn: switchimage
        RowLayout{
            spacing: 0
            Label{
                id:waypointshow
                horizontalAlignment:    Text.AlignLeft
                font.pointSize:  shihuanfontsize
                color: "#FFFFFF"
                font.bold: true
                text: qsTr("航点:")
            }
            SwitchButton{
                id:waypointswitch
                implicitHeight: switchsize*0.6
                implicitWidth: switchsize*1.2
                onClicked :
                {
                    if(waypointswitch.checked)
                    {
                        someObject.hidePointData()
                    }else
                    {
                        someObject.showPointData()
                    }
                }
            }
        }

        RowLayout{
            spacing: 0
            Label{
                id:wayroute
                horizontalAlignment:    Text.AlignLeft
                font.pointSize:  shihuanfontsize
                color: "#FFFFFF"
                font.bold: true
                text: qsTr("航线:")
            }
            SwitchButton{
                id:watrouteswitch
                implicitHeight: switchsize*0.6
                implicitWidth: switchsize*1.2
                onClicked :
                {
                    if(watrouteswitch.checked)
                    {
                        someObject.hideLineData()
                    }else
                    {
                        someObject.showLineData()
                    }
                }
            }
        }
    }





}
