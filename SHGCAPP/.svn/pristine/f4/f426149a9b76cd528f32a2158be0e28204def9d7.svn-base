import QtQuick 2.0
import QtQml 2.0
import QtQuick.Controls 2.5
import QtQuick.Controls.Styles  1.4
import QtQuick.Layouts 1.12


Rectangle {
    id:_root
    property int mapsize
    visible: false
    opacity: 0.8
    Image {
        id:mapchooseimage
        width: mapsize*3.6
        height: mapsize*2
        fillMode: Image.Stretch
        source: "qrc:/qml/resources/qml/mapSwitch_Frame.png"
    }

    RowLayout{
        anchors.centerIn: mapchooseimage
        spacing: mapsize*0.6

        ColumnLayout{
            Button{
                id:twodimension
                implicitHeight: mapsize
                implicitWidth: mapsize
                onClicked: {
                    someObject.showNormalMap()
                }

                background: Rectangle{
                    id:twodimensionre
                    radius: mapsize*0.2
                    border.width: 2
                    opacity: 1
                    border.color: "#03A8F7"
        //            color:"transparent"
                    Image {
                        anchors.fill: parent
                        fillMode: Image.PreserveAspectFit
                        source: "qrc:/qml/resources/qml/ATwo_dimensionalMap.png"
                        }
                }
            }

            Label{
                horizontalAlignment:    Text.AlignHCenter
                font.pointSize:  shihuanfontsize
                color: "#FFFFFF"
                font.bold: true
                text: qsTr("二维地图")
            }
        }

        ColumnLayout{
            Button{
                id:satellitemap
                implicitHeight: mapsize
                implicitWidth: mapsize
                onClicked: someObject.showHybridMap()
                background: Rectangle{
                    id:satellitemapre
                    opacity: 1
                    radius: mapsize*0.2
                    border.width: 2
                    border.color: "#03A8F7"
        //            color:"transparent"
                    Image {
                        anchors.fill: parent
                        fillMode: Image.PreserveAspectFit
                        source: "qrc:/qml/resources/qml/SatelliteMaps.png"
                        }
                }
            }

            Label{
                horizontalAlignment:    Text.AlignHCenter
                font.pointSize:  shihuanfontsize
                color: "#FFFFFF"
                font.bold: true
                text: qsTr("卫星地图")
            }
        }
    }





}
