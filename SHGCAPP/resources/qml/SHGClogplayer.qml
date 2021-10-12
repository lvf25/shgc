import QtQuick 2.0
import QtQml 2.0
import QtQuick.Controls         1.4
import QtQuick.Controls.Styles  1.4
import QtQuick.Layouts 1.12
import QtQuick.Window 2.0
import QtWebEngine 1.4
import QtMultimedia 5.4
import QtWebChannel 1.0
import QtPositioning 5.11

import QGroundControl.Controllers       1.0
import QGroundControl                   1.0

Item {
    id:logplay

    property var  _activeVehicle:        QGroundControl.multiVehicleManager.activeVehicle
    property var     fontSize
    property var     logname: ""
    property var     labersize
    property string  time: "00:00:20/00:08:45"
    property var     mavlinkplayer:                  mavlinkplayer
    property int     slidervalue:                    _activeVehicle? mavlinkplayer.sliderValue : 0
    property string  logFilenameShort:               _activeVehicle? mavlinkplayer.logFilenameShort : ""
    property string  logLengthTime:                  _activeVehicle? mavlinkplayer.logCurrentTime + "/" + mavlinkplayer.logLengthTime:"00:00:00/00:00:00"
    property var     relaylink:                      mavlinkplayer.isreplayLink
    property var     model:                          chooselogfile.model

    onSlidervalueChanged: {
        slider.value = slidervalue
    }

    Rectangle{
        RowLayout{
            anchors.centerIn: parent
            spacing: labersize*0.5
            Rectangle{
                id:rec
                implicitHeight: labersize
                implicitWidth: labersize*3
                color: "#161616"
                opacity: 0.8
                radius: 3
                Label
                {
                    id:logname
                    anchors.verticalCenter: parent.verticalCenter
                    horizontalAlignment:    Text.AlignVCenter
                    font.pointSize:  fontSize
                    color: "#FFFFFF"
                    font.bold: false
                    text:  logFilenameShort
                }
            }

            Button{
                id: play
                text:""
                implicitWidth: labersize*0.8
                implicitHeight: labersize*0.8
                visible: true
                style: ButtonStyle{
                    background: BorderImage{
                        source:"qrc:/qml/resources/qml/pause.png"
                        border.left: 0;border.right: 0;border.top: 0;border.bottom: 0
                    }
                }

                onClicked:
                {
                    play.visible = false;
                    pause.visible = true;
                    mavlinkplayer.pause();
                }
            }

            Button{
                id: pause
                text:""
                implicitWidth: labersize*0.8
                implicitHeight: labersize*0.8
                visible: false
                style: ButtonStyle{
                    background: BorderImage{
                        source:"qrc:/qml/resources/qml/play.png"
                        border.left: 0;border.right: 0;border.top: 0;border.bottom: 0
                    }
                }

                onClicked:
                {
                    pause.visible = false;
                    play.visible = true;
                    mavlinkplayer.playPauseToggle()

                }
            }

            RowLayout{
                spacing: 10
                Slider{
                    id:  slider
                    implicitWidth:labersize*10
                    implicitHeight: 10
                    value: slidervalue
                    maximumValue: 100
                    minimumValue:0
                    visible: true
                    wheelEnabled:false
                    updateValueWhileDragging: true
                    onValueChanged:
                    {
                        if(pressed && pause.visible)
                        {
                            mavlinkplayer.setPlayheadFromSlider(slider.value)
                        }else
                        {
                            slider.value = slidervalue
                        }
                    }

                    style: SliderStyle
                    {
                        id:_sliderstyle
                        groove: Rectangle
                        {
                            id:groove
                            anchors.verticalCenter: parent.verticalCenter
                            implicitWidth:labersize*10
                            implicitHeight:10
                            color:"gray"
                            Rectangle
                            {
                                id:line_child
                                height: 10
                                anchors.top:parent.top
                                anchors.left: parent.left
                                width: groove.width * slider.value *0.01
                                color: "#202020"
                            }

                        }
                        handle: Rectangle{
                            anchors.centerIn: parent;
                            color:"#03A8F7";
                            implicitHeight: labersize/3
                            implicitWidth: labersize/3
                            radius: labersize/6
                        }
                    }
                }

                Rectangle{
                    id:rec01
                    implicitHeight: labersize
                    implicitWidth: labersize*4
                    color: Qt.rgba(22/255,22/255,22/255,0)//(1)
                    Label
                    {
                        id:playertime
                        anchors.verticalCenter: parent.verticalCenter
                        horizontalAlignment:    Text.AlignVCenter
                        font.pointSize:  fontSize*1.2
                        color: "#FFFFFF"
                        font.bold: true
                        text:  logLengthTime
                    }
                }
            }

            Button{
                id: openfile
                text:""
                implicitWidth: labersize*0.8
                implicitHeight: labersize*0.8
                visible: true
                style: ButtonStyle{
                    background: BorderImage{
                        source:"qrc:/qml/resources/qml/openfile.png"
                        border.left: 0;border.right: 0;border.top: 0;border.bottom: 0
                    }
                }

                onClicked:
                {
                    chooselogfile.visible = true;
                }
            }


            Button{
                id: close
                text:""
                implicitWidth: labersize*0.8
                implicitHeight: labersize*0.8
                visible: true
                style: ButtonStyle{
                    background: BorderImage{
                        source:"qrc:/qml/resources/qml/closelog.png"
                        border.left: 0;border.right: 0;border.top: 0;border.bottom: 0
                    }
                }
                onClicked:
                {
                    logplay.visible = false
                }
            }
        }
    }

    QGCMAVLinkLogPlayer{
        id:mavlinkplayer
        onUpdateSql: model.select();
    }


}
